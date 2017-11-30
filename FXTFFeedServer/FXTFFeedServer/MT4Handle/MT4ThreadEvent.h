#if !defined MT4ThreadEvent_H 
#define MT4ThreadEvent_H

#define _WIN32_WINNT 0x0400
#include <windows.h>
#include <string>
using namespace std;

class MT4ThreadEvent  
{

public:
	
	MT4ThreadEvent(string eventName="",PAPCFUNC _pfnAPC=NULL,BOOL queueUserAPC=FALSE);
    virtual ~MT4ThreadEvent();

	// return the event handle
    HANDLE getEventHandle() { return mEvent; }

	// return the thread handle
	HANDLE getThread() { return mThread; }

	// return the address of the callback function 
    PAPCFUNC getAPCFunc() { return pfnAPC; }

	// return whether the user wants to queue the callback 
	// when event happens
	BOOL getQueueUserAPC() { return queueUserAPC; }

    void setEvent();      // wrapper to SetEvent()  
	void pulseEvent();    // wrapper to PulseEvent()
	void resetEvent();    // wrapper to ResetEvent()

	// wait for the event
    BOOL waitForEvent(DWORD waitTime=INFINITE);
    
private:

	// called by the OS when the thread is instantiated, this
	// must have the WINAPI function declaration
    static DWORD WINAPI ThreadProc(LPVOID lpData);

private:

    HANDLE mEvent;                  // event handle
    PAPCFUNC pfnAPC;               // address of user-provided callback
	LPSECURITY_ATTRIBUTES mSecurityAttribute;      // SECURITY_ATTRBUTES structure
    DWORD  mThreadID;               // thread id
    HANDLE mThread;                 // handle to thread

	// if the following is set to be TRUE, when the events occurs, 
	// will call QueueUserAPC() to queue the user-provided callback
	// as a user defined API. if the following is set to be FALSE;
	// then no user-provided callback will be queued. 
	// if pfnAPC is NULL (no user-provided callback is defined),
	// then the following is ignored 
	BOOL queueUserAPC;        
	
private:

	static void getErrorDetail(int&,string&);

};

#endif