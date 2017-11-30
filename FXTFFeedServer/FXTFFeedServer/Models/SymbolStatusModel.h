#if !defined SymbolStatusModel_H
#define SymbolStatusModel_H

#include "../stdafx.h"

class SymbolStatusModel
{
private:
	int			mAutoIncrimentID;
	int			mSymbolID;
	bool		mIsEnabled;
	bool		mIsActiveToSendMT4;
	bool		mIsActiveToSendClient;

public:
	SymbolStatusModel();
	~SymbolStatusModel();

	int getAutoIncrimentID(){return mAutoIncrimentID;}
	void setAutoIncrimentID(int aAutoIncrimentID){mAutoIncrimentID=aAutoIncrimentID;}

	int getSymbolID(){return mSymbolID;}
	void setSymbolID(int aSymbolID){mSymbolID=aSymbolID;}

	bool isEnabled(){return mIsEnabled;}
	void setEnable(bool aIsEnable){mIsEnabled=aIsEnable;}

	bool isActiveToSendMT4(){return mIsActiveToSendMT4;}
	void setActiveToSendMT4(bool aIsActiveToSendMT4){mIsActiveToSendMT4=aIsActiveToSendMT4;}

	bool isActiveToSendClient(){return mIsActiveToSendClient;}
	void setActiveToSendClient(bool mIsActiveToSendClient){mIsActiveToSendClient=mIsActiveToSendClient;}

private:

};

#endif