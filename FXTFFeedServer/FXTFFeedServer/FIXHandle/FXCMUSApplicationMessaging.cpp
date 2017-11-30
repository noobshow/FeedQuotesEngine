#include "FXCMUSApplicationMessaging.h"

FXCMUSApplicationMessaging::FXCMUSApplicationMessaging(FXCMUSFixController* aFXCMUSFixController)
{
	mFXCMUSFixController=aFXCMUSFixController;
	mRequestID=1;
	mFIXLoginStatus=false;
	
}

FXCMUSApplicationMessaging::~FXCMUSApplicationMessaging()
{
}

void FXCMUSApplicationMessaging::onCreate(const SessionID& aSession_ID)
{
	cout << "Session -> created : " <<aSession_ID<< endl;
	mSessionID = aSession_ID;
}

void FXCMUSApplicationMessaging::onLogon(const SessionID& aSession_ID)
{
	cout << "Session -> logon : " <<aSession_ID<< endl;
	mFIXLoginStatus=true;
	GetTradingSessionStatus();
}

void FXCMUSApplicationMessaging::onLogout(const SessionID& aSession_ID)
{
	mFIXLoginStatus=false;
	cout << "Session -> logout : " <<aSession_ID<< endl;
	FileStore* lFileStore=(FileStore*)Session::lookupSession(aSession_ID)->getStore();
	try
	{
		lFileStore->reset();
		lFileStore->refresh();
	}catch(Exception aEx)
	{
		cout << "Session -> logout : File Store Clear Exception" <<aEx.what()<< endl;
	}
	
	mFXCMUSFixController->fixLogOutOccured(aSession_ID);
}

void FXCMUSApplicationMessaging::reLoginAfterLogout(const SessionID& aSession_ID)
{
	Session::lookupSession(aSession_ID)->logon();
}

void FXCMUSApplicationMessaging::toAdmin(Message& aToAdminMessage, const SessionID& aSession_ID)
{

	string lMessageType = aToAdminMessage.getHeader().getField(FIELD::MsgType);
	if(lMessageType == "A"){

		string lUserName = mSessionSettings->get().getString("Username");
		string lPassword = mSessionSettings->get().getString("Password");
		string lTargetSubID = mSessionSettings->get().getString("TargetSubID"); //No Need For OCBC May need to Add SenderCompID
		aToAdminMessage.getHeader().setField(TargetSubID(lTargetSubID));		//No Need For OCBC May need to Add SenderCompID
		aToAdminMessage.setField(Username(lUserName));
		aToAdminMessage.setField(Password(lPassword));
	}
}

void FXCMUSApplicationMessaging::toApp(Message& aToAppMessage, const SessionID& aSession_ID)
{
	string lTargetSubID = mSessionSettings->get().getString("TargetSubID");		//No Need For OCBC May need to Add SenderCompID
	aToAppMessage.getHeader().setField(TargetSubID(lTargetSubID));				//No Need For OCBC May need to Add SenderCompID
}

void FXCMUSApplicationMessaging::fromAdmin(const Message& aFromAdminMessage, const SessionID& aSession_ID)
{
	cout << "From Admin  Message : "<<aFromAdminMessage<<endl;
	crack(aFromAdminMessage, aSession_ID);
}

void FXCMUSApplicationMessaging::fromApp(const Message& aFromAppMessage, const SessionID& aSession_ID)
{
	crack(aFromAppMessage, aSession_ID);
}

void FXCMUSApplicationMessaging::onMessage(const FIX44::TradingSessionStatus& aTradingSessionStatus, const SessionID& aSession_ID)
{
	mCurrentSessionStatus=aTradingSessionStatus;
	// Check TradSesStatus field to see if the trading desk is open or closed
	// 2 = Open; 3 = Closed
	string trad_status = aTradingSessionStatus.getField(FIELD::TradSesStatus);
	cout << "[FXCMUS]TradingSessionStatus -> TradSesStatus -" << trad_status << endl;
	/*
	// Within the TradingSessionStatus message is an embeded SecurityList. From SecurityList we can see
	// the list of available trading securities and information relevant to each; e.g., point sizes,
	// minimum and maximum order quantities by security, etc. 
	cout << "  SecurityList via TradingSessionStatus -> " << endl;
	int symbols_count = IntConvertor::convert(aTradingSessionStatus.getField(FIELD::NoRelatedSym));
	for(int i = 1; i <= symbols_count; i++){
		// Get the NoRelatedSym group and for each, print out the Symbol value
		FIX44::SecurityList::NoRelatedSym symbols_group;
		aTradingSessionStatus.getGroup(i,symbols_group);
		string symbol = symbols_group.getField(FIELD::Symbol);
		cout << "    Symbol -> " << symbol << endl;
	}
	*/
	mFXCMUSFixController->StartGettingMarketData(atoi(trad_status.c_str()));
}

