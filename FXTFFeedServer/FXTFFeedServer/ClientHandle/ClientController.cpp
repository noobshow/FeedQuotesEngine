#include "../stdafx.h"
#include "ClientController.h"
#include "../Common/SocketLog.h"
#include "../Common/FXTFLog.h"

#define WIN32_LEAN_AND_MEAN

SocketLog winLog;
ClientController::ClientController(MainThreadArgument* aMainThreadArgument )
{
	mMainThreadArgument=aMainThreadArgument;
	aMainThreadArgument->setClientController(this);
	mCurrentNumberOfClients=0;
	mServerClientArgument = NULL;
}


ClientController::~ClientController(void)
{
	delete mServerClientArgument;
	delete mClientTCPServer;
}

bool ClientController::initializeServerForClient(void)
{
	// Initialize the winsock library
	ClientTcpSocket::initialize();

	ClientHostInfo lServerInfo;
	string lServerName = lServerInfo.getHostName();
    string lServerIPAddress = lServerInfo.getHostIPAddress();

	cout << "my localhost (server) information:" << endl;
	cout << "	Name:    " << lServerName << endl;
    cout << "	Address: " << lServerIPAddress << endl;
	
	mClientTCPServer=new ClientTcpSocket(PORTNUM_CLIENT);
	mClientTCPServer->setLingerSeconds(0);
	cout   << mClientTCPServer;

	mServerClientArgument = new ClientThreadArgument(mClientTCPServer,lServerName,this);
	ClientThread* lServerClientThread = new ClientThread(ClientController::ClientServerHandleThread,(void*)mServerClientArgument);
	lServerClientThread->execute();

	//Another Thread For Handeling removed Client

	//ClientThread* lServerClientThreadFroRemoveOption = new ClientThread(ClientController::ClientServerHandleThreadForremovedClient,(void*)mServerClientArgument);
	//lServerClientThreadFroRemoveOption->execute();

	return true;
}



void ClientController::sendFeedMessageToAllClient(string aFeedMessage)
{
	//cout <<getCurrentTime()<<" => "<<aFeedMessage<<endl;

	if (mServerClientArgument!=NULL)
	{
		for (int i = 0; i < MAX_NUM_CLIENTS; i++)
		{
			ClientThreadArgument* lClientInfo=mServerClientArgument->getClientArgument(i);
			
			if (lClientInfo)
			{
				//Need To handle in more case
				if (!lClientInfo->getExitEvent()->waitForEvent(0))
				{
					int lSendNumBytes=lClientInfo->getClientConnect()->sendMessage(aFeedMessage,this);
					if (lSendNumBytes==-99)
					{
						removeMessageSendFailedClientFromList(lClientInfo);
					}
				}
				
			}
		}
	}
	
}

void ClientController::sendMessageToSpecificClient(string Message)
{
	// Need to Develop for Specific Client Operation
}


