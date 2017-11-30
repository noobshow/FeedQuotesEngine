#pragma once
#define WIN32_LEAN_AND_MEAN

#include <time.h>

#include "../MT4Handle/MT4ThreadEvent.h"
#include "../MT4Handle/MT4Socket.h"
//#include "myLog.h"
#include "../Common/ThreadException.h"
#include "../MT4Handle/MT4HostInfo.h"
#include "../MT4Handle/MT4Thread.h"
#include "../MT4Handle/MT4ThreadArgument.h"
#include "../MainThreadArgument.h"

#pragma comment(lib, "ws2_32")

class MT4Controller
{
private:
	//Members
	int			mCurrentNumberOfClients;
	MT4ThreadArgument* mServerMT4Argument;
	MT4TcpSocket* mMT4TCPServer;
	MainThreadArgument* mMainThreadArgument;
public:
	//Members

private:
	//Functions
	string		getCurrentTime();
	

public:
	//Functions
	bool		initializeServerForClient();
	int			getCurrentNumberOfClients();
	bool		increaseCurrentNumberOfClients();
	bool		decreaseCurrentNumberOfClients();
	void		setCurrentNumberOfClients(int aCount);

	void		receivedClientResponse(string aClientResponseMessage);
	void		sendFeedMessageToAllClient(string aFeedMessage);
	void		sendMessageToSpecificClient(string aMessage);

	//
	void		removeDisconnectedClientFromList();
	void		removeMessageSendFailedClientFromList(MT4ThreadArgument* aClientInfo);

	MainThreadArgument* getMainThreadArgument(){return mMainThreadArgument;};

	// Thread Handeler Functions
	static DWORD WINAPI MT4ServerHandleThread(LPVOID aThreadInfo);
	static DWORD WINAPI MT4ServerHandleThreadForremovedClient(LPVOID aThreadInfo);
	static DWORD WINAPI MT4HandleThread(LPVOID aThreadInfo);

	bool ConfirmLogin(MT4TcpSocket* aClientTCPSocket);
	bool GetRequestedSymbols(MT4TcpSocket* aClientTCPSocket);

public:
	MT4Controller(MainThreadArgument* aMainThreadArgument);
	~MT4Controller(void);
};

