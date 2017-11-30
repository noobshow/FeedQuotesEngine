#include "../stdafx.h"
#include "../MT4Handle/MT4ThreadEvent.h"
#include "MT4ThreadArgument.h"


/*
ClientThreadArgument::myThreadArgument(myTcpSocket* client,mySemaphore* coutSem,string& cName)
{
	coutSemaphore = coutSem;
	clientConnection = client;
	hostName = cName;
	exitEvent = new myEvent();
	signalToEnd = false;

	for ( int i = 0; i < MAX_NUM_CLIENTS; i ++ ) clientArgument[i] = NULL;
    numOfConnectedClients = 0;
}

*/
MT4ThreadArgument::MT4ThreadArgument(MT4TcpSocket* client,string& cName)
{
	//coutSemaphore = coutSem;
	mMT4Connection = client;
	mHostName = cName;
	mExitEvent = new MT4ThreadEvent();
	mSignalToEnd = false;

	for ( int i = 0; i < MAX_NUM_CLIENTS; i ++ ) mMT4ThreadArgument[i] = NULL;
    mNumOfConnectedClients = 0;
}

MT4ThreadArgument::MT4ThreadArgument(MT4TcpSocket* client,string& cName,MT4Controller* aMT4Controller)
{
	//coutSemaphore = coutSem;
	mMT4Connection = client;
	mMT4Controller=aMT4Controller;
	mHostName = cName;
	mExitEvent = new MT4ThreadEvent();
	mSignalToEnd = false;

	for ( int i = 0; i < MAX_NUM_CLIENTS; i ++ ) mMT4ThreadArgument[i] = NULL;
    mNumOfConnectedClients = 0;
}


MT4ThreadArgument::~MT4ThreadArgument() 
{
	delete mExitEvent;
}

MT4Controller* MT4ThreadArgument::getThisClassController()
{
	return mMT4Controller;
}

void MT4ThreadArgument::addClientArgument(MT4ThreadArgument* argument)
{
	if ( argument )
		mMT4ThreadArgument[mNumOfConnectedClients++] = argument;
}

void MT4ThreadArgument::removeClientArgument(MT4ThreadArgument* argument,int aIndex)
{
	if ( argument )
	{
		mMT4ThreadArgument[aIndex] = NULL;
		mNumOfConnectedClients--;
		shiftClientsForRemoved(aIndex);
	}
}

MT4ThreadArgument* MT4ThreadArgument::getClientArgument(int index)
{
	if ( index < MAX_NUM_MT4_SERVER ) return mMT4ThreadArgument[index];
	return NULL;
}

void MT4ThreadArgument::shiftClientsForRemoved(int aDeletedIndex)
{
	for (int i = 0; i < MAX_NUM_MT4_SERVER; i++)
	{
		if (i>=aDeletedIndex && i+1<MAX_NUM_MT4_SERVER)
		{
			mMT4ThreadArgument[i]=mMT4ThreadArgument[i+1];
		}	
	}
}

