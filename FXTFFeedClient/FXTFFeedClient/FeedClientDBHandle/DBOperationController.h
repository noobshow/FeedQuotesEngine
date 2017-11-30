#if !defined DBOperationController_H
#define DBOperationController_H

#include "../stdafx.h"
#include <string>
#include <stdio.h>

#include "DBHandle\MySQLDBOperation.h"
#include "FeedClientDBOperation.h"

using namespace std;

class FeedClientDBOperation;

class DBOperationController
{
private:
	MySQLDBOperation			*mMySQLDBOperation;
	FeedClientDBOperation		*mFeedClientDBOperation;

private:
	void loadDBConfiguretion();

public:
	DBOperationController();
	~DBOperationController();

	int SpikeProtectionSettings(string CSVFileData);
	UsersModel* loadLoginUsersName(string aAvailableUsers);
	list<SymbolModel> loadSymbolNames(string aAvailableSymbols);
	list<SymbolOutModel> loadSymbolOut(string aAvailableSymbolOut);
	list<SymbolSpikeMappingModel> loadSymbolSpikeMappingInfo(string aAvailableSymbols);
	list<SymbolSourceMappingModel> loadSymbolSourceMapping(string aSymbolSourceMapping);
	list<SymbolStatusModel> loadSymbolStatusModel(string aSymbolStatus);
	int setOperationToSymbolSourceMapping(int lQueryType,int lQuerySymbolID,int lQueryFeedProviderID);
	int setOperationToAllSymbolSourceMapping(int lTotalSymbolID,int lQueryFeedProviderID);
};

#endif