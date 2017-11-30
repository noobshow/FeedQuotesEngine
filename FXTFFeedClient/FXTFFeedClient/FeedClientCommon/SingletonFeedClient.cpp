#include "SingletonFeedClient.h"
#include <iostream>

bool SingletonFeedClient::mInstanceFlag							= false;
SingletonFeedClient* SingletonFeedClient::mSingletonFeedClient	= NULL;

SingletonFeedClient::SingletonFeedClient()
{
	mFeedMonitorInfo=NULL;
}

SingletonFeedClient::~SingletonFeedClient()
{
	mInstanceFlag = false;
}

SingletonFeedClient* SingletonFeedClient::GetInstance()
{
	if(!mInstanceFlag)
	{
		mSingletonFeedClient = new SingletonFeedClient();
		mInstanceFlag = true;
		return mSingletonFeedClient;
	}
	else
	{
		return mSingletonFeedClient;
	}
}

