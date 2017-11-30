#include "OCBCApplicationMessaging.h"

OCBCApplicationMessaging::OCBCApplicationMessaging(OCBCFixController* aOCBCFixController)
{
	mOCBCFixController=aOCBCFixController;
	mRequestID=1;
	mFIXLoginStatus=false;
}

OCBCApplicationMessaging::~OCBCApplicationMessaging()
{
}

void OCBCApplicationMessaging::onCreate(const SessionID& aSession_ID)
{
	cout << "Session -> created : " <<aSession_ID<< endl;
	mSessionID = aSession_ID;
}

void OCBCApplicationMessaging::onLogon(const SessionID& aSession_ID)
{
	cout << "Session -> logon : " <<aSession_ID<< endl;
	mFIXLoginStatus=true;
	GetTradingSessionStatus();
}

void OCBCApplicationMessaging::onLogout(const SessionID& aSession_ID)
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
	
	mOCBCFixController->fixLogOutOccured(aSession_ID);
}

void OCBCApplicationMessaging::reLoginAfterLogout(const SessionID& aSession_ID)
{
	Session::lookupSession(aSession_ID)->logon();
}

void OCBCApplicationMessaging::toAdmin(Message& aToAdminMessage, const SessionID& aSession_ID)
{

	string lMessageType = aToAdminMessage.getHeader().getField(FIELD::MsgType);
	if(lMessageType == "A"){

		string lUserName	= mSessionSettings->get().getString("Username");
		string lPassword	= mSessionSettings->get().getString("Password");
		string sub_ID		= mSessionSettings->get().getString("SenderCompID");
		aToAdminMessage.getHeader().setField(SenderCompID(sub_ID));
		aToAdminMessage.setField(Username(lUserName));
		aToAdminMessage.setField(Password(lPassword));
	}
}

void OCBCApplicationMessaging::toApp(Message& aToAppMessage, const SessionID& aSession_ID)
{
	string sub_ID = mSessionSettings->get().getString("SenderCompID");
	aToAppMessage.getHeader().setField(SenderCompID(sub_ID));
}

void OCBCApplicationMessaging::fromAdmin(const Message& aFromAdminMessage, const SessionID& aSession_ID)
{
	//cout << "From Admin  Message : "<<aFromAdminMessage<<endl;
	crack(aFromAdminMessage, aSession_ID);
}

void OCBCApplicationMessaging::fromApp(const Message& aFromAppMessage, const SessionID& aSession_ID)
{
	crack(aFromAppMessage, aSession_ID);
}

void OCBCApplicationMessaging::onMessage(const FIX44::TradingSessionStatus& aTradingSessionStatus, const SessionID& aSession_ID)
{
	mCurrentSessionStatus=aTradingSessionStatus;
	// Check TradSesStatus field to see if the trading desk is open or closed
	// 2 = Open; 3 = Closed
	string lTradeStatus = aTradingSessionStatus.getField(FIELD::TradSesStatus);
	cout << "[OCBC]TradingSessionStatus -> TradSesStatus -" << lTradeStatus << endl;
	mOCBCFixController->StartGettingMarketData(atoi(lTradeStatus.c_str()));
}

void OCBCApplicationMessaging::onMessage(const FIX44::MarketDataRequestReject& aMarketDataRequestReject, const SessionID& aSession_ID)
{
	cout << "[OCBC]MarketDataRequestReject -> " <<aMarketDataRequestReject<< endl;
	if(aMarketDataRequestReject.isSetField(FIELD::Text)){
		cout << " Text -> " << aMarketDataRequestReject.getField(FIELD::Text) << endl;
	}
}

void OCBCApplicationMessaging::onMessage(const FIX44::MarketDataSnapshotFullRefresh& aMarketDataSnapshotFullRefresh, const SessionID& aSession_ID)
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
	//lSymbol.erase(3,1);
	//string lFeedMessage=lSymbol+" 0 "+lsBidPrice+" "+lsAskPrice;
	//mOCBCFixController->UpdatedMarketData(lFeedMessage);

	SymbolSourceModel *lSymbolSourceModel=new SymbolSourceModel();
	lSymbolSourceModel->setFeedProviderID(mOCBCFixController->getFeedProviderID());
	lSymbolSourceModel->setFeedSymbol(lSymbol);
	lSymbolSourceModel->setBidValue(bid_price);
	lSymbolSourceModel->setAskValue(ask_price);
	
	//lSymbol.erase(3,1);
	//string lFeedMessage=lSymbol+" 0 "+lsBidPrice+" "+lsAskPrice;
	mOCBCFixController->UpdatedMarketData(lSymbolSourceModel);

}