void FXCMUSApplicationMessaging::onMessage(const FIX44::MarketDataRequestReject& aMarketDataRequestReject, const SessionID& aSession_ID)
{
	cout << "[FXCMUS]MarketDataRequestReject -> " <<aMarketDataRequestReject<< endl;
	if(aMarketDataRequestReject.isSetField(FIELD::Text)){
		cout << " Text -> " << aMarketDataRequestReject.getField(FIELD::Text) << endl;
	}
}

//  ********* 
//    *****
//     **
//Getting Market Data...Here

void FXCMUSApplicationMessaging::onMessage(const FIX44::MarketDataSnapshotFullRefresh& aMarketDataSnapshotFullRefresh, const SessionID& aSession_ID)
{
	string lSymbol = aMarketDataSnapshotFullRefresh.getField(FIELD::Symbol);
	double bid_price = 0;
	double ask_price = 0;
	string lsBidPrice;
	string lsAskPrice;

	int entry_count = IntConvertor::convert(aMarketDataSnapshotFullRefresh.getField(FIELD::NoMDEntries));
	for(int i = 1; i < entry_count; i++){
		FIX44::MarketDataSnapshotFullRefresh::NoMDEntries group;
		aMarketDataSnapshotFullRefresh.getGroup(i,group);
		string entry_type = group.getField(FIELD::MDEntryType);
		if(entry_type == "0"){ // Bid
			lsBidPrice=group.getField(FIELD::MDEntryPx);
			bid_price = DoubleConvertor::convert(group.getField(FIELD::MDEntryPx));
		}else if(entry_type == "1"){ // Ask (Offer)
			ask_price = DoubleConvertor::convert(group.getField(FIELD::MDEntryPx));
			lsAskPrice=group.getField(FIELD::MDEntryPx);
		}
	}
	SymbolSourceModel *lSymbolSourceModel=new SymbolSourceModel();
	lSymbolSourceModel->setFeedProviderID(mFXCMUSFixController->getFeedProviderID());
	lSymbolSourceModel->setFeedSymbol(lSymbol);
	lSymbolSourceModel->setBidValue(bid_price);
	lSymbolSourceModel->setAskValue(ask_price);
	
	//lSymbol.erase(3,1);
	//string lFeedMessage=lSymbol+" 0 "+lsBidPrice+" "+lsAskPrice;

	if (lSymbolSourceModel && (lSymbolSourceModel->getFeedSymbol().compare("")!=0) && lSymbolSourceModel->getBidValue()!=0.0 && lSymbolSourceModel->getAskValue()!=0.0 && lSymbolSourceModel->getBidValue()>0.0 && lSymbolSourceModel->getAskValue()>0.0)
	{
		mFXCMUSFixController->UpdatedMarketData(lSymbolSourceModel);
	}
}

void FXCMUSApplicationMessaging::StartFixSession()
{
	try{
		mSessionSettings		= new SessionSettings("settings_fxcm_us.cfg");
		mFileStoreFactory		= new FileStoreFactory(* mSessionSettings);
		mFileLogFactory			= new FileLogFactory(* mSessionSettings);
		mSocketInitiator		= new SocketInitiator(* this, * mFileStoreFactory, * mSessionSettings, * mFileLogFactory/*Optional*/);

		mSocketInitiator->start();

	}catch(ConfigError error){
		cout << error.what() << endl;
	}
}

// Logout and end session 
void FXCMUSApplicationMessaging::EndFixSession()
{
	mSocketInitiator->stop();
	delete mSocketInitiator;
	delete mSessionSettings;
	delete mFileStoreFactory;
	delete mFileLogFactory;
}

void FXCMUSApplicationMessaging::GetTradingSessionStatus()
{
	// Request TradingSessionStatus message 
	FIX44::TradingSessionStatusRequest lRequest;
	lRequest.setField(TradSesReqID(GetNextRequestID()));
	lRequest.setField(TradingSessionID("FXCM"));
	lRequest.setField(SubscriptionRequestType(SubscriptionRequestType_SNAPSHOT));
	//Need To SenderCompID in case OCBC
	Session::sendToTarget(lRequest, mSessionID);
}

