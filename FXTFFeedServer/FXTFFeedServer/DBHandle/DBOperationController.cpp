#include "DBOperationController.h"



DBOperationController::DBOperationController(MainThreadArgument* aMainThreadArgument )
{
	mMainThreadArgument=aMainThreadArgument;
	aMainThreadArgument->setDBOperationController(this);

	loadDBConfiguretion();

	mMySQLDBOperation =new MySQLDBOperation(mServerIPAddress,mDBUserName,mDBUserPassword,mTargetDatabase);
	mSymbolDBOperation =new SymbolDBOperation(mMySQLDBOperation);
	mFeedProviderDBOperation=new FeedProviderDBOperation(mMySQLDBOperation);

}

void DBOperationController::loadDBConfiguretion()
{
	ConfigFileRead lConfigFileReader(INI_DB_CONFIG_FILE_NAME);
	mServerIPAddress=lConfigFileReader.Get(INI_DATABASE_CONFIG,INI_DB_SERVER_IP_ADDRESS,"");
	mDBUserName=lConfigFileReader.Get(INI_DATABASE_CONFIG,INI_DB_USER_NAME,"");
	mDBUserPassword=lConfigFileReader.Get(INI_DATABASE_CONFIG,INI_DB_USER_PASSWORD,"");
	mTargetDatabase=lConfigFileReader.Get(INI_DATABASE_CONFIG,INI_DATABASE_NAME,"");

}

DBOperationController::~DBOperationController()
{
	delete mMySQLDBOperation;
	delete mSymbolDBOperation;
}

list<SymbolValues*> DBOperationController::loadInputSymbolNameFromDB()
{
	return mSymbolDBOperation->getInputSymbolNames();
}

bool DBOperationController::loadSymbolStatus(list<SymbolValues*> aAvailableSymbols)
{
	mSymbolDBOperation->getInputSymbolStatus(aAvailableSymbols);
	return true;
}

bool DBOperationController::loadSymbolSpikeMappingInfo(list<SymbolValues*> aAvailableSymbols)
{
	mSymbolDBOperation->getInputSymbolSpikeMappingInfo(aAvailableSymbols);
	return true;
}

bool DBOperationController::loadSymbolOutText(list<SymbolValues*> aAvailableSymbols)
{
	mSymbolDBOperation->getSymbolOutTexts(aAvailableSymbols);
	return true;
}

bool DBOperationController::loadSymbolSourceMappingModel(list<SymbolValues*> aAvailableSymbols)
{
	mSymbolDBOperation->getSymbolSourceMappingModel(aAvailableSymbols);
	return true;
}

list<FeedSourceStatusModel*> DBOperationController::loadAvailableFeedProviders()
{
	return mFeedProviderDBOperation->getAvailableFeedProviders();
}