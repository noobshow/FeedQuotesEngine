#if !defined FeedSourceStatusModel_H
#define FeedSourceStatusModel_H

#include "../stdafx.h"
#include "FeedProviderModel.h"
class FeedSourceStatusModel : public FeedProviderModel
{
private:
	int			mAutoIncrimentID;
	int			mFeedProviderID;
	bool		mIsEnabled;
	bool		mIsActiveToSendMT4;
	bool		mIsActiveToSendClient;

public:
	FeedSourceStatusModel();
	~FeedSourceStatusModel();

	int getAutoIncrimentID(){return mAutoIncrimentID;}
	void setAutoIncrimentID(int aAutoIncrimentID){mAutoIncrimentID=aAutoIncrimentID;}

	int getFeedProviderID(){return mFeedProviderID;}
	void setFeedProviderID(int aFeedProviderID){mFeedProviderID=aFeedProviderID;}

	bool isEnabled(){return mIsEnabled;}
	void setEnable(bool aIsEnable){mIsEnabled=aIsEnable;}

	bool isActiveToSendMT4(){return mIsActiveToSendMT4;}
	void setActiveToSendMT4(bool aIsActiveToSendMT4){mIsActiveToSendMT4=aIsActiveToSendMT4;}

	bool isActiveToSendClient(){return mIsActiveToSendClient;}
	void setActiveToSendClient(bool mIsActiveToSendClient){mIsActiveToSendClient=mIsActiveToSendClient;}

private:

};

#endif