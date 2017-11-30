
#include "OCBCFixController.h"
#include "../FilterProcess/FilterProcessController.h"

OCBCFixController::OCBCFixController(MainThreadArgument* aMainThreadArgument )
{
	mMainThreadArgument=aMainThreadArgument;
	mOCBCApplicationMessaging=NULL;
}


OCBCFixController::~OCBCFixController(void)
{
	mOCBCApplicationMessaging->EndFixSession();
	delete mOCBCApplicationMessaging;
}

void OCBCFixController::initializaFIXConnection( )
{
	mOCBCApplicationMessaging= new OCBCApplicationMessaging(this);
	deletePreviousLogFiles();
	mOCBCApplicationMessaging->StartFixSession();
}

void OCBCFixController::deletePreviousLogFiles()
{
	cout << "----Deleting previous Log Files..."<<endl;
	deletingFilesFromDirectory("OCBCStore");
	deletingFilesFromDirectory("OCBCLogs");
}

void OCBCFixController::fixLogOutOccured(const SessionID& aSession_ID)
{
	cout << "[OCBC]----LogOut Action Received... "<<endl;
	mMainThreadArgument->getLogWriter()->LogOutLn("[OCBC] ---------  Going for Re-Login  ---------");
	deletePreviousLogFiles();
	mOCBCApplicationMessaging->reLoginAfterLogout(aSession_ID);

	/*
	cout << "[OCBC] LogOut Action Received... after 30 second Force login will happend if Not Login automatically "<<endl;
	Sleep(30000);
	if (!mOCBCApplicationMessaging->mFIXLoginStatus)
	{
		try
		{
			mMainThreadArgument->getLogWriter()->LogOutLn("[OCBC] ---------  Going for Re-Login  ---------");
			cout << "[OCBC] ---------Going for Re-Login ---------"<<endl;
			mOCBCApplicationMessaging->EndFixSession();
			delete mOCBCApplicationMessaging;
			mOCBCApplicationMessaging=NULL;
			mOCBCApplicationMessaging= new OCBCApplicationMessaging(this);
			cout << "[OCBC] Newly Create Application Messaging"<<endl;
			deletePreviousLogFiles();
			cout << "[OCBC] Delete Prev Log Files"<<endl;
			mOCBCApplicationMessaging->StartFixSession();
		}catch(Exception aEx)
		{
			cout << "[OCBC] Exception Occured While Re-Login :"<<aEx.what()<<endl;
		
		}
	}else
	{
		mMainThreadArgument->getLogWriter()->LogOutLn("[OCBC] LogOut But FIX Relogin Occured");
	}
	*/
}

void OCBCFixController::deletingFilesFromDirectory(string aDirectoryPath)
{
	HANDLE dir;
    WIN32_FIND_DATA file_data;

	char buffer[MAX_PATH];
    GetModuleFileName( NULL, buffer, MAX_PATH );
	string::size_type pos = string( buffer ).find_last_of( "\\/" );
    string ltemp=string( buffer ).substr( 0, pos)+"\\"+aDirectoryPath;

    if ((dir = FindFirstFile((ltemp + "/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
        return; /* No files found */

    do {
        const string file_name = file_data.cFileName;
        const string full_file_name = ltemp + "/" + file_name;
        const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

        if (file_name[0] == '.')
            continue;

        if (is_directory)
            continue;

		remove(full_file_name.c_str());
    } while (FindNextFile(dir, &file_data));

    FindClose(dir);
}

void OCBCFixController::StartGettingMarketData( int aTradeSessionStatus)
{
	if (aTradeSessionStatus==2)
	{
		cout << "******************** OCBC Market is Open , Now will subscribe for Market Data ********************"<<endl;
		if (mOCBCApplicationMessaging!=NULL)
		{
			mOCBCApplicationMessaging->SubscribeForMarketData(mMainThreadArgument->getSettingsController()->getAvailableSymbol());
		}
	}else
	{
		cout << "OCBC Market is Closed !!!!!!!!!!!!!"<<endl;
	}
}

void OCBCFixController::StopGettingMarketData( )
{
}

void OCBCFixController::UpdatedMarketData(SymbolSourceModel* aSymbolSourceModel)
{
	mMainThreadArgument->getFilterProcessController()->FeedDataFromOCBC(aSymbolSourceModel);
}