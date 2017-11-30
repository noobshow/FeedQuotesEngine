#if !defined OCBCFixController_H
#define OCBCFixController_H
#include "../stdafx.h"
#include "../MainThreadArgument.h"
#include "OCBCApplicationMessaging.h"
#include "../Settings/SettingsController.h"
#include "quickfix\SessionID.h"
#include <string>
using namespace std;
using namespace FIX;
class OCBCApplicationMessaging;

class OCBCFixController
{
private:
	//Members
	MainThreadArgument*			mMainThreadArgument;  
	OCBCApplicationMessaging*	mOCBCApplicationMessaging;
	int							mFeedProviderID;
private:
	//Functions
	
	
public:
	//Members
	void initializaFIXConnection();
	void StartGettingMarketData(int aTradeSessionStatus);
	void StopGettingMarketData();

	void UpdatedMarketData(SymbolSourceModel* aSymbolSourceModel);
	//--
	void deletePreviousLogFiles();
	void deletingFilesFromDirectory(string aDirectoryPath);
	void setFeedProviderID(int aFeedProviderID){mFeedProviderID=aFeedProviderID;}
	int getFeedProviderID(){return mFeedProviderID;}
	void fixLogOutOccured(const SessionID& aSession_ID);
	
public:
	OCBCFixController(MainThreadArgument*  aMainThreadArgument);
	~OCBCFixController();
};

#endif