#include "../stdafx.h"
#include "../ClientHandle/ClientThreadEvent.h"
#include "ClientThreadArgument.h"


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
ClientThreadArgument::ClientThreadArgument(ClientTcpSocket* client,string& cName)
{
	//coutSemaphore = coutSem;
	clientConnection = client;
	hostName = cName;
	exitEvent = new ClientThreadEvent();
	signalToEnd = false;

	for ( int i = 0; i < MAX_NUM_CLIENTS; i ++ ) mClientThreadArgument[i] = NULL;
    numOfConnectedClients = 0;
}

ClientThreadArgument::ClientThreadArgument(ClientTcpSocket* client,string& cName,ClientController* aClientController)
{
	//coutSemaphore = coutSem;
	clientConnection = client;
	mClientController=aClientController;
	hostName = cName;
	exitEvent = new ClientThreadEvent();
	signalToEnd = false;

	for ( int i = 0; i < MAX_NUM_CLIENTS; i ++ ) mClientThreadArgument[i] = NULL;
    numOfConnectedClients = 0;
}


ClientThreadArgument::~ClientThreadArgument() 
{
	delete exitEvent;
}

ClientController* ClientThreadArgument::getThisClassController()
{
	return mClientController;
}

void ClientThreadArgument::addClientArgument(ClientThreadArgument* argument)
{
	if ( argument )
		mClientThreadArgument[numOfConnectedClients++] = argument;
}

void ClientThreadArgument::removeClientArgument(ClientThreadArgument* argument,int aIndex)
{
	if ( argument )
	{
		mClientThreadArgument[aIndex] = NULL;
		numOfConnectedClients--;
		shiftClientsForRemoved(aIndex);
	}
}

ClientThreadArgument* ClientThreadArgument::getClientArgument(int index)
{
	if ( index < MAX_NUM_CLIENTS ) return mClientThreadArgument[index];
	return NULL;
}

void ClientThreadArgument::shiftClientsForRemoved(int aDeletedIndex)
{
	for (int i = 0; i < MAX_NUM_CLIENTS; i++)
	{
		if (i>=aDeletedIndex && i+1<MAX_NUM_CLIENTS)
		{
			mClientThreadArgument[i]=mClientThreadArgument[i+1];
		}	
	}
}

