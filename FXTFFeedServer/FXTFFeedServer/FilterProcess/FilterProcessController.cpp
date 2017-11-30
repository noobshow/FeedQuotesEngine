#include "../stdafx.h"
#include "FilterProcessController.h"

const int FEED_PROVIDER_STRING_SIZE = 12;
const int FEED_MESSAGE_STRING_SIZE = 128;
FilterProcessController::FilterProcessController(MainThreadArgument* aMainThreadArgument )
{
	mMainThreadArgument=aMainThreadArgument;
	mIsSendingToMT4=true;
	mIsSendingToClient=true;
	aMainThreadArgument->setFilterProcessController(this);
	createSendingThread();

}


FilterProcessController::~FilterProcessController(void)
{

}

void FilterProcessController::LoadSetting()
{
	// Load Setting From Database
}

void FilterProcessController::RequestToLoadSettingFromAdminClient(int aRequestCondition)
{
	// Load Setting From Database with Condition
}

bool FilterProcessController::CheckSettingForFXCM( )
{
	//Check Settings
	return true;
}

bool FilterProcessController::CheckSettingForOCBC( )
{
	//Check Settings
	return true;
}

bool FilterProcessController::CheckSettingForPQE( )
{
	//Check Settings
	return false;
}


//Message From Feed Provider Expcting as below
// "<FeedProvider> <Symbol> 0 <BID> <ASK>"
void FilterProcessController::FeedDataFromFXCM(SymbolSourceModel* aSymbolSourceModel)
{
	if (true)
	{
		sendToClientUnfilterData(aSymbolSourceModel);
	}
	mutex lCriticalSectionControlForFXCM;
	bool lIsOkayToSend=false;
	lCriticalSectionControlForFXCM.lock();

		SymbolValues* lSymbolValue =getSymbolValueForFeedSymbol(aSymbolSourceModel);
	
		list<SymbolSourceMappingModel*> lTempSourceModels=lSymbolValue->getSymbolSourceModels();

		for(std::list<SymbolSourceMappingModel*>::iterator list_iter = lTempSourceModels.begin(); list_iter != lTempSourceModels.end(); list_iter++)
		{
			SymbolSourceMappingModel* lSymbolSourceMappingModel=(SymbolSourceMappingModel*)*list_iter;
			if (lSymbolSourceMappingModel->getFeedProviderID()==aSymbolSourceModel->getFeedProviderID())
			{
				lIsOkayToSend=true;
			}
		}
	lCriticalSectionControlForFXCM.unlock();

	if (lIsOkayToSend)
	{
		FilterFeedData(lSymbolValue,aSymbolSourceModel);
	}
}

//Message From Feed Provider Expcting as below
// "<FeedProvider> <Symbol> 0 <BID> <ASK>"
void FilterProcessController::FeedDataFromFXCMUS(SymbolSourceModel* aSymbolSourceModel)
{
	cout<<"Symbol : "<<aSymbolSourceModel->getFeedSymbol()<< " B : "<<aSymbolSourceModel->getBidValue()<<" A : "<<aSymbolSourceModel->getAskValue()<<endl;
	if (true)
	{
		sendToClientUnfilterData(aSymbolSourceModel);
	}
	mutex lCriticalSectionControlForFXCM;
	bool lIsOkayToSend=false;
	lCriticalSectionControlForFXCM.lock();

		SymbolValues* lSymbolValue =getSymbolValueForFeedSymbol(aSymbolSourceModel);
	
		list<SymbolSourceMappingModel*> lTempSourceModels=lSymbolValue->getSymbolSourceModels();

		for(std::list<SymbolSourceMappingModel*>::iterator list_iter = lTempSourceModels.begin(); list_iter != lTempSourceModels.end(); list_iter++)
		{
			SymbolSourceMappingModel* lSymbolSourceMappingModel=(SymbolSourceMappingModel*)*list_iter;
			if (lSymbolSourceMappingModel->getFeedProviderID()==aSymbolSourceModel->getFeedProviderID())
			{
				lIsOkayToSend=true;
			}
		}
	lCriticalSectionControlForFXCM.unlock();

	if (lIsOkayToSend)
	{
		FilterFeedData(lSymbolValue,aSymbolSourceModel);
	}
}


void FilterProcessController::FeedDataFromOCBC(SymbolSourceModel* aSymbolSourceModel)
{
	if (true)
	{
		sendToClientUnfilterData(aSymbolSourceModel);
	}
	mutex lCriticalSectionControlForOCBC;
	bool lIsOkayToSend=false;
	lCriticalSectionControlForOCBC.lock();
		SymbolValues* lSymbolValue =getSymbolValueForFeedSymbol(aSymbolSourceModel);

		list<SymbolSourceMappingModel*> lTempSourceModels=lSymbolValue->getSymbolSourceModels();

		for(std::list<SymbolSourceMappingModel*>::iterator list_iter = lTempSourceModels.begin(); list_iter != lTempSourceModels.end(); list_iter++)
		{
			SymbolSourceMappingModel* lSymbolSourceMappingModel=(SymbolSourceMappingModel*)*list_iter;
			if (lSymbolSourceMappingModel->getFeedProviderID()==aSymbolSourceModel->getFeedProviderID())
			{
				lIsOkayToSend=true;
				//FilterFeedData(lSymbolValue,aSymbolSourceModel);
			}
		}
	lCriticalSectionControlForOCBC.unlock();
	if (lIsOkayToSend)
	{
		FilterFeedData(lSymbolValue,aSymbolSourceModel);
	}
	
}

