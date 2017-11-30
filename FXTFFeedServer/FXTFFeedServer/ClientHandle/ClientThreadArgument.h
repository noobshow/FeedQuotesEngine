#if !defined ClientThreadArgument_H
#define ClientThreadArgument_H

#include "../stdafx.h"
#include "../Common/Constant.h"
#include <string>
//#include "ClientController.h"
using namespace std;

//class mySemaphore;
class ClientTcpSocket;
class ClientThreadEvent;
class ClientController;

class ClientThreadArgument
{

private:

	// you have to change here to fit your needs

	//mySemaphore* coutSemaphore;       // semaphore for accessing the log file
	ClientTcpSocket* clientConnection;    // the connecting socket to the client
	string hostName;                // the name of the client
	ClientThreadArgument* mClientThreadArgument[MAX_NUM_CLIENTS];
    int numOfConnectedClients;

	// but the exitEvent is always needed
	ClientThreadEvent* exitEvent;
	bool signalToEnd;

	//This Class Controller
	ClientController *mClientController;
private:
	//Fubctions
	ClientThreadArgument(ClientTcpSocket* client,string& cName);

public:

	// you have to change here to fit your needs
	//ClientThreadArgument(ClientTcpSocket* client,mySemaphore* coutSem,string& cName);
	
	ClientThreadArgument::ClientThreadArgument(ClientTcpSocket* client,string& cName,ClientController * aClientController);
	~ClientThreadArgument() ;

	// use the accessor functions that fit your needs

	//mySemaphore* getCoutSemaphore()     { return coutSemaphore; }
	ClientTcpSocket* getClientConnect()     { return clientConnection; }
	string&      getHostName()          { return hostName; }
	bool getSignalToEnd()               { return signalToEnd; }
	void setSignalToEnd(bool s)         { signalToEnd = s; }

	void addClientArgument(ClientThreadArgument*);
	void removeClientArgument(ClientThreadArgument* argument,int aIndex);
	void shiftClientsForRemoved(int aDeletedIndex);
	ClientThreadArgument* getClientArgument(int);
	
	//Handle For Cotroller class Members
	ClientController * getThisClassController();


	// do not change here
    ClientThreadEvent* getExitEvent() { return exitEvent;}
};

#endif