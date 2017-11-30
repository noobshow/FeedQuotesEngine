#include "DBOperationController.h"
#include "../FeedClientCommon/SingletonFeedClient.h"

DBOperationController::DBOperationController()
{
	loadDBConfiguretion();

	mMySQLDBOperation = SingletonFeedClient::GetInstance()->getMySQLDBOperation();
	mFeedClientDBOperation =new FeedClientDBOperation(mMySQLDBOperation);
}

void DBOperationController::loadDBConfiguretion()
{
}

DBOperationController::~DBOperationController()
{
	delete mMySQLDBOperation;
	delete mFeedClientDBOperation;
}

list<SymbolOutModel> DBOperationController::loadSymbolOut(string aAvailableSymbolOut)
{
	return mFeedClientDBOperation->getSymbolOut(aAvailableSymbolOut);
}

list<SymbolSourceMappingModel> DBOperationController::loadSymbolSourceMapping(string aSymbolSourceMapping)
{
	return mFeedClientDBOperation->getSymbolSourceMapping(aSymbolSourceMapping);
}

list<SymbolStatusModel> DBOperationController::loadSymbolStatusModel(string aSymbolStatus)
{
	return mFeedClientDBOperation->getSymbolStatus(aSymbolStatus);
}
UsersModel* DBOperationController::loadLoginUsersName(string aAvailableUsers)
{
	return mFeedClientDBOperation->getLoginCredential(aAvailableUsers);
}
list<SymbolSpikeMappingModel> DBOperationController::loadSymbolSpikeMappingInfo(string aAvailableSymbols)
{
	return mFeedClientDBOperation->getInputSymbolSpikeMappingInfo(aAvailableSymbols);
}

list<SymbolModel> DBOperationController::loadSymbolNames(string aAvailableSymbols)
{
	return mFeedClientDBOperation->getInputSymbolName(aAvailableSymbols);
}

int DBOperationController::SpikeProtectionSettings(string CSVFileData)
{
	return mFeedClientDBOperation->UpdateTableAsCSVFile(CSVFileData);
}

int DBOperationController::setOperationToSymbolSourceMapping(int lQueryType,int lQuerySymbolID,int lQueryFeedProviderID)
{
	return mFeedClientDBOperation->doOperationToSymbolSourceMapping(lQueryType,lQuerySymbolID,lQueryFeedProviderID);
}

int DBOperationController::setOperationToAllSymbolSourceMapping(int lTotalSymbolID,int lQueryFeedProviderID)
{
	return mFeedClientDBOperation->doOperationToAllSymbolSourceMapping(lTotalSymbolID,lQueryFeedProviderID);
}