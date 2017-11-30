#include "FXCMUSFixController.h"
#include "../FilterProcess/FilterProcessController.h"

FXCMUSFixController::FXCMUSFixController(MainThreadArgument* aMainThreadArgument )
{
	mMainThreadArgument=aMainThreadArgument;
	mFXCMUSApplicationMessaging=NULL;
}


FXCMUSFixController::~FXCMUSFixController(void)
{
	mFXCMUSApplicationMessaging->EndFixSession();
	delete mFXCMUSApplicationMessaging;
}

void FXCMUSFixController::initializaFIXConnection( )
{
	mFXCMUSApplicationMessaging= new FXCMUSApplicationMessaging(this);
	deletePreviousLogFiles();
	mFXCMUSApplicationMessaging->StartFixSession();
}

void FXCMUSFixController::deletePreviousLogFiles()
{
	cout << "----Deleting previous Log Files..."<<endl;
	deletingFilesFromDirectory("FXCMUSStore");
	deletingFilesFromDirectory("FXCMUSLogs");
}

void FXCMUSFixController::fixLogOutOccured(const SessionID& aSession_ID)
{
	cout << "[FXCMUS]----LogOut Action Received... "<<endl;
	mMainThreadArgument->getLogWriter()->LogOutLn("[FXCMUS] ---------  Going for Re-Login  ---------");
	deletePreviousLogFiles();
	//mFXCMUSApplicationMessaging->reLoginAfterLogout(aSession_ID);
	/*

	cout << "[FXCM]----LogOut Action Received... after 30 second Force login will happend if Not Login automatically"<<endl;
	Sleep(30000);
	if (!mFXCMApplicationMessaging->mFIXLoginStatus)
	{
		mMainThreadArgument->getLogWriter()->LogOutLn("[FXCM] ---------  Going for Re-Login  ---------");
		cout << "[FXCM] Going for Re-Login"<<endl;
		try
		{
			mFXCMApplicationMessaging->EndFixSession();
			delete mFXCMApplicationMessaging;
			mFXCMApplicationMessaging=NULL;
			mFXCMApplicationMessaging= new FXCMApplicationMessaging(this);
			cout << "[FXCM] Newly Create Application Messaging"<<endl;
			
			cout << "[FXCM] Delete Prev Log Files"<<endl;
			mFXCMApplicationMessaging->StartFixSession();
		}catch(Exception aex)
		{
			cout << "[FXCM] Exception Occured While Re-Login :"<<aex.what()<<endl;
		}
	}else
	{
		mMainThreadArgument->getLogWriter()->LogOutLn("[FXCM] LogOut But FIX Relogin Occured");
	}
	*/
}

void FXCMUSFixController::deletingFilesFromDirectory(string aDirectoryPath)
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

void FXCMUSFixController::StartGettingMarketData( int aTradeSessionStatus )
{
	if (aTradeSessionStatus==2)
	{
		cout << "******************** FXCM US Market is Open , Now will subscribe for Market Data ********************"<<endl;
		if (mFXCMUSApplicationMessaging!=NULL)
		{
			mFXCMUSApplicationMessaging->SubscribeForMarketData(mMainThreadArgument->getSettingsController()->getAvailableSymbol());
		}
	}else
	{
		cout << "FXCM US Market is Closed !!!!!!!!!!!!!"<<endl;
	}
}

void FXCMUSFixController::StopGettingMarketData( )
{
}

void FXCMUSFixController::UpdatedMarketData(SymbolSourceModel* aSymbolSourceModel)
{
	mMainThreadArgument->getFilterProcessController()->FeedDataFromFXCMUS(aSymbolSourceModel);
}