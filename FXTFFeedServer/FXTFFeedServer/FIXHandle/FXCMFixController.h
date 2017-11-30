#if !defined FXCMFixController_H
#define FXCMFixController_H
#include "../stdafx.h"
#include "../MainThreadArgument.h"
#include "FXCMApplicationMessaging.h"
#include "../Settings/SettingsController.h"
#include "quickfix\SessionID.h"
#include <string>
using namespace std;
using namespace FIX;

class FXCMApplicationMessaging;

class FXCMFixController
{
private:
	//Members
	MainThreadArgument*				mMainThreadArgument;  
	FXCMApplicationMessaging*		mFXCMApplicationMessaging;
	//
	int								mFeedProviderID;
public:
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
	int  getFeedProviderID(){return mFeedProviderID;}
	void fixLogOutOccured(const SessionID& aSession_ID);

public:
	FXCMFixController(MainThreadArgument*  aMainThreadArgument);
	~FXCMFixController();
};

#endif