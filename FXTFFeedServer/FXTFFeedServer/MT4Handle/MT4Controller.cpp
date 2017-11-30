#include "../stdafx.h"
#include "MT4Controller.h"
#include "../Common/SocketLog.h"
#include "../Common/FXTFLog.h"

#define WIN32_LEAN_AND_MEAN



MT4Controller::MT4Controller(MainThreadArgument* aMainThreadArgument )
{
	mMainThreadArgument=aMainThreadArgument;
	aMainThreadArgument->setMT4Controller(this);
	mCurrentNumberOfClients=0;
	mServerMT4Argument = NULL;
}


MT4Controller::~MT4Controller(void)
{
	delete mServerMT4Argument;
	delete mMT4TCPServer;
}

bool MT4Controller::initializeServerForClient(void)
{
	// Initialize the winsock library
	MT4TcpSocket::initialize();

	MT4HostInfo lServerInfo;
	string lServerName = lServerInfo.getHostName();
    string lServerIPAddress = lServerInfo.getHostIPAddress();

	cout << "my localhost (server) information:" << endl;
	cout << "	Name:    " << lServerName << endl;
    cout << "	Address: " << lServerIPAddress << endl;
	
	mMT4TCPServer=new MT4TcpSocket(PORTNUM_MT4_SERVER);
	cout   << mMT4TCPServer;

	mServerMT4Argument = new MT4ThreadArgument(mMT4TCPServer,lServerName,this);
	MT4Thread* lServerMT4Thread = new MT4Thread(MT4Controller::MT4ServerHandleThread,(void*)mServerMT4Argument);
	lServerMT4Thread->execute();

	//Another Thread For Handeling removed Client

	//ClientThread* lServerClientThreadFroRemoveOption = new ClientThread(ClientController::ClientServerHandleThreadForremovedClient,(void*)mServerClientArgument);
	//lServerClientThreadFroRemoveOption->execute();

	return true;
}



void MT4Controller::sendFeedMessageToAllClient(string aFeedMessage)
{
	//cout <<getCurrentTime()<<" => "<<aFeedMessage<<endl;

	if (mServerMT4Argument!=NULL)
	{
		for (int i = 0; i < MAX_NUM_MT4_SERVER; i++)
		{
			MT4ThreadArgument* lMT4Info=mServerMT4Argument->getClientArgument(i);
			
			if (lMT4Info)
			{
				//Need To handle in more case
				//if (!lMT4Info->getExitEvent()->waitForEvent(0))
				{
					int lSendNumBytes=lMT4Info->getClientConnect()->sendMessage(aFeedMessage);
					if (lSendNumBytes==-99)
					{
						Sleep(5);
						removeMessageSendFailedClientFromList(lMT4Info);
					}
				}
			}
		}
	}
	
}



void MT4Controller::sendMessageToSpecificClient(string Message)
{
	// Need to Develop for Specific Client Operation
}


DWORD WINAPI MT4Controller::MT4ServerHandleThread(LPVOID aThreadInfo)
{
	// this structure will contain all the data this callback will work on
    MT4ThreadArgument* lServerMT4Argument = (MT4ThreadArgument*)aThreadInfo;

	// get the server
	MT4TcpSocket* lServerForClient = lServerMT4Argument->getClientConnect();
	string lServerName = lServerMT4Argument->getHostName();
	// bind the server to the socket
	lServerForClient->bindSocket();
	MT4Controller *lThisClass=lServerMT4Argument->getThisClassController();
	


	cout   << endl << "server finishes binding process... " << endl;
	
	lThisClass->getMainThreadArgument()->getLogWriter()->LogOutLn(" Info :: Server Started and Binding Properly For MT4 ");

	// server starts to wait for client calls
	lServerForClient->listenToClient();

	MT4ThreadArgument* lMT4Argument[MAX_NUM_MT4_SERVER];
	MT4Thread* lMT4Handle[MAX_NUM_MT4_SERVER];
	for ( int i = 0; i < MAX_NUM_MT4_SERVER; i++ )
	{
		lMT4Argument[i] = NULL;
		lMT4Handle[i] = NULL;
	}

	//Infinite loop
	//Ready to Accept Client Connection Request 
	while (true)
	{
		// wait to accept a client connection.  
		// processing is suspended until the client connects
    	MT4TcpSocket* lClientTCPSocket;		// connection dedicated for client communication
		string lClientName;				// client name 
		lClientTCPSocket = lServerForClient->acceptClient(lClientName);	
		lClientName = lClientName + "-" +std::to_string(lThisClass->getCurrentNumberOfClients());
		
		cout << "Connection Request Recieved From(MT4) : "<<lClientName<<endl;
		
		lThisClass->getMainThreadArgument()->getLogWriter()->LogOutLn("Connection Request Recieved From(MT4) : "+lClientName);
		
		if (lThisClass->getCurrentNumberOfClients()<MAX_NUM_MT4_SERVER-1)
		{
			lMT4Argument[lThisClass->getCurrentNumberOfClients()] = new MT4ThreadArgument(lClientTCPSocket,lClientName,lThisClass);
			//lMT4Handle[lThisClass->getCurrentNumberOfClients()] = new MT4Thread(MT4Controller::MT4HandleThread,(void*)lMT4Argument[lThisClass->getCurrentNumberOfClients()]);
			lServerMT4Argument->addClientArgument(lMT4Argument[lThisClass->getCurrentNumberOfClients()]);
			
			//lMT4Handle[lThisClass->getCurrentNumberOfClients()]->execute();
			lThisClass->increaseCurrentNumberOfClients();
		}
	}
	return 1;
}