void FilterProcessController::FeedDataFromPQE(SymbolSourceModel* aSymbolSourceModel)
{
	if (true)
	{
		sendToClientUnfilterData(aSymbolSourceModel);
	}
}

SymbolValues* FilterProcessController::getSymbolValueForFeedSymbol(SymbolSourceModel* aSymbolSourceModel)
{
	list<SymbolValues*> lAvailableSymbols=mMainThreadArgument->getSettingsController()->getAvailableSymbol();
	
	for(std::list<SymbolValues*>::iterator list_iter = lAvailableSymbols.begin(); list_iter != lAvailableSymbols.end(); list_iter++)
	{
		SymbolValues* lSymbolValue=(SymbolValues*)*list_iter;
		if (lSymbolValue->getSymbolName().compare(aSymbolSourceModel->getFeedSymbol())==0)
		{
			return lSymbolValue;
		}
	}
}

void FilterProcessController::updateAvailableValuesForTickSend(SymbolValues* aSymbolValues)
{
	list<SymbolValues*> lTempList=mMainThreadArgument->getSettingsController()->getAvailableSymbol();
	std::list<SymbolValues*>::iterator list_iter;

	for( list_iter= lTempList.begin(); list_iter != lTempList.end(); ++list_iter)
	{
		SymbolValues *lSymbolValues=(SymbolValues*)*list_iter;
		if (lSymbolValues->getAutoIncrimentID()==aSymbolValues->getAutoIncrimentID())
		{			
			*list_iter=aSymbolValues;
			break;
		}
	}
}


//bool tempCompleteCheck=true;
void FilterProcessController::FilterFeedData(SymbolValues *aSymbolValue,SymbolSourceModel* aSymbolSourceModel)
{
	mSpikeAndSendingThreadSafe.lock();
	string lTempSorceModelID =  to_string( aSymbolSourceModel->getFeedProviderID() );
		if (aSymbolValue->checkIsSpiked(mMainThreadArgument->getLogWriter(),aSymbolSourceModel))
		{
			//cout <<" **** Spiked : **** "<<endl;
		}else
		{
			//string lTempSorceModelID =  aSymbolSourceModel->getFeedProviderID() ;
			mMainThreadArgument->getLogWriter()->LogOutLn(" Source: "+ lTempSorceModelID + " BidAsk: "+aSymbolValue->valueOutforMT4());
			//mMT4Ticks.push_back(aSymbolValue->valueOutforMT4());
			//string aLogMessage = aSymbolValue->getFeedProviderName();
			SendDataToMT4Server(aSymbolValue->valueOutforMT4());
			updateAvailableValuesForTickSend(aSymbolValue);
		}
	mSpikeAndSendingThreadSafe.unlock();
}

void FilterProcessController::sendToClientUnfilterData(SymbolSourceModel* aSymbolSourceModel)
{
	string lFeedMessage=to_string(aSymbolSourceModel->getFeedProviderID())+" "+aSymbolSourceModel->getFeedSymbol()+" "+to_string(aSymbolSourceModel->getBidValue())+" "+to_string(aSymbolSourceModel->getAskValue());
	//SendDataToClientForMonitor(lFeedMessage);
	mClientSendingThreadLock.lock();
	mClientTicks.push_back(lFeedMessage);
	mClientSendingThreadLock.unlock();
}

string FilterProcessController::FilterEngine(string aFeedProvider,string aFeedMessage)
{
	// Need to Impliment Filter Logic here
	return aFeedMessage+"\n";
}

void FilterProcessController::SendDataToClientForMonitor(string aFeedMessage)
{
	if (mMainThreadArgument && mMainThreadArgument->getClientController() )
	{
		mMainThreadArgument->getClientController()->sendFeedMessageToAllClient(aFeedMessage);
	}
}

void FilterProcessController::SendDataToMT4Server(string aFeedMessage)
{
	//mMainThreadArgument->getLogWriter()->LogOutLn(aFeedMessage);
	//cout << aFeedMessage<<endl;
	if (mMainThreadArgument && mMainThreadArgument->getMT4Controller() )
	{
		//mMainThreadArgument->getLogWriter()->LogOutLn(aFeedMessage);
		mMainThreadArgument->getMT4Controller()->sendFeedMessageToAllClient(aFeedMessage);
	}
}

// May Need More Precaution here

