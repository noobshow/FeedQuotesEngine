#include "FXCMFixController.h"
#include "../FilterProcess/FilterProcessController.h"

FXCMFixController::FXCMFixController(MainThreadArgument* aMainThreadArgument )
{
	mMainThreadArgument=aMainThreadArgument;
	mFXCMApplicationMessaging=NULL;
}


FXCMFixController::~FXCMFixController(void)
{
	mFXCMApplicationMessaging->EndFixSession();
	delete mFXCMApplicationMessaging;
}

void FXCMFixController::initializaFIXConnection( )
{
	mFXCMApplicationMessaging= new FXCMApplicationMessaging(this);
	deletePreviousLogFiles();
	mFXCMApplicationMessaging->StartFixSession();
}

void FXCMFixController::deletePreviousLogFiles()
{
	cout << "----Deleting previous Log Files..."<<endl;
	deletingFilesFromDirectory("FXCMStore");
	deletingFilesFromDirectory("FXCMLogs");
}

void FXCMFixController::fixLogOutOccured(const SessionID& aSession_ID)
{
	cout << "[FXCM]----LogOut Action Received... "<<endl;
	mMainThreadArgument->getLogWriter()->LogOutLn("[FXCM] ---------  Going for Re-Login  ---------");
	deletePreviousLogFiles();
	mFXCMApplicationMessaging->reLoginAfterLogout(aSession_ID);
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

void FXCMFixController::deletingFilesFromDirectory(string aDirectoryPath)
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

void FXCMFixController::StartGettingMarketData( int aTradeSessionStatus )
{
	if (aTradeSessionStatus==2)
	{
		cout << "******************** FXCM Market is Open , Now will subscribe for Market Data ********************"<<endl;
		if (mFXCMApplicationMessaging!=NULL)
		{
			mFXCMApplicationMessaging->SubscribeForMarketData(mMainThreadArgument->getSettingsController()->getAvailableSymbol());
		}
	}else
	{
		cout << "FXCM Market is Closed !!!!!!!!!!!!!"<<endl;
	}
}

void FXCMFixController::StopGettingMarketData( )
{
}

void FXCMFixController::UpdatedMarketData(SymbolSourceModel* aSymbolSourceModel)
{
	mMainThreadArgument->getFilterProcessController()->FeedDataFromFXCM(aSymbolSourceModel);
}