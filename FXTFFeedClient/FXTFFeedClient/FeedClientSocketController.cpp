#include "stdafx.h"
#include "FeedClientSocketController.h"

FeedClientSocketController::FeedClientSocketController()
{
	ConfigFileRead lConfigFileRead(INI_CONFIG_FILE_NAME);
	mServerIPAddress = lConfigFileRead.Get(INI_FEEDSERVER_CONFIG_NAME,INI_FEED_SERVER_IP_ADDRESS,"");
	initializeSocketForClient();
}
FeedClientSocketController::~FeedClientSocketController()
{
}
bool FeedClientSocketController::initializeSocketForClient(void)
{
	// Initialize the winsock library
	FeedClientTcpSocket::initialize();
	mFeedClientTCPSocket = new FeedClientTcpSocket(PORTNUM_CLIENT);
	mFeedClientTCPSocket->connectToServer(mServerIPAddress,ADDRESSClient);

	FeedClientSocketThread* lFeedClientSocketThread = new FeedClientSocketThread(FeedClientSocketController::FeedClientSocketHandleThread,this);
	lFeedClientSocketThread->execute();

	return true;
}

DWORD WINAPI FeedClientSocketController::FeedClientSocketHandleThread(LPVOID aThreadInfo)
{
	FeedClientSocketController *lFeedClientSocketController=(FeedClientSocketController*)aThreadInfo;
	//Get server's information (assume neither the name nor the address is given)
	FeedClientHostInfo serverInfo(lFeedClientSocketController->mServerIPAddress,ADDRESSClient);
	string serverName = serverInfo.getHostName();
	SingletonFeedClient::GetInstance()->getFXTFLog()->LogOutLn("Server Name: "+ serverName);
	SingletonFeedClient::GetInstance()->getFXTFLog()->LogOutLn("Server IP: "+ lFeedClientSocketController->mServerIPAddress);

	//Create the socket for client
    FeedClientTcpSocket *lFeedClientTCPSocket= lFeedClientSocketController->getMyTcpSocket();
	lFeedClientSocketController->getFeedMonitorInfoView();
	int recvBytes = 0;
	while (1)
	{
		string lQuotesMessageFromServer = "";
        recvBytes = lFeedClientTCPSocket->recieveMessage(lQuotesMessageFromServer);
		if ( recvBytes == -99 ) break;
		FXTFLog *lFXTFLog = SingletonFeedClient::GetInstance()->getFXTFLog();
		lFXTFLog->LogOutLn(lQuotesMessageFromServer);
		if(lFeedClientSocketController->getFeedMonitorInfoView()!=NULL)
		{
			lFeedClientSocketController->getFeedMonitorInfoView()->receiveQuotesMessage(lQuotesMessageFromServer);
		}
	}
	
	return 1;
}

FeedMonitorInfoView * FeedClientSocketController::getFeedMonitorInfoView()
{
	if (SingletonFeedClient::GetInstance()->getFeedMonitorInfo())
	{
		return SingletonFeedClient::GetInstance()->getFeedMonitorInfo()->getFeedMonitorInfoView();
	}
	return NULL;
}

int FeedClientSocketController::sendSpikeSettingsToServer(string aSpikeSettingMessage)
{
	//Send information to Server
	string lRandomNumber		= "123";
	string lUserID				= "123";
	string lSpikeSettingsType	= CLIENT_COMMAND_TYPE_SPIKE;
	string lActionID			= aSpikeSettingMessage;
	string lValue				= "123";

	int biteServer;
	aSpikeSettingMessage = lRandomNumber+" "+ lUserID + " "+ lSpikeSettingsType +" "+ lActionID +" "+ lValue;
	SingletonFeedClient::GetInstance()->getFXTFLog()->LogOutLn("Before: Spike MSG has been sent to server");

	if(mFeedClientTCPSocket)
	{
		biteServer = mFeedClientTCPSocket->sendMessage(aSpikeSettingMessage);
	}
	SingletonFeedClient::GetInstance()->getFXTFLog()->LogOutLn("Spike MSG Send to Server:" + aSpikeSettingMessage);
	SingletonFeedClient::GetInstance()->getFXTFLog()->LogOutLn("After: Spike MSG has been sent to server");

	return biteServer;
}

int FeedClientSocketController::sendFeedSwitcherSettingsToServer(int aSymbolID, string aUserName)
{
	//string lRandomNumber		= "123";
	//string lUserID				= to_string(aSymbolID);
	//string lSpikeSettingsType	= CLIENT_COMMAND_TYPE_SPIKE;
	//string lSpikeSettingsType	= "FeedSwitcher";
	//string lActionID			= sprintf_s(tempTextMessage,"%d",aSymbolID);;
	//string lValue				= "123";

	int biteServer;
	string aFeedSwitcherSettingMessage = "456 "+aUserName+" "+CLIENT_COMMAND_TYPE_FEED_SWITCHER+" "+to_string(aSymbolID);
	//SingletonFeedClient::GetInstance()->getFXTFLog()->LogOutLn("Before: Spike MSG has been sent to server");

	if(mFeedClientTCPSocket)
	{
		biteServer = mFeedClientTCPSocket->sendMessage(aFeedSwitcherSettingMessage);
	}

	return biteServer;
}