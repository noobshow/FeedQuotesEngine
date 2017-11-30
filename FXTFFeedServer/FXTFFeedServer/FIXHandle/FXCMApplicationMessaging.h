#ifndef FXCMApplicationMessaging_H
#define FXCMApplicationMessaging_H

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

#include "FXCMFixController.h"
#include "../Settings/SymbolValues.h"

using namespace std;
using namespace FIX;

class FXCMFixController;

class FXCMApplicationMessaging : public MessageCracker, public Application
{
private:
	// Members
	FXCMFixController* mFXCMFixController;

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
	enum FXCM_FIX_FIELDS
	{
		FXCM_FIELD_PRODUCT_ID      = 9080,
		FXCM_POS_ID                = 9041,
		FXCM_POS_OPEN_TIME         = 9042,
		FXCM_ERROR_DETAILS         = 9029,
		FXCM_REQUEST_REJECT_REASON = 9025,
		FXCM_USED_MARGIN           = 9038,
		FXCM_POS_CLOSE_TIME        = 9044,
		FXCM_MARGIN_CALL           = 9045,
		FXCM_ORD_TYPE              = 9050,
		FXCM_ORD_STATUS            = 9051,
		FXCM_CLOSE_PNL             = 9052,
		FXCM_SYM_POINT_SIZE        = 9002,
		FXCM_SYM_PRECISION         = 9001,
		FXCM_TRADING_STATUS        = 9096,
		FXCM_PEG_FLUCTUATE_PTS     = 9061,
		FXCM_NO_PARAMS             = 9016,
		FXCM_PARAM_NAME            = 9017,
		FXCM_PARAM_VALUE           = 9018
	};

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

public:
	FXCMApplicationMessaging(FXCMFixController* aFXCMFixController);
	~FXCMApplicationMessaging();

	void StartFixSession(); 
	void EndFixSession();
	void GetTradingSessionStatus();
	
	void SubscribeForMarketData(list<SymbolValues*> aAvailableSymbols);
	void UnsubscribeForMarketData();
	void reLoginAfterLogout(const SessionID& aSession_ID);
public:
	bool mFIXLoginStatus;
};

#endif