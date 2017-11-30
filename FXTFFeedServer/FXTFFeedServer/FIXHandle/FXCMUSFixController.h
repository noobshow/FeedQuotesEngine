#if !defined FXCMFUSFixController_H
#define FXCMFUSFixController_H
#include "../stdafx.h"
#include "../MainThreadArgument.h"
#include "FXCMUSApplicationMessaging.h"
#include "../Settings/SettingsController.h"
#include "quickfix\SessionID.h"
#include <string>
using namespace std;
using namespace FIX;

class FXCMUSApplicationMessaging;

class FXCMUSFixController
{
private:
	//Members
	MainThreadArgument*				mMainThreadArgument;  
	FXCMUSApplicationMessaging*		mFXCMUSApplicationMessaging;
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
	FXCMUSFixController(MainThreadArgument*  aMainThreadArgument);
	~FXCMUSFixController();
};

#endif