void FXCMUSApplicationMessaging::SubscribeForMarketData(list<SymbolValues*> aAvailableSymbols)
{
	FIX44::MarketDataRequest request;
	request.setField(MDReqID(GetNextRequestID()));
	request.setField(SubscriptionRequestType(
	SubscriptionRequestType_SNAPSHOT_PLUS_UPDATES));
	request.setField(MarketDepth(0));
	request.setField(NoRelatedSym(1));
	
	for(std::list<SymbolValues*>::iterator list_iter = aAvailableSymbols.begin(); list_iter != aAvailableSymbols.end(); list_iter++)
	{
		SymbolValues* lSymbolValues=(SymbolValues*)*list_iter;
		if (lSymbolValues->getSymboStatusModel()->isEnabled())
		{
			FIX44::MarketDataRequest::NoRelatedSym symbols_group;
			symbols_group.setField(Symbol(lSymbolValues->getSymbolName()));
			cout << "    Symbol -> " << lSymbolValues->getSymbolName() << endl;
			request.addGroup(symbols_group);
		}
	}
	
	
	/*
	int symbols_count = IntConvertor::convert(mCurrentSessionStatus.getField(FIELD::NoRelatedSym));
	for(int i = 1; i <= symbols_count; i++){
		// Get the NoRelatedSym group and for each, print out the Symbol value
		FIX44::SecurityList::NoRelatedSym symbols_group;
		mCurrentSessionStatus.getGroup(i,symbols_group);
		string symbol = symbols_group.getField(FIELD::Symbol);
		cout << "    Symbol -> " << symbol << endl;
		//request.addGroup(symbols_group);
	}
	*/

	// Add the NoMDEntryTypes group to the request for each MDEntryType
	// that we are subscribing to. This includes Bid, Offer, High, and Low
	FIX44::MarketDataRequest::NoMDEntryTypes entry_types;
	entry_types.setField(MDEntryType(MDEntryType_BID));
	request.addGroup(entry_types);
	entry_types.setField(MDEntryType(MDEntryType_OFFER));
	request.addGroup(entry_types);
	entry_types.setField(MDEntryType(MDEntryType_TRADING_SESSION_HIGH_PRICE));
	request.addGroup(entry_types);
	entry_types.setField(MDEntryType(MDEntryType_TRADING_SESSION_LOW_PRICE));
	request.addGroup(entry_types);
	//Need To SenderCompID in case OCBC
	Session::sendToTarget(request, mSessionID);
}

void FXCMUSApplicationMessaging::UnsubscribeForMarketData()
{
	// Unsubscribe from EUR/USD. Note that our request_ID is the exact same
	// that was sent for our request to subscribe. This is necessary to 
	// unsubscribe. This request below is identical to our request to subscribe
	// with the exception that SubscriptionRequestType is set to
	// "SubscriptionRequestType_DISABLE_PREVIOUS_SNAPSHOT_PLUS_UPDATE_REQUEST"
	string request_ID = "EUR_USD_Request_";
	FIX44::MarketDataRequest request;
	request.setField(MDReqID(request_ID));
	request.setField(SubscriptionRequestType(
		SubscriptionRequestType_DISABLE_PREVIOUS_SNAPSHOT_PLUS_UPDATE_REQUEST));
	request.setField(MarketDepth(0));
	request.setField(NoRelatedSym(1));

	// Add the NoRelatedSym group to the request with Symbol
	// field set to EUR/USD
	FIX44::MarketDataRequest::NoRelatedSym symbols_group;
	symbols_group.setField(Symbol("EUR/USD"));
	request.addGroup(symbols_group);

	// Add the NoMDEntryTypes group to the request for each MDEntryType
	// that we are subscribing to. This includes Bid, Offer, High, and Low
	FIX44::MarketDataRequest::NoMDEntryTypes entry_types;
	entry_types.setField(MDEntryType(MDEntryType_BID));
	request.addGroup(entry_types);
	entry_types.setField(MDEntryType(MDEntryType_OFFER));
	request.addGroup(entry_types);
	entry_types.setField(MDEntryType(MDEntryType_TRADING_SESSION_HIGH_PRICE));
	request.addGroup(entry_types);
	entry_types.setField(MDEntryType(MDEntryType_TRADING_SESSION_LOW_PRICE));
	request.addGroup(entry_types);

	Session::sendToTarget(request, mSessionID);
}

string FXCMUSApplicationMessaging::GetNextRequestID()
{
	if(mRequestID == 65535)
		mRequestID = 1;

	mRequestID++;
	string next_ID = IntConvertor::convert(mRequestID);
	return next_ID;
}