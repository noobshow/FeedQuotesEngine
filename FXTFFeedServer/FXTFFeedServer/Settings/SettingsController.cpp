#include "SettingsController.h"
#include "../DBHandle/DBOperationController.h"

SettingsController::SettingsController(MainThreadArgument *aMainThreadArgument)
{
	mMainThreadArgument=aMainThreadArgument;
	mMainThreadArgument->setSettingsController(this);
	loadAvailableFeedProviders();
	loadAvailableSymbolInfo();
}

SettingsController::~SettingsController()
{
}

void SettingsController::loadAvailableSymbolInfo()
{
	mAvailableFeedProviders=mMainThreadArgument->getDBOperationController()->loadAvailableFeedProviders();
	mAvailableSymbolsToHandle=mMainThreadArgument->getDBOperationController()->loadInputSymbolNameFromDB();
	loadSymbolStatusInfo(mAvailableSymbolsToHandle);
	loadSymbolSpikeMappingInfo(mAvailableSymbolsToHandle);
	loadSymbolOutTexts(mAvailableSymbolsToHandle);
	loadSymbolSourceMappingModel(mAvailableSymbolsToHandle);
}

void SettingsController::loadSymbolStatusInfo(list<SymbolValues*> aAvailableSymbolsToHandle)
{
	mMainThreadArgument->getDBOperationController()->loadSymbolStatus(aAvailableSymbolsToHandle);
}

void SettingsController::loadSymbolSpikeMappingInfo(list<SymbolValues*> aAvailableSymbolsToHandle)
{
	mMainThreadArgument->getDBOperationController()->loadSymbolSpikeMappingInfo(aAvailableSymbolsToHandle);
}

void SettingsController::updateSymbolSpikeMappingInfo()
{
	mMainThreadArgument->getDBOperationController()->loadSymbolSpikeMappingInfo(mAvailableSymbolsToHandle);

	for(std::list<SymbolValues*>::iterator list_iter = mAvailableSymbolsToHandle.begin(); list_iter != mAvailableSymbolsToHandle.end(); list_iter++)
	{
		SymbolValues* lTestModelClass=(SymbolValues*)*list_iter;
		SymbolSpikeMappingModel* lSymbolSpikeMappingModel=lTestModelClass->getSymbolSpikeMappingModel();
		cout <<"Symbol Spike Info : AutoIncrID : "<<lSymbolSpikeMappingModel->getAutoIncrimentID()<<" Symbol ID : "<<lSymbolSpikeMappingModel->getSymbolID()<< " Value Percent : "<<lSymbolSpikeMappingModel->getTickValueChangePercent()<<endl;
		mMainThreadArgument->getLogWriter()->LogOutLn(" Symbol ID : "+to_string(lSymbolSpikeMappingModel->getSymbolID())+ " Value Percent : "+to_string(lSymbolSpikeMappingModel->getTickValueChangePercent()));
	}
}
void SettingsController::loadSymbolOutTexts(list<SymbolValues*> aAvailableSymbolsToHandle)
{
	mMainThreadArgument->getDBOperationController()->loadSymbolOutText(aAvailableSymbolsToHandle);
}
void SettingsController::updateSymbolSourceMappingModel()
{
	loadSymbolSourceMappingModel(mAvailableSymbolsToHandle);
}

void SettingsController::loadSymbolSourceMappingModel(list<SymbolValues*> aAvailableSymbolsToHandle)
{
	mMainThreadArgument->getDBOperationController()->loadSymbolSourceMappingModel(aAvailableSymbolsToHandle);
}

void SettingsController::loadAvailableFeedProviders()
{
	mAvailableFeedProviders=mMainThreadArgument->getDBOperationController()->loadAvailableFeedProviders();
}