bool MT4Controller::ConfirmLogin(MT4TcpSocket* aClientTCPSocket)
  {
   string lReciveMessage="";

   aClientTCPSocket->recieveMessage(lReciveMessage);
   mMainThreadArgument->getLogWriter()->LogOutLn("Connection Request Recieved From(MT4) : ");
   /*
//---- checks
   if(m_socket_client==INVALID_SOCKET) return(FALSE);
//--- send "Login: "
   strcpy_s(tmp,sizeof(tmp),"Login: ");
   if(!SendString(tmp))
     {
      Close();
      return(FALSE);
     }
//--- wait for login
   if(!ReadString(tmp,sizeof(tmp)))
     {
      Close();
      return(FALSE);
     }
//--- send "Password: "
   strcpy_s(tmp,sizeof(tmp),"Password: ");
   if(!SendString(tmp))
     {
      Close();
      return(FALSE);
     }
//--- wait for password
   if(!ReadString(tmp,sizeof(tmp)))
     {
      Close();
      return(FALSE);
     }
//--- send access granted
   strcpy_s(tmp,sizeof(tmp),"Access granted\n\r");
   if(!SendString(tmp))
     {
      Close();
      return(FALSE);
     }
   printf_s("Login: successful\r\n");
   */
//---
   return(TRUE);
  }
//+------------------------------------------------------------------+
//| Get symbols list                                                 |
//+------------------------------------------------------------------+
bool MT4Controller::GetRequestedSymbols(MT4TcpSocket* aClientTCPSocket)
  {
	  /*
   char  tmp[1024];
//---- checks
   if(m_socket_client==INVALID_SOCKET) return(FALSE);
//--- wait for login
   if(ReadCheckString(tmp,sizeof(tmp),"> Symbols:")<=0)
     {
      Close();
      return(FALSE);
     }
   if(!ReadString(tmp,sizeof(tmp)))
     {
      Close();
      return(FALSE);
     }
//--- parse symbols list
   char* smb_start=tmp;
   char* smb_end;
   for(UINT i=0;i<_countof(m_ticks) && *smb_start;i++)
     {
      smb_end=strstr(smb_start,";");
      if(smb_end==NULL)
         break;
      *smb_end='\0';
      strcpy_s(m_ticks[i].symbol,sizeof(m_ticks[i].symbol),smb_start);
      m_ticks[i].bid=1.0;
      m_ticks[i].ask=1.01;
      smb_start=smb_end+1;
     }
//---
   printf_s("GetSymbols: successful\r\n");
   */
   return(TRUE);
  }

DWORD WINAPI MT4Controller::MT4ServerHandleThreadForremovedClient(LPVOID aThreadInfo)
{
	MT4ThreadArgument* lServerMT4Argument = (MT4ThreadArgument*)aThreadInfo;
	MT4Controller *lThisClass=lServerMT4Argument->getThisClassController();
	while (true)
	{
		lThisClass->removeDisconnectedClientFromList();
	}
	return 1;
}

