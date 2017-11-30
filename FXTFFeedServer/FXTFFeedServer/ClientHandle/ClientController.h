#pragma once
#define WIN32_LEAN_AND_MEAN

#include <time.h>

#include "../ClientHandle/ClientThreadEvent.h"
#include "../ClientHandle/ClientSocket.h"
//#include "myLog.h"
#include "../Common/ThreadException.h"
#include "../ClientHandle/ClientHostInfo.h"
#include "../ClientHandle/ClientThread.h"
#include "../ClientHandle/ClientThreadArgument.h"
#include "../Settings/SettingsController.h"
#include "../MainThreadArgument.h"
#include "../Models/ClientRequestModel.h"

#pragma comment(lib, "ws2_32")

class ClientController
{
private:
	//Members
	int			mCurrentNumberOfClients;
	ClientThreadArgument* mServerClientArgument;
	ClientTcpSocket* mClientTCPServer;
	MainThreadArgument* mMainThreadArgument;
public:
	//Members

private:
	//Functions
	string		getCurrentTime();
	bool		spikeOperatio(ClientRequestModel *aClientRequestModel);
	bool		feedSwitcher();

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
	void		removeMessageSendFailedClientFromList(ClientThreadArgument* aClientInfo);

	MainThreadArgument* getMainThreadArgument(){return mMainThreadArgument;};

	// Thread Handeler Functions
	static DWORD WINAPI ClientServerHandleThread(LPVOID aThreadInfo);
	static DWORD WINAPI ClientServerHandleThreadForremovedClient(LPVOID aThreadInfo);
	static DWORD WINAPI clientHandleThread(LPVOID aThreadInfo);

	void acceptClientMessage(string aInputMessage);

public:
	ClientController(MainThreadArgument* aMainThreadArgument);
	~ClientController(void);
};

