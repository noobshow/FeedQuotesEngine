#if !defined DBOperationController_H
#define DBOperationController_H

#include "../stdafx.h"
#include <string>
#include <stdio.h>
//--
#include "ConfigFileRead.h"
#include "MySQLDBOperation.h"
#include "SymbolDBOperation.h"
#include "FeedProviderDBOperation.h"
#include "../MainThreadArgument.h"
#include "../Models/FeedSourceStatusModel.h"

using namespace std;

class SymbolValues;

class DBOperationController
{
private:
	string						mServerIPAddress;
	string						mDBUserName;
	string						mDBUserPassword;
	string						mTargetDatabase;

	MySQLDBOperation			*mMySQLDBOperation;
	SymbolDBOperation			*mSymbolDBOperation;
	FeedProviderDBOperation		*mFeedProviderDBOperation;
	MainThreadArgument*			mMainThreadArgument;

public:
	DBOperationController(MainThreadArgument* aMainThreadArgument);
	~DBOperationController();

	//---
	list<SymbolValues*> loadInputSymbolNameFromDB();
	bool loadSymbolStatus(list<SymbolValues*> aAvailableSymbols);
	bool loadSymbolSpikeMappingInfo(list<SymbolValues*> aAvailableSymbols);
	bool loadSymbolOutText(list<SymbolValues*> aAvailableSymbols);
	bool loadSymbolSourceMappingModel(list<SymbolValues*> aAvailableSymbols);

	//--
	list<FeedSourceStatusModel*> loadAvailableFeedProviders();

private:
	void loadDBConfiguretion();
};

#endif