void MT4Controller::removeDisconnectedClientFromList()
{
	for ( int i = 0; i < MAX_NUM_MT4_SERVER; i ++ )
		{
			MT4ThreadArgument* lMT4Info = mServerMT4Argument->getClientArgument(i);
			if ( lMT4Info && lMT4Info->getExitEvent()->waitForEvent(0) )
			{
				lMT4Info->setSignalToEnd(true);
				cout   << " Removed >> " << lMT4Info->getHostName() << endl;
				mMainThreadArgument->getLogWriter()->LogOutLn("Removed1 MT4 : "+lMT4Info->getHostName());
				mServerMT4Argument->removeClientArgument(lMT4Info,i);
				decreaseCurrentNumberOfClients();
				break;
			}
		}
}

void MT4Controller::removeMessageSendFailedClientFromList(MT4ThreadArgument* aClientInfo)
{
	for ( int i = 0; i < MAX_NUM_MT4_SERVER; i ++ )
	{
		MT4ThreadArgument* lMT4Info = mServerMT4Argument->getClientArgument(i);
		if (lMT4Info!=NULL && aClientInfo!=NULL)
		{
			if (!lMT4Info->getSignalToEnd())
			{
				if ( strcmp(lMT4Info->getHostName().c_str(),aClientInfo->getHostName().c_str())==0)
				{
					lMT4Info->setSignalToEnd(true);
					cout   << " Removed >> " << lMT4Info->getHostName() << endl;
					mMainThreadArgument->getLogWriter()->LogOutLn("Removed2 MT4 : "+lMT4Info->getHostName());
					mServerMT4Argument->removeClientArgument(lMT4Info,i);
					decreaseCurrentNumberOfClients();
					break;
				}
			}
		}
	}
}

DWORD WINAPI MT4Controller::MT4HandleThread(LPVOID aThreadInfo)
{
	// this structure will contain all the data this callback will work on
    MT4ThreadArgument* lMT4Argument = (MT4ThreadArgument*)aThreadInfo;
	
	//Get This Class Controller
	MT4Controller *lThisClass=lMT4Argument->getThisClassController();

	// get the client connection: receiving messages from client and
	// sending messages to the client will all be done by using
	// this client connection
	MT4TcpSocket* lMT4Connection = lMT4Argument->getClientConnect();
	string lClientName = lMT4Argument->getHostName();
	//cout << "New Client Added, Client Name : "<<clientName<<endl;
	// Get Client response 
	lThisClass->getMainThreadArgument()->getLogWriter()->LogOutLn("Thread Started Properly For : "+lClientName);
	/*
	while (true)
	{
		string lMessageFromSpecificClient = "";
		//lMT4Connection->mIsRecevingFromClient=true;
		// receive from the client
		//int lNumBytes = lMT4Connection->recieveMessage(lMessageFromSpecificClient);
		
		//cout << "Client Receve Bytes : "<<lNumBytes;
		
		//if ( lNumBytes == -99 )
		//{
			//cout << "Going to remove Client From List"<<endl;
		//	lThisClass->removeMessageSendFailedClientFromList(lMT4Argument);
		//	break;
		//}
		
		// Message Format for Client Message Handle

		//string lFormattedClientMessage =  lClientName+" "+lMessageFromSpecificClient+"\n";

		//cout   <<" MT4 : "<< lFormattedClientMessage;
		//lMT4Connection->mIsRecevingFromClient=false;
	}
	*/
	return 1;
}


void MT4Controller::receivedClientResponse(string aClientResponseMessage)
{
	// Handle This Message for Configuretion Setting and else
	cout   <<" Message Recived "<< aClientResponseMessage;
}


int MT4Controller::getCurrentNumberOfClients()
{
	return mCurrentNumberOfClients;
}

bool MT4Controller::increaseCurrentNumberOfClients()
{
	mCurrentNumberOfClients++;
	return true;
}

bool MT4Controller::decreaseCurrentNumberOfClients()
{
	mCurrentNumberOfClients--;

	if (mCurrentNumberOfClients<0)
	{
		return false;
	}
	return true;
}

void MT4Controller::setCurrentNumberOfClients(int aCount)
{
	mCurrentNumberOfClients=aCount;
}

string  MT4Controller::getCurrentTime()
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
