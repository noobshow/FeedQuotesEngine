#include "../stdafx.h"
#include "MT4Thread.h"
#include "../Common/ThreadException.h"



MT4Thread::MT4Thread(LPTHREAD_START_ROUTINE pThreadFunc,
				   LPVOID pThreadFuncParameter,DWORD exeFlags,
				   DWORD sSize,BOOL inheritable)
{
	mSecurityAttribute = (LPSECURITY_ATTRIBUTES)HeapAlloc(GetProcessHeap(),0,sizeof(SECURITY_ATTRIBUTES));
	mSecurityAttribute->nLength = sizeof(SECURITY_ATTRIBUTES);
    mSecurityAttribute->lpSecurityDescriptor = NULL;
	mSecurityAttribute->bInheritHandle = inheritable;

	mThreadFunction = pThreadFunc;
	mThreadArgument = pThreadFuncParameter;

	mExecuteFlag = exeFlags;
	mStackSize = sSize;
	mExitCode = 0xFFFFFFFF;

}

void MT4Thread::execute()
{
	if ( (mThread = CreateThread(mSecurityAttribute,mStackSize,mThreadFunction,mThreadArgument,mExecuteFlag,&mThreadId)) == NULL)
    {
		int errCode = 0;
		string header = "CreateThread() failed";
		getErrorDetail(errCode,header);
		ThreadException excp(errCode,header);
        throw excp;
        return;
    }
}

MT4Thread::~MT4Thread()
{
	if (mSecurityAttribute) 
	{
		HeapFree(GetProcessHeap(),0,mSecurityAttribute); 
		mSecurityAttribute = NULL;
	}
}

void MT4Thread::waitForThreadToFinish()
{
	if ( WaitForSingleObject(mThread,INFINITE) == WAIT_FAILED )
	{
		int errCode = 0;
		string header = "WaitForSingleObject() failed";
		getErrorDetail(errCode,header);
		ThreadException excp(errCode,header);
		throw excp;
		return;
    }
}

DWORD MT4Thread::getExitCode()
{
	if ( GetExitCodeThread(mThread,&mExitCode) )
    {
		return mExitCode;
    }
    else
	{
		int errCode = 0;
		string header = "GetExitCodeThread() failed";
		getErrorDetail(errCode,header);
        ThreadException excp(errCode,header);
        throw excp;
    }
}

DWORD MT4Thread::getThreadId()
{
	return mThreadId;
}

void MT4Thread::resume()
{
	if ( ResumeThread(mThread) == 0xFFFFFFFF )
	{
		int errCode = 0;
		string header = "ResumeThread() failed";
		getErrorDetail(errCode,header);
		ThreadException excp(errCode,header);
        throw excp;
        return;
    }
}

void MT4Thread::suspend()
{
	if ( SuspendThread(mThread) == 0xFFFFFFFF )
	{
		int errCode = 0;
		string header = "SuspendThread() failed";
		getErrorDetail(errCode,header);
		ThreadException excp(errCode,header);
		throw excp;
        return;
    }
}

int MT4Thread::getThreadPriority()
{
    int retVal;
	if ( (retVal = GetThreadPriority(mThread)) == THREAD_PRIORITY_ERROR_RETURN )
	{
		int errCode;
		string header = "GetThreadPriority() failed";
		getErrorDetail(errCode,header);
		ThreadException excp(errCode,header);
        throw excp;
        return 0;
    }
    return retVal;
}

void MT4Thread::setThreadPriority(int thePriority)
{
	if ( !SetThreadPriority(mThread,thePriority) )
    {
		int errCode = 0;
		string header = "SetThreadPriority() failed";
		getErrorDetail(errCode,header);
        ThreadException excp(errCode,header);
        throw excp;
    }
}

void MT4Thread::disablePriorityBoost(BOOL disable)
{
    if ( !SetThreadPriorityBoost(mThread,disable) )
	{
		int errCode = 0;
		string header = "SetThreadPriorityBoost() failed";
		getErrorDetail(errCode,header);
		ThreadException excp(errCode,header);
        throw excp;
    }
}

BOOL MT4Thread::isPriorityBoostDisabled()
{
	BOOL boosted;

	if ( !GetThreadPriorityBoost(mThread,&boosted) )
	{
		int errCode = 0;
		string header = "GetThreadPriorityBoost() failed";
		getErrorDetail(errCode,header);
		ThreadException excp(errCode,header);
		throw excp;
    }

	return boosted;
}

void MT4Thread::setProcessorMask(DWORD mask)
{
	if ( !SetThreadAffinityMask(mThread,mask) )
	{
		int errCode = 0;
		string header = "SetThreadAffinityMask() failed";
		getErrorDetail(errCode,header);
		ThreadException excp(errCode,header);
		throw excp;
    }
}

void MT4Thread::setIdealProcessor(DWORD proc)
{
	if ( SetThreadIdealProcessor(mThread,proc) == -1 )
	{
		int errCode = 0;
		string header = "SetThreadIdealProcessor failed";
		getErrorDetail(errCode,header);
		ThreadException excp(errCode,header);
		throw excp;
    }
}

void MT4Thread::getThreadStatistics(LPFILETIME  creationTime,LPFILETIME exitTime,
								   LPFILETIME kernelTime,LPFILETIME userTime)
{
	if ( !GetThreadTimes(mThread,creationTime,exitTime,kernelTime,userTime) )
    {
		int errCode = 0;
		string header = "GetThreadTimes() failed";
		getErrorDetail(errCode,header);
		ThreadException excp(errCode,header);
        throw excp;
    }
}

void MT4Thread::getErrorDetail(int& errCode,string& header)
{
	char buf[256];
	LPVOID lpDetailMsgBuf;
	UINT errorCode;

    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
				  NULL,errorCode=GetLastError(),MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
				  (LPTSTR)&lpDetailMsgBuf,0,NULL);

	memset(buf,0,255);
	wsprintf(buf,": %s",(LPTSTR)lpDetailMsgBuf);
    LocalFree(lpDetailMsgBuf);	
	
	header.append(string(buf));
	errCode = errorCode;
}
