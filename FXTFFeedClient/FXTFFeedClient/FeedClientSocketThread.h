#if !defined FeedClientSocketThread_H
#define FeedClientSocketThread_H

#define _WIN32_WINNT 0x0400
#include <windows.h>
#include <string>
using namespace std;

class FeedClientSocketThread  
{

public:

    FeedClientSocketThread(
		LPTHREAD_START_ROUTINE pThreadFunc,    // address of thread callback
		LPVOID pThreadFuncParameter=NULL,      // address of callback's parameter,
		DWORD  exeFlags=0,                     // creation flag
		DWORD  sSize=0,                        // stack size
		BOOL   inheritable=FALSE               // inheritable
	);        

    ~FeedClientSocketThread();

public:

	void execute();
	void waitForThreadToFinish();    
	void suspend();
    void resume();

	void setThreadPriority(int);
    int  getThreadPriority();

    void disablePriorityBoost(BOOL);
	BOOL isPriorityBoostDisabled();

    void setProcessorMask(DWORD);
	void setIdealProcessor(DWORD);

	LPVOID getThreadArgument() { return mThreadArgument; }
    DWORD getExitCode();    
	DWORD getThreadId();
	void  getThreadStatistics(LPFILETIME,LPFILETIME,LPFILETIME,LPFILETIME);

private:
	
    HANDLE mThread;
    DWORD  mThreadId;
	DWORD  mStackSize;
	DWORD  mExecuteFlag;
	LPTHREAD_START_ROUTINE mThreadFunction;
	LPVOID mThreadArgument;
    LPSECURITY_ATTRIBUTES mSecurityAttribute;
    DWORD mExitCode;

private:

    static void getErrorDetail(int&,string&);
};
#endif