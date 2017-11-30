#if !defined SingletonFeedClient_H
#define SingletonFeedClient_H

#define WIN32_LEAN_AND_MEAN

#include "Common/FXTFLog.h"

class MySQLDBOperation;
class FeedClientSocketController;
class FeedMonitorInfo;
class UsersModel;

class SingletonFeedClient{

private:
	static bool					mInstanceFlag;
	static SingletonFeedClient	*mSingletonFeedClient;
	MySQLDBOperation			*mMySQLDBOperation;
	FXTFLog						*mFXTFLog;
	FeedClientSocketController	*mFeedClientSocketController;
	FeedMonitorInfo				*mFeedMonitorInfo;
	UsersModel					*mUsersModelInfo;

	SingletonFeedClient();
	~SingletonFeedClient();

public:
	static SingletonFeedClient* GetInstance();

	MySQLDBOperation* getMySQLDBOperation(){return mMySQLDBOperation;}
	void setMySQLDBOperation(MySQLDBOperation* aMySQLDBOperation){mMySQLDBOperation=aMySQLDBOperation;}
	
	FXTFLog* getFXTFLog(){return mFXTFLog;}
	void setFXTFLog(FXTFLog* aFXTFLog){mFXTFLog=aFXTFLog;}

	FeedClientSocketController* getFeedClientSocketController(){return mFeedClientSocketController;}
	void setFeedClientSocketController(FeedClientSocketController* aFeedClientSocketController){mFeedClientSocketController=aFeedClientSocketController;}
	
	FeedMonitorInfo* getFeedMonitorInfo(){return mFeedMonitorInfo;}
	void setFeedMonitorInfo(FeedMonitorInfo* aFeedMonitorInfo){mFeedMonitorInfo=aFeedMonitorInfo;}

	UsersModel* getUsersModel(){return mUsersModelInfo;}
	void setUsersModel(UsersModel* aUsersModelInfo){mUsersModelInfo=aUsersModelInfo;}
};

#endif