DWORD WINAPI ClientController::ClientServerHandleThread(LPVOID aThreadInfo)
{
	// this structure will contain all the data this callback will work on
    ClientThreadArgument* lServerClientArgument = (ClientThreadArgument*)aThreadInfo;

	// get the server
	ClientTcpSocket* lServerForClient = lServerClientArgument->getClientConnect();
	string lServerName = lServerClientArgument->getHostName();
	// bind the server to the socket
	lServerForClient->bindSocket();
	ClientController *lThisClass=lServerClientArgument->getThisClassController();
	


	cout   << endl << "server finishes binding process... " << endl;
	lThisClass->getMainThreadArgument()->getLogWriter()->LogOutLn(" Info :: Server Started and Binding Properly For Client ");
	
	// server starts to wait for client calls
	lServerForClient->listenToClient();

	ClientThreadArgument* lClientArgument[MAX_NUM_CLIENTS];
	ClientThread* lClientHandle[MAX_NUM_CLIENTS];
	for ( int i = 0; i < MAX_NUM_CLIENTS; i++ )
	{
		lClientArgument[i] = NULL;
		lClientHandle[i] = NULL;
	}

	//Infinite loop
	//Ready to Accept Client Connection Request 
	while (true)
	{
		// wait to accept a client connection.  
		// processing is suspended until the client connects
    	ClientTcpSocket* lClientTCPSocket;		// connection dedicated for client communication
		string lClientName;				// client name 
		lClientTCPSocket = lServerForClient->acceptClient(lClientName);	
		lClientTCPSocket->setLingerSeconds(0);
		lClientName = lClientName + "-" +std::to_string(lThisClass->getCurrentNumberOfClients());
		
		cout << "Connection Request Recieved From(Client) : "<<lClientName<<endl;
		
		lThisClass->getMainThreadArgument()->getLogWriter()->LogOutLn("Connection Request Recieved(Client) From : "+lClientName);

		if (lThisClass->getCurrentNumberOfClients()<MAX_NUM_CLIENTS-1)
		{
			lClientArgument[lThisClass->getCurrentNumberOfClients()] = new ClientThreadArgument(lClientTCPSocket,lClientName,lThisClass);
			lClientHandle[lThisClass->getCurrentNumberOfClients()] = new ClientThread(ClientController::clientHandleThread,(void*)lClientArgument[lThisClass->getCurrentNumberOfClients()]);
			lServerClientArgument->addClientArgument(lClientArgument[lThisClass->getCurrentNumberOfClients()]);
			lClientHandle[lThisClass->getCurrentNumberOfClients()]->execute();
			lThisClass->increaseCurrentNumberOfClients();
		}
	}
	return 1;
}

DWORD WINAPI ClientController::ClientServerHandleThreadForremovedClient(LPVOID aThreadInfo)
{
	ClientThreadArgument* lServerClientArgument = (ClientThreadArgument*)aThreadInfo;
	ClientController *lThisClass=lServerClientArgument->getThisClassController();
	while (true)
	{
		lThisClass->removeDisconnectedClientFromList();
	}
	return 1;
}

void ClientController::removeDisconnectedClientFromList()
{
	for ( int i = 0; i < MAX_NUM_CLIENTS; i ++ )
		{
			ClientThreadArgument* lClientInfo = mServerClientArgument->getClientArgument(i);
			if ( lClientInfo && lClientInfo->getExitEvent()->waitForEvent(0) )
			{
				lClientInfo->setSignalToEnd(true);
				cout   << " Removed >> " << lClientInfo->getHostName() << endl;
				mMainThreadArgument->getLogWriter()->LogOutLn("Removed1 Client : "+lClientInfo->getHostName());
				mServerClientArgument->removeClientArgument(lClientInfo,i);
				decreaseCurrentNumberOfClients();
				break;
			}
		}
}

void ClientController::removeMessageSendFailedClientFromList(ClientThreadArgument* aClientInfo)
{
	for ( int i = 0; i < MAX_NUM_CLIENTS; i ++ )
	{
		ClientThreadArgument* lClientInfo = mServerClientArgument->getClientArgument(i);
		if (lClientInfo!=NULL && aClientInfo!=NULL)
		{
			if (!lClientInfo->getSignalToEnd())
			{
				if ( strcmp(lClientInfo->getHostName().c_str(),aClientInfo->getHostName().c_str())==0)
				{
					lClientInfo->setSignalToEnd(true);
					cout   << " Removed >> " << lClientInfo->getHostName() << endl;
					mMainThreadArgument->getLogWriter()->LogOutLn("Removed2 Client : "+lClientInfo->getHostName());
					mServerClientArgument->removeClientArgument(lClientInfo,i);
					decreaseCurrentNumberOfClients();
					break;
				}
			}
		}
	}
}

