#if !defined MainThreadArgument_H
#define MainThreadArgument_H

class ClientController;
class MT4Controller;
class FilterProcessController;
class FixHandleController;
class DBOperationController;
class SettingsController;
class FXTFLog;

class MainThreadArgument
{
private:
	//Members
	ClientController*			mClientController;
	MT4Controller*				mMT4Controller;
	FilterProcessController*	mFilterProcessController;
	FixHandleController*		mFixHandleController;
	DBOperationController*		mDBOperationController;
	SettingsController*			mSettingsController;
	FXTFLog*					mLogWriter;
private:
	//Functions

public:
	MainThreadArgument(void);
	~MainThreadArgument(void);

	ClientController* getClientController()     { return mClientController; }
	void setClientController(ClientController* aClientController)  { mClientController = aClientController; }

	MT4Controller* getMT4Controller()     { return mMT4Controller; }
	void setMT4Controller(MT4Controller* aMT4Controller)  { mMT4Controller = aMT4Controller; }

	FilterProcessController* getFilterProcessController() { return mFilterProcessController; }
	void setFilterProcessController(FilterProcessController* aFilterProcessController)  { mFilterProcessController = aFilterProcessController; }

	FixHandleController* getFixHandleController() { return mFixHandleController; }
	void setFixHandleController(FixHandleController* aFixHandleController)  { mFixHandleController = aFixHandleController; }

	DBOperationController* getDBOperationController() { return mDBOperationController; }
	void setDBOperationController(DBOperationController* aDBOperationController)  { mDBOperationController = aDBOperationController; }

	SettingsController* getSettingsController() { return mSettingsController; }
	void setSettingsController(SettingsController* aSettingsController)  { mSettingsController = aSettingsController;}

	FXTFLog* getLogWriter() { return mLogWriter; }
	void setLogWriter(FXTFLog* aLogWriter)  { mLogWriter = aLogWriter; }

};

#endif