void FilterProcessController::SplitProviderAndFeed(char* apFeedProvider,char* apFeedMessage,string aSorceString)
{
	char *lSourceString=(char*)aSorceString.c_str();
	//if((lSourceString=strstr((char*)aSorceString.c_str()," "))==NULL)     return;
	initializeCharArray(apFeedProvider,FEED_PROVIDER_STRING_SIZE);
	initializeCharArray(apFeedMessage,FEED_MESSAGE_STRING_SIZE);
	int length =aSorceString.length(); 
	bool lbMessageFound=false;
	int lCount1=0;
	int lCount2=0;
	for (int i = 0; i < length; i++)
	{
		if (!lbMessageFound)
		{
			apFeedProvider[lCount1]=lSourceString[i];
			lCount1++;
		}else
		{
			apFeedMessage[lCount2]=lSourceString[i];
			lCount2++;
		}
		if (lSourceString[i]==' ')
		{
			lbMessageFound=true;
		}
	}
}

void FilterProcessController::initializeCharArray(char* aCharArray,int aLength)
{
	for (int i = 0; i < aLength; i++)
	{
		aCharArray[i]='\0';
	}
}


void FilterProcessController::testMutexImplimentetion()
{
	HANDLE ghMutex; 
	HANDLE aThread[3];
    DWORD ThreadID;
	ghMutex = CreateMutex( 
        NULL,              // default security attributes
        FALSE,             // initially not owned
        NULL); 
	if (ghMutex == NULL) 
    {
        cout<<"CreateMutex error:"<<GetLastError();
        
    }
}

void FilterProcessController::createSendingThread()
{
	//SendingThreadHandler* lSendingThreadHandler = new SendingThreadHandler(FilterProcessController::sendingToMT4Thread,(void*)this);
	SendingThreadHandler* lSendingThreadHandler = new SendingThreadHandler(FilterProcessController::sendingToClientThread,(void*)this);
	lSendingThreadHandler->execute();
}

DWORD WINAPI FilterProcessController::sendingToMT4Thread(LPVOID aThreadData)
{
	 FilterProcessController* lFilterProcessController = (FilterProcessController*)aThreadData;

	
	 mutex lAccesshCriticalSection;
	 while (lFilterProcessController->IsSendingToMT4())
	 {
		 int lTempCount=0;
		 lAccesshCriticalSection.lock();
		 list<string> lAvailableTicks=lFilterProcessController->getAvailableTicksToSend();

		 int lSize=lAvailableTicks.size();
		 if(lSize>0)
		 {
			 for(std::list<string>::iterator list_iter = lAvailableTicks.begin(); list_iter != lAvailableTicks.end(); list_iter++)
			 {
				 string lTickString=(string)*list_iter;
				 lFilterProcessController->SendDataToMT4Server(lTickString);
				 //lFilterProcessController->mMainThreadArgument->getLogWriter()->LogOutLn(lTickString);
				 //lAvailableTicks.pop_front();
				 lFilterProcessController->RemoveItems(lTickString);
				 lTempCount++;
				 if(lTempCount>31)
				 {
					 lFilterProcessController->mMainThreadArgument->getLogWriter()->LogOutLn("Currently Excedding 31");
					 Sleep(50);
					 break;
				 }
			 }
			 //lFilterProcessController->ClearItems();
		 }

		 lAccesshCriticalSection.unlock();
		 //Sleep(100);
	 }
	 return 1;
}

DWORD WINAPI FilterProcessController::sendingToClientThread(LPVOID aThreadData)
{
	 FilterProcessController* lFilterProcessController = (FilterProcessController*)aThreadData;
	 mutex lAccesshCriticalSection;
	 while (lFilterProcessController->IsSendingToClient())
	 {
		 lFilterProcessController->mClientSendingThreadLock.lock();
		 vector<string> lAvailableTicks=lFilterProcessController->getAvailableTicksToSendClient();
		 int lSize=lFilterProcessController->getSizeClientTick();
		 if(lSize>0)
		 {
			 for (int i = 0; i <lSize; i++)
			 {
				  lFilterProcessController->SendDataToClientForMonitor(lAvailableTicks[i]);
				  /*
				  i=lFilterProcessController->RemoveItemsClient(i);
				  if (i!=lFilterProcessController->getBeginClient())
				  {
					  i=lFilterProcessController->setPrevClient(i);
						continue;
				  }else
				  {
					  break;
				  }
				  */
			 }
			 lFilterProcessController->clearClientTickItems();
		 }
			 /*

			 for(std::list<string>::iterator list_iter = lAvailableTicks.begin(); list_iter != lAvailableTicks.end(); list_iter++)
			 {
				 string lTickString=(string)*list_iter;
				 lFilterProcessController->SendDataToClientForMonitor(lTickString);
				 //lFilterProcessController->mMainThreadArgument->getLogWriter()->LogOutLn(lTickString);
				 //lAvailableTicks.pop_front();
				 lFilterProcessController->RemoveItems(lTickString);
				
			 }
			 //lFilterProcessController->ClearItems();
		 }
		 */
		lFilterProcessController->mClientSendingThreadLock.unlock();
		 Sleep(5);
	 }
	 return 1;
}