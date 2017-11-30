#include "stdafx.h"
#include "FeedClientSocketThread.h"
//#include "../Common/ThreadException.h"
#include "FeedClientSocketHandle\FeedClientException.h"



FeedClientSocketThread::FeedClientSocketThread(LPTHREAD_START_ROUTINE pThreadFunc,
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

void FeedClientSocketThread::execute()
{
	if ( (mThread = CreateThread(mSecurityAttribute,mStackSize,mThreadFunction,mThreadArgument,mExecuteFlag,&mThreadId)) == NULL)
    {
		int errCode = 0;
		string header = "CreateThread() failed";
		getErrorDetail(errCode,header);
		//ThreadException excp(errCode,header);
		FeedClientException excp(errCode,header);
        throw excp;
        return;
    }
}

FeedClientSocketThread::~FeedClientSocketThread()
{
	if (mSecurityAttribute) 
	{
		HeapFree(GetProcessHeap(),0,mSecurityAttribute); 
		mSecurityAttribute = NULL;
	}
}

void FeedClientSocketThread::waitForThreadToFinish()
{
	if ( WaitForSingleObject(mThread,INFINITE) == WAIT_FAILED )
	{
		int errCode = 0;
		string header = "WaitForSingleObject() failed";
		getErrorDetail(errCode,header);
		//ThreadException excp(errCode,header);
		FeedClientException excp(errCode,header);
		throw excp;
		return;
    }
}

DWORD FeedClientSocketThread::getExitCode()
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
        //ThreadException excp(errCode,header);
		FeedClientException excp(errCode,header);
        throw excp;
    }
}

DWORD FeedClientSocketThread::getThreadId()
{
	return mThreadId;
}

void FeedClientSocketThread::resume()
{
	if ( ResumeThread(mThread) == 0xFFFFFFFF )
	{
		int errCode = 0;
		string header = "ResumeThread() failed";
		getErrorDetail(errCode,header);
		//ThreadException excp(errCode,header);
		FeedClientException excp(errCode,header);
        throw excp;
        return;
    }
}

void FeedClientSocketThread::suspend()
{
	if ( SuspendThread(mThread) == 0xFFFFFFFF )
	{
		int errCode = 0;
		string header = "SuspendThread() failed";
		getErrorDetail(errCode,header);
		//ThreadException excp(errCode,header);
		FeedClientException excp(errCode,header);
		throw excp;
        return;
    }
}

int FeedClientSocketThread::getThreadPriority()
{
    int retVal;
	if ( (retVal = GetThreadPriority(mThread)) == THREAD_PRIORITY_ERROR_RETURN )
	{
		int errCode;
		string header = "GetThreadPriority() failed";
		getErrorDetail(errCode,header);
		//ThreadException excp(errCode,header);
		FeedClientException excp(errCode,header);
        throw excp;
        return 0;
    }
    return retVal;
}

void FeedClientSocketThread::setThreadPriority(int thePriority)
{
	if ( !SetThreadPriority(mThread,thePriority) )
    {
		int errCode = 0;
		string header = "SetThreadPriority() failed";
		getErrorDetail(errCode,header);
        //ThreadException excp(errCode,header);
		FeedClientException excp(errCode,header);
        throw excp;
    }
}

void FeedClientSocketThread::disablePriorityBoost(BOOL disable)
{
    if ( !SetThreadPriorityBoost(mThread,disable) )
	{
		int errCode = 0;
		string header = "SetThreadPriorityBoost() failed";
		getErrorDetail(errCode,header);
		//ThreadException excp(errCode,header);
		FeedClientException excp(errCode,header);
        throw excp;
    }
}

BOOL FeedClientSocketThread::isPriorityBoostDisabled()
{
	BOOL boosted;

	if ( !GetThreadPriorityBoost(mThread,&boosted) )
	{
		int errCode = 0;
		string header = "GetThreadPriorityBoost() failed";
		getErrorDetail(errCode,header);
		//ThreadException excp(errCode,header);
		FeedClientException excp(errCode,header);
		throw excp;
    }

	return boosted;
}

void FeedClientSocketThread::setProcessorMask(DWORD mask)
{
	if ( !SetThreadAffinityMask(mThread,mask) )
	{
		int errCode = 0;
		string header = "SetThreadAffinityMask() failed";
		getErrorDetail(errCode,header);
		//ThreadException excp(errCode,header);
		FeedClientException excp(errCode,header);
		throw excp;
    }
}

void FeedClientSocketThread::setIdealProcessor(DWORD proc)
{
	if ( SetThreadIdealProcessor(mThread,proc) == -1 )
	{
		int errCode = 0;
		string header = "SetThreadIdealProcessor failed";
		getErrorDetail(errCode,header);
		//ThreadException excp(errCode,header);
		FeedClientException excp(errCode,header);
		throw excp;
    }
}

void FeedClientSocketThread::getThreadStatistics(LPFILETIME  creationTime,LPFILETIME exitTime,
								   LPFILETIME kernelTime,LPFILETIME userTime)
{
	if ( !GetThreadTimes(mThread,creationTime,exitTime,kernelTime,userTime) )
    {
		int errCode = 0;
		string header = "GetThreadTimes() failed";
		getErrorDetail(errCode,header);
		//ThreadException excp(errCode,header);
		FeedClientException excp(errCode,header);
        throw excp;
    }
}

void FeedClientSocketThread::getErrorDetail(int& errCode,string& header)
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
