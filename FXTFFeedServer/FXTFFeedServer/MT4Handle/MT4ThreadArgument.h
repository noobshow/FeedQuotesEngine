#if !defined MT4ThreadArgument_H
#define MT4ThreadArgument_H

#include "../stdafx.h"
#include "../Common/Constant.h"
#include <string>

//#include "ClientController.h"
using namespace std;

//class mySemaphore;
class MT4TcpSocket;
class MT4ThreadEvent;
class MT4Controller;

class MT4ThreadArgument
{

private:

	// you have to change here to fit your needs

	//mySemaphore* coutSemaphore;       // semaphore for accessing the log file
	MT4TcpSocket* mMT4Connection;    // the connecting socket to the client
	string mHostName;                // the name of the client
	MT4ThreadArgument* mMT4ThreadArgument[MAX_NUM_MT4_SERVER];
    int mNumOfConnectedClients;

	// but the exitEvent is always needed
	MT4ThreadEvent* mExitEvent;
	bool mSignalToEnd;

	//This Class Controller
	MT4Controller *mMT4Controller;
private:
	//Fubctions
	MT4ThreadArgument(MT4TcpSocket* client,string& cName);

public:

	// you have to change here to fit your needs
	//ClientThreadArgument(ClientTcpSocket* client,mySemaphore* coutSem,string& cName);
	
	MT4ThreadArgument::MT4ThreadArgument(MT4TcpSocket* client,string& cName,MT4Controller * aClientController);
	~MT4ThreadArgument() ;

	// use the accessor functions that fit your needs

	//mySemaphore* getCoutSemaphore()     { return coutSemaphore; }
	MT4TcpSocket* getClientConnect()		{ return mMT4Connection; }
	string&      getHostName()				{ return mHostName; }
	bool getSignalToEnd()					{ return mSignalToEnd; }
	void setSignalToEnd(bool s)				{ mSignalToEnd = s; }

	void addClientArgument(MT4ThreadArgument*);
	void removeClientArgument(MT4ThreadArgument* argument,int aIndex);
	void shiftClientsForRemoved(int aDeletedIndex);
	MT4ThreadArgument* getClientArgument(int);
	
	//Handle For Cotroller class Members
	MT4Controller * getThisClassController();

	// do not change here
    MT4ThreadEvent* getExitEvent() { return mExitEvent;}
};

#endif