void OCBCApplicationMessaging::onMessage(const FIX44::MarketDataIncrementalRefresh& aMarketDataSnapshotFullRefresh, const SessionID& aSession_ID)
{
	string lSymbol = "";
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
		lSymbol=group.getField(FIELD::Symbol);
	}
	
	SymbolSourceModel *lSymbolSourceModel=new SymbolSourceModel();
	lSymbolSourceModel->setFeedProviderID(mOCBCFixController->getFeedProviderID());
	lSymbolSourceModel->setFeedSymbol(lSymbol);
	lSymbolSourceModel->setBidValue(bid_price);
	lSymbolSourceModel->setAskValue(ask_price);
	
	//lSymbol.erase(3,1);
	//string lFeedMessage=lSymbol+" 0 "+lsBidPrice+" "+lsAskPrice;
	if (lSymbolSourceModel && (lSymbolSourceModel->getFeedSymbol().compare("")!=0) && lSymbolSourceModel->getBidValue()!=0.0 && lSymbolSourceModel->getAskValue()!=0.0 && lSymbolSourceModel->getBidValue()>0.0 && lSymbolSourceModel->getAskValue()>0.0)
	{
		mOCBCFixController->UpdatedMarketData(lSymbolSourceModel);
	}
}

void OCBCApplicationMessaging::StartFixSession()
{
	try{
		mSessionSettings		= new SessionSettings("settings_ocbc.cfg");
		mFileStoreFactory		= new FileStoreFactory(* mSessionSettings);
		mFileLogFactory			= new FileLogFactory(* mSessionSettings);
		mSocketInitiator		= new SocketInitiator(* this, * mFileStoreFactory, * mSessionSettings, * mFileLogFactory/*Optional*/);

		mSocketInitiator->start();

	}catch(ConfigError error){
		cout << error.what() << endl;
	}
}

// Logout and end session 
void OCBCApplicationMessaging::EndFixSession()
{
	mSocketInitiator->stop();
	delete mSocketInitiator;
	delete mSessionSettings;
	delete mFileStoreFactory;
	delete mFileLogFactory;
}

void OCBCApplicationMessaging::GetTradingSessionStatus()
{
	// Request TradingSessionStatus message 
	FIX44::TradingSessionStatusRequest lRequest;
	lRequest.setField(TradSesReqID(GetNextRequestID()));
	lRequest.setField(TradingSessionID("CNX"));
	lRequest.setField(SubscriptionRequestType(SubscriptionRequestType_SNAPSHOT));
	string sub_ID = mSessionSettings->get().getString("SenderCompID");
	lRequest.setField(SenderCompID(sub_ID));
	//Need To SenderCompID in case OCBC
	Session::sendToTarget(lRequest, mSessionID);
}

