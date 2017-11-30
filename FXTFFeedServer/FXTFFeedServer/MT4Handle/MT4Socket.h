#ifndef ClientSocket_H
#define ClientSocket_H

#include "MT4HostInfo.h"

#ifdef UNIX
    #include <sys/socket.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <errno.h>
    #include <iostream.h>
    #include <sys/types.h>
    #include <stropts.h>
    #include <sys/filio.h>
#else
    #include <winsock2.h>
#endif

// so far we only consider the TCP socket, UDP will be added in later release
//const int MAX_RECV_LEN = 1048576;


class MT4Socket
{

protected:

	/*
	   only used when the socket is used for client communication
	   once this is done, the next necessary call is setSocketId(int)
	*/
	MT4Socket() {}
	void setSocketId(int socketFd) { socketId = socketFd; }

protected:

    int portNumber;        // Socket port number    
    int socketId;          // Socket file descriptor

	SOCKET            m_socket_client;
	UINT              m_socket_client_timeout;
	WSAEVENT          m_socket_client_event;

    int blocking;          // Blocking flag
    int bindFlag;          // Binding flag

    struct sockaddr_in clientAddr;    // Address of the client that sent data

public:

    MT4Socket(int);                       // given a port number, create a socket

    virtual ~MT4Socket()
    {
		#ifdef WINDOWS_XP
            closesocket(socketId);
        #else
            close(socketId);
        #endif
     }

public:

	// socket options : ON/OFF

    void setDebug(int);
    void setReuseAddr(int);
    void setKeepAlive(int);
    void setLingerOnOff(bool);
	void setLingerSeconds(int);
    void setSocketBlocking(int);

    // size of the send and receive buffer

    void setSendBufSize(int);
    void setReceiveBufSize(int);

    // retrieve socket option settings

    int  getDebug();
    int  getReuseAddr();
    int  getKeepAlive();
    int  getSendBufSize();
    int  getReceiveBufSize();
    int  getSocketBlocking() { return blocking; }
	int  getLingerSeconds();
    bool getLingerOnOff();
	
    // returns the socket file descriptor
    int getSocketId() { return socketId; }

	// returns the port number
	int getPortNumber() { return portNumber; }

	// show the socket 
	friend ostream& operator<<(ostream&,MT4Socket&);

private:

	// Gets the system error
	#ifdef WINDOWS_XP
		void detectErrorOpenWinSocket(int*,string&);
		void detectErrorSetSocketOption(int*,string&);
		void detectErrorGetSocketOption(int*,string&);
	#endif

    #ifdef UNIX
		char *sGetError()
		{
           return strerror(errno);
        }
	#endif
};

class MT4TcpSocket : public MT4Socket
{

private:
	bool mSocketCommunicationErrorOccured;
	

	#ifdef WINDOWS_XP
		// Windows version of the MSG_WAITALL option
		int XPrecieveMessage(string&);
	#endif

public:
	bool mIsRecevingFromClient;
	// initialization work, the clean up is in the destructor
	static void initialize();

public:

	/* 
	   Constructor. used for creating instances dedicated to client 
	   communication:

	   when accept() is successful, a socketId is generated and returned
	   this socket id is then used to build a new socket using the following
	   constructor, therefore, the next necessary call should be setSocketId()
	   using this newly generated socket fd
	*/
	MT4TcpSocket() {
		mSocketCommunicationErrorOccured=false;
		mIsRecevingFromClient=false;
	};
	~MT4TcpSocket();

	// Constructor.  Used to create a new TCP socket given a port
	MT4TcpSocket(int portId) : MT4Socket(portId) { };

	/*
	   Sends a message to a connected host. The number of bytes sent is returned
	   can be either server call or client call
	*/
	int sendMessage(string&);

	/*
	   receive messages and stores the message in a buffer
	*/
	int recieveMessage(string&);
	bool ReadData(char *buf,int len);
	/*
	   Binds the socket to an address and port number
	   a server call
	*/
	void bindSocket();
	void closeSocket();

	/*
	   accepts a connecting client.  The address of the connected client 
	   is stored in the parameter
	   a server call
	*/
	MT4TcpSocket* acceptClient(string&);

	// Listens to connecting clients, a server call
	void listenToClient(int numPorts = 5);

	// connect to the server, a client call
	virtual void connectToServer(string&,hostType);

private:

	void detectErrorBind(int*,string&);
	void detectErrorSend(int*,string&);
	void detectErrorRecv(int*,string&);
	void detectErrorConnect(int*,string&);
    void detectErrorAccept(int*,string&);
	void detectErrorListen(int*,string&);

};

#endif
        