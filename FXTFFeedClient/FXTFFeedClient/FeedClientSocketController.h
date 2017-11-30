#ifndef FeedClientSocketController_H
#define FeedClientSocketController_H

#include "FeedClientSocketThread.h"
#include "FeedMonitorInfo.h"
#include "FeedMonitorInfoView.h"
#include "FeedClientSocketHandle\FeedClientException.h"
#include "FeedClientSocketHandle\FeedClientHostInfo.h"
#include "FeedClientSocketHandle\FeedClientSocket.h"
#include "FeedClientCommon\FeedClientConstant.h"
#include "FeedClientCommon\SingletonFeedClient.h"
#include "Common\FXTFLog.h"
#include "Common\Constant.h"
#include "DBHandle\ConfigFileRead.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <string.h>

class FeedClientSocketController
{
private:
	FeedClientTcpSocket	*mFeedClientTCPSocket;

public:
	string	mServerIPAddress;

public:
	void ReceiveQuotesFromServer();
	bool initializeSocketForClient();
	void receivedResponseFromServer(string aClientResponseMessage);
	int	sendSpikeSettingsToServer(string aFeedMessage);
	int sendFeedSwitcherSettingsToServer(int aSymbolID, string aUserName);

	static DWORD WINAPI FeedClientSocketHandleThread(LPVOID aThreadInfo);

	FeedClientTcpSocket* getMyTcpSocket(){ return mFeedClientTCPSocket;}
	void setMyTcpSocket(FeedClientTcpSocket* aFeedClientTCPSocket){mFeedClientTCPSocket=aFeedClientTCPSocket;}

	FeedMonitorInfoView *getFeedMonitorInfoView();

public:
	FeedClientSocketController();
	~FeedClientSocketController();
};

#endif