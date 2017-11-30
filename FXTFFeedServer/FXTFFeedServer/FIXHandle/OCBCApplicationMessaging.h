#ifndef OCBCApplicationMessaging_H
#define OCBCApplicationMessaging_H

#include <iostream>
#include <vector>

#include "quickfix\Application.h"
#include "quickfix\FileLog.h"
#include "quickfix\FileStore.h"
#include "quickfix\fix44\CollateralInquiry.h"
#include "quickfix\fix44\CollateralInquiryAck.h"
#include "quickfix\fix44\CollateralReport.h"
#include "quickfix\fix44\ExecutionReport.h"
#include "quickfix\fix44\MarketDataRequest.h"
#include "quickfix\fix44\MarketDataRequestReject.h"
#include "quickfix\fix44\MarketDataSnapshotFullRefresh.h"
#include "quickfix\fix44\MarketDataIncrementalRefresh.h"
#include "quickfix\fix44\NewOrderList.h"
#include "quickfix\fix44\NewOrderSingle.h"
#include "quickfix\fix44\PositionReport.h"
#include "quickfix\fix44\RequestForPositions.h"
#include "quickfix\fix44\RequestForPositionsAck.h"
#include "quickfix\fix44\SecurityList.h"
#include "quickfix\fix44\TradingSessionStatus.h"
#include "quickfix\fix44\TradingSessionStatusRequest.h"
#include "quickfix\MessageCracker.h"
#include "quickfix\Session.h"

#include "quickfix\SessionSettings.h"
#include "quickfix\SocketInitiator.h"

#include "OCBCFixController.h"
#include "../Settings/SymbolValues.h"

using namespace std;
using namespace FIX;

class OCBCFixController;

class OCBCApplicationMessaging : public MessageCracker, public Application
{
private:
	// Members
	OCBCFixController* mOCBCFixController;

	// Used as a counter for producing unique request identifiers
	unsigned int			mRequestID;
	SessionSettings*		mSessionSettings;
	FileStoreFactory*		mFileStoreFactory;
	FileLogFactory*			mFileLogFactory;
	SocketInitiator*		mSocketInitiator;

	SessionID				mSessionID;
	vector<string>			mListAccountID;

	FIX44::TradingSessionStatus mCurrentSessionStatus;

	// Custom FXCM FIX fields

private:
	//
	string GetNextRequestID();
	void GetRecordAccount(string aAccountID);
	// Functions
	void onCreate(const SessionID& aSession_ID);
	void onLogon(const SessionID& aSession_ID);
	void onLogout(const SessionID& aSession_ID);
	void toAdmin(Message& aToAdminMessage, const SessionID& aSession_ID);
	void toApp(Message& aToAppMessage, const SessionID& aSession_ID);
	void fromAdmin(const Message& aFromAdminMessage, const SessionID& aSession_ID);
	void fromApp(const Message& aFromMessage, const SessionID& aSession_ID);

	//
	void onMessage(const FIX44::TradingSessionStatus& aTradingSessionStatus, const SessionID& aSession_ID);
	void onMessage(const FIX44::MarketDataRequestReject& aMarketDataRequestReject, const SessionID& aSession_ID);
	void onMessage(const FIX44::MarketDataSnapshotFullRefresh& aMarketDataSnapshotFullRefresh, const SessionID& aSession_ID);
	void onMessage(const FIX44::MarketDataIncrementalRefresh& aMarketDataIncrementalRefresh, const SessionID& aSession_ID);



public:
	OCBCApplicationMessaging(OCBCFixController* aOCBCFixController);
	~OCBCApplicationMessaging();

	void StartFixSession(); 
	void EndFixSession();
	void GetTradingSessionStatus();
	
	void SubscribeForMarketData(list<SymbolValues*> aAvailableSymbols);
	void UnsubscribeForMarketData();
	void SubscribeMarketDataForSpecificSymbol(string aSymbolName);
	void reLoginAfterLogout(const SessionID& aSession_ID);
public:
	bool mFIXLoginStatus;
};

#endif