DWORD WINAPI ClientController::clientHandleThread(LPVOID aThreadInfo)
{
	// this structure will contain all the data this callback will work on
    ClientThreadArgument* lClientArgument = (ClientThreadArgument*)aThreadInfo;
	
	//Get This Class Controller
	ClientController *lThisClass=lClientArgument->getThisClassController();

	// get the client connection: receiving messages from client and
	// sending messages to the client will all be done by using
	// this client connection
	ClientTcpSocket* lClientConnection = lClientArgument->getClientConnect();
	string lClientName = lClientArgument->getHostName();
	lThisClass->getMainThreadArgument()->getLogWriter()->LogOutLn("Thread Started Properly For : "+lClientName);

	// Get Client response 

	while (true)
	{
		string lMessageFromSpecificClient = "";

		// receive from the client
		int numBytes = lClientConnection->recieveMessage(lMessageFromSpecificClient);
		if ( numBytes == -99 ) break;
		
		// Message Format for Client Message Handle

		string lFormattedClientMessage =  lClientName+" "+lMessageFromSpecificClient+"\n";

		cout   << lFormattedClientMessage<<endl;
		lThisClass->acceptClientMessage(lFormattedClientMessage);
	}
	
	return 1;
}


void ClientController::receivedClientResponse(string aClientResponseMessage)
{
	// Handle This Message for Configuretion Setting and else
	cout   <<" Message Recived "<< aClientResponseMessage;
}


int ClientController::getCurrentNumberOfClients()
{
	return mCurrentNumberOfClients;
}

bool ClientController::increaseCurrentNumberOfClients()
{
	mCurrentNumberOfClients++;
	return true;
}

bool ClientController::decreaseCurrentNumberOfClients()
{
	mCurrentNumberOfClients--;

	if (mCurrentNumberOfClients<0)
	{
		return false;
	}
	return true;
}

void ClientController::setCurrentNumberOfClients(int aCount)
{
	mCurrentNumberOfClients=aCount;
}

string  ClientController::getCurrentTime()
{
	// setup time
	time_t lSttime;
	time(&lSttime);
	
	// convert to gm time
	struct tm * tim = gmtime(&lSttime);
	
	// set data items
	int sec  = tim->tm_sec;           // second (0-61, allows for leap seconds)
	int min  = tim->tm_min;           // minute (0-59)
	int hour = tim->tm_hour;          // hour (0-23)

//      int mon  = tim->tm_mon + 1;       // month (0-11)
//      int mday = tim->tm_mday;          // day of the month (1-31)
//      int year = tim->tm_year % 100;    // years since 1900

      char cur_time[9];
//      char cur_date[9];

      sprintf(cur_time,"%02d:%02d:%02d",hour,min,sec);

	  return cur_time;
}


// Client Request Handeling
void ClientController::acceptClientMessage(string aInputMessage)
{
	mMainThreadArgument->getLogWriter()->LogOutLn(aInputMessage);
	ClientRequestModel *lClientRequestModel=new ClientRequestModel();
	if(lClientRequestModel->setInputMessage(aInputMessage))
	{
		if (lClientRequestModel->getCommandType().compare(CLIENT_COMMAND_TYPE_SPIKE)==0)
		{
			if(spikeOperatio(lClientRequestModel))
			{
				//May Send Updated Info to Client
				cout  << "Spike Updated To Server"<<endl;
				mMainThreadArgument->getLogWriter()->LogOutLn("Client Message Not Formatted properly : ");
			}
		}
		if(lClientRequestModel->getCommandType().compare(CLIENT_COMMAND_TYPE_FEED_SWITCHER)==0)
		{
			if(feedSwitcher())
			{
				cout  << "Feed Switcher Updated To Server"<<endl;
				mMainThreadArgument->getLogWriter()->LogOutLn("Feed Switcher Update request came from server.");
			}

		}
		else if (true)
		{
			cout  << "Other Operation : "<<endl;
		}

	}else
	{
		cout  << "Client Message Not Formatted properly : "<<endl;
		mMainThreadArgument->getLogWriter()->LogOutLn("Client Message Not Formatted properly : ");
	}
}

bool ClientController::spikeOperatio(ClientRequestModel *aClientRequestModel)
{
	mMainThreadArgument->getSettingsController()->updateSymbolSpikeMappingInfo();
	mMainThreadArgument->getLogWriter()->LogOutLn("SPIKE Info Updated Successfully For Mode ********* ********");
	return true;
}

bool ClientController::feedSwitcher()
{
	mMainThreadArgument->getSettingsController()->updateSymbolSourceMappingModel();
	return true;
}