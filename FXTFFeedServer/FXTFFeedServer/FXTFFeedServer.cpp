// FXTFFeedServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ClientHandle\ClientController.h"
#include "MT4Handle\MT4Controller.h"
#include "FilterProcess\FilterProcessController.h"
#include "FIXHandle\FixHandleController.h"
#include "DBHandle\DBOperationController.h"
#include "MainThreadArgument.h"
#include "Settings\SymbolValues.h"
#include "Settings\SettingsController.h"
#include "Common\FXTFLog.h"

class SymbolValues;


void DisplaySymbolSettingsInfo(list<SymbolValues*> aAvailableSymbols,FXTFLog* aLogWriter)
{
	cout <<" -------------------Showing Symbol Info ----------------------"<<endl;
	for(std::list<SymbolValues*>::iterator list_iter = aAvailableSymbols.begin(); list_iter != aAvailableSymbols.end(); list_iter++)
	{
		SymbolValues* lTestModelClass=(SymbolValues*)*list_iter;
		SymbolStatusModel* lSymbolStatusModel=lTestModelClass->getSymboStatusModel();
		SymbolSpikeMappingModel* lSymbolSpikeMappingModel=lTestModelClass->getSymbolSpikeMappingModel();
		//cout<<*list_iter<<endl;
		cout <<"Symbol Values : AutoIncrID : "<<lTestModelClass->getAutoIncrimentID()<<" SymbolName : "<<lTestModelClass->getSymbolName()<<" Description :"<<lTestModelClass->getDescription()<<endl;
		
		aLogWriter->LogOutLn("Symbol Values : AutoIncrID : "+to_string(lTestModelClass->getAutoIncrimentID())+" SymbolName : "+lTestModelClass->getSymbolName());
		
		cout <<"Symbol Status Values : AutoIncrID : "<<lSymbolStatusModel->getAutoIncrimentID()<<" Symbol ID : "<<lSymbolStatusModel->getSymbolID()<<" is Enable :"<<lSymbolStatusModel->isEnabled()<<endl;
		aLogWriter->LogOutLn(" Symbol ID : "+to_string(lSymbolStatusModel->getSymbolID())+" is Enabled :"+to_string(lSymbolStatusModel->isEnabled()));
		cout <<"Symbol Spike Info : AutoIncrID : "<<lSymbolSpikeMappingModel->getAutoIncrimentID()<<" Symbol ID : "<<lSymbolSpikeMappingModel->getSymbolID()<< " Value Percent : "<<lSymbolSpikeMappingModel->getTickValueChangePercent()<<endl;
		aLogWriter->LogOutLn(" Symbol ID : "+to_string(lSymbolSpikeMappingModel->getSymbolID())+ " Value Percent : "+to_string(lSymbolSpikeMappingModel->getTickValueChangePercent()));
		list<SymbolOutModel*> lSymbolOutTexts=lTestModelClass->getSymbolOutTexts();

		for(std::list<SymbolOutModel*>::iterator list_iter1 = lSymbolOutTexts.begin(); list_iter1 != lSymbolOutTexts.end(); list_iter1++)
		{
			SymbolOutModel* lSymbolOutModel=(SymbolOutModel*)*list_iter1;
			cout <<"\t"<<"SymbolOutText : AutoIncrID"<<lSymbolOutModel->getAutoIncrimentID()<<" SymbolID : "<<lSymbolOutModel->getSymbolID()<<" SymbolOutText : "<<lSymbolOutModel->getSymbolOutText()<<endl;
		}

		list<SymbolSourceMappingModel*> lSymbolSourceModels=lTestModelClass->getSymbolSourceModels();

		for(std::list<SymbolSourceMappingModel*>::iterator list_iter2 = lSymbolSourceModels.begin(); list_iter2 != lSymbolSourceModels.end(); list_iter2++)
		{
			SymbolSourceMappingModel* lSymbolOutModel=(SymbolSourceMappingModel*)*list_iter2;
			cout <<"\t"<<"SymbolSourceModel : AutoIncrID"<<lSymbolOutModel->getAutoIncrimentID()<<" SymbolID : "<<lSymbolOutModel->getSymbolID()<<" Feed ProviderID : "<<lSymbolOutModel->getFeedProviderID()<<endl;
		}
	}
}
void DisplayFeedProviderInfo(list<FeedSourceStatusModel*> aAvailableFeedProviders,FXTFLog* aLogWriter)
{
	cout <<" -------------------Showing Feed provider Info ----------------------"<<endl;
	for(std::list<FeedSourceStatusModel*>::iterator list_iter = aAvailableFeedProviders.begin(); list_iter != aAvailableFeedProviders.end(); list_iter++)
	{
		FeedSourceStatusModel* lFeedSourceStatusModel=(FeedSourceStatusModel*)*list_iter;
		cout << "FeedProvider Info : AutoID : "<<lFeedSourceStatusModel->getAutoIncrimentID()<< " Feed Provider ID : "<<lFeedSourceStatusModel->getFeedProviderID()<<" Provider Name : "<<lFeedSourceStatusModel->getFeedProviderName()<<" Status : "<<lFeedSourceStatusModel->isEnabled()<<endl;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	FXTFLog mLogWriter;
	// Main Function Argument for Passing Data in Different Thread..
	MainThreadArgument* mMainThreadArgument = new MainThreadArgument(); 
	
	if (mLogWriter.CommonLogInit()){
		mLogWriter.Enable();
		mLogWriter.GetTimeStr();
		mLogWriter.LogOut("");};

	mMainThreadArgument->setLogWriter(&mLogWriter);

	mMainThreadArgument->getLogWriter()->LogOutLn("**********************  Feed Server Starting ********************");

	DBOperationController *lDBOperationController=new DBOperationController(mMainThreadArgument);
	SettingsController *lSettingsController=new SettingsController(mMainThreadArgument);

	list<FeedSourceStatusModel*> lListAvailableFeedProvider=lSettingsController->getAvailableFeedProviders();

	DisplayFeedProviderInfo(lListAvailableFeedProvider,&mLogWriter);

	list<SymbolValues*> lListData=lSettingsController->getAvailableSymbol();

	DisplaySymbolSettingsInfo(lListData,&mLogWriter);

	mMainThreadArgument->getLogWriter()->LogOutLn("**********************  Settings Loaded Successfully ********************");
	ClientController* lClientController = new ClientController(mMainThreadArgument);
	lClientController->initializeServerForClient();
	MT4Controller* lMT4Controller = new MT4Controller(mMainThreadArgument);
	lMT4Controller->initializeServerForClient();

	FilterProcessController* lFilterProcessController=new FilterProcessController(mMainThreadArgument);
	
	FixHandleController* lFixHandleController=new FixHandleController(mMainThreadArgument);
	
	
	//Sleep(10000);

	//while (true)
	//{
		//lClientController->sendFeedMessageToAllClient("USDJPY 0 120.123 120.143");
	//	mMainThreadArgument->getFilterProcessController()->FeedDataFromFXCM("FXCM USDJPY 0 120.123 120.143");
	//	Sleep(10);
	//}

	while (true)
	{

	}
	return 0;
}