void OCBCApplicationMessaging::SubscribeForMarketData(list<SymbolValues*> aAvailableSymbols)
{
	for(std::list<SymbolValues*>::iterator list_iter = aAvailableSymbols.begin(); list_iter != aAvailableSymbols.end(); list_iter++)
	{
		SymbolValues* lSymbolValues=(SymbolValues*)*list_iter;
		if (lSymbolValues->getSymboStatusModel()->isEnabled())
		{
			SubscribeMarketDataForSpecificSymbol(lSymbolValues->getSymbolName());
		}
	}
	/*
	SubscribeMarketDataForSpecificSymbol("AUD/USD");
	SubscribeMarketDataForSpecificSymbol("EUR/CHF");
	SubscribeMarketDataForSpecificSymbol("EUR/GBP");
	SubscribeMarketDataForSpecificSymbol("JPY/USD");
	SubscribeMarketDataForSpecificSymbol("EUR/JPY");
	SubscribeMarketDataForSpecificSymbol("EUR/USD");
	SubscribeMarketDataForSpecificSymbol("GBP/USD");
	SubscribeMarketDataForSpecificSymbol("NZD/USD");
	SubscribeMarketDataForSpecificSymbol("USD/CHF");
	SubscribeMarketDataForSpecificSymbol("AUD/CAD");
	SubscribeMarketDataForSpecificSymbol("AUD/CHF");
	SubscribeMarketDataForSpecificSymbol("AUD/JPY");
	SubscribeMarketDataForSpecificSymbol("AUD/NZD");
	SubscribeMarketDataForSpecificSymbol("AUD/SGD");
	SubscribeMarketDataForSpecificSymbol("CAD/JPY");
	SubscribeMarketDataForSpecificSymbol("CAD/SGD");
	SubscribeMarketDataForSpecificSymbol("CHF/JPY");
	SubscribeMarketDataForSpecificSymbol("DKK/JPY");
	SubscribeMarketDataForSpecificSymbol("EUR/AUD");
	SubscribeMarketDataForSpecificSymbol("EUR/CAD");
	SubscribeMarketDataForSpecificSymbol("EUR/NZD");
	SubscribeMarketDataForSpecificSymbol("EUR/SEK");
	SubscribeMarketDataForSpecificSymbol("EUR/SGD");
	SubscribeMarketDataForSpecificSymbol("EUR/JAR");
	SubscribeMarketDataForSpecificSymbol("GBP/AUD");
	SubscribeMarketDataForSpecificSymbol("GBP/CHF");
	SubscribeMarketDataForSpecificSymbol("GBP/JPY");
	SubscribeMarketDataForSpecificSymbol("HKD/JPY");
	SubscribeMarketDataForSpecificSymbol("NOK/JPY");
	SubscribeMarketDataForSpecificSymbol("NZD/CAD");
	SubscribeMarketDataForSpecificSymbol("NZD/CHF");
	SubscribeMarketDataForSpecificSymbol("NZD/JPY");
	SubscribeMarketDataForSpecificSymbol("NZD/SGD");
	SubscribeMarketDataForSpecificSymbol("PLN/JPY");
	SubscribeMarketDataForSpecificSymbol("SEK/JPY");
	SubscribeMarketDataForSpecificSymbol("TRY/JPY");
	SubscribeMarketDataForSpecificSymbol("USD/HKD");
	SubscribeMarketDataForSpecificSymbol("USD/SEK");
	SubscribeMarketDataForSpecificSymbol("USD/SGD");
	SubscribeMarketDataForSpecificSymbol("USD/ZAR");
	SubscribeMarketDataForSpecificSymbol("ZAR/JPY");
	SubscribeMarketDataForSpecificSymbol("USD/JPY");
	SubscribeMarketDataForSpecificSymbol("USD/CAD");
	SubscribeMarketDataForSpecificSymbol("USD/NOK");

	SubscribeMarketDataForSpecificSymbol("NZD/JPY");
	*/







}

void OCBCApplicationMessaging::SubscribeMarketDataForSpecificSymbol(string aSymbolName)
{
	FIX44::MarketDataRequest lMarketDataRequest;
	lMarketDataRequest.setField(MDReqID(GetNextRequestID()));
	lMarketDataRequest.setField(SubscriptionRequestType(SubscriptionRequestType_SNAPSHOT_PLUS_UPDATES));
	lMarketDataRequest.setField(MarketDepth(1));
	lMarketDataRequest.setField(NoRelatedSym(1));
	lMarketDataRequest.setField(MDUpdateType(1));
	//request.setField(MDUpdateAction(0));
	lMarketDataRequest.setField(NoMDEntryTypes(5));
	
	string sub_ID = mSessionSettings->get().getString("SenderCompID");
	lMarketDataRequest.setField(SenderCompID(sub_ID));

	lMarketDataRequest.setField(AggregatedBook("Y"));

	FIX44::MarketDataRequest::NoRelatedSym symbols_group;
	symbols_group.setField(Symbol(aSymbolName));
	lMarketDataRequest.addGroup(symbols_group);

	FIX44::MarketDataRequest::NoMDEntryTypes entry_types;

	entry_types.setField(MDEntryType(MDEntryType_BID));
	lMarketDataRequest.addGroup(entry_types);
	entry_types.setField(MDEntryType(MDEntryType_OFFER));
	lMarketDataRequest.addGroup(entry_types);
	entry_types.setField(MDEntryType(MDEntryType_TRADING_SESSION_HIGH_PRICE));
	lMarketDataRequest.addGroup(entry_types);
	entry_types.setField(MDEntryType(MDEntryType_TRADING_SESSION_LOW_PRICE));
	lMarketDataRequest.addGroup(entry_types);
	entry_types.setField(MDEntryType('9'));
	lMarketDataRequest.addGroup(entry_types);

	Session::sendToTarget(lMarketDataRequest, mSessionID);
}


void OCBCApplicationMessaging::UnsubscribeForMarketData()
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

string OCBCApplicationMessaging::GetNextRequestID()
{
	if(mRequestID == 65535)
		mRequestID = 1;

	mRequestID++;
	string next_ID = IntConvertor::convert(mRequestID);
	return next_ID;
}