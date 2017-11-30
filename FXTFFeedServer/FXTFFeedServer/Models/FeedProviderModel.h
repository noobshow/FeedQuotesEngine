#if !defined FeedProviderModel_H
#define FeedProviderModel_H

#include "../stdafx.h"
#include <string>
#include <stdio.h>

using namespace std;

class FeedProviderModel
{
private:
	int			mAutoIncrimentID;
	string		mFeedProviderName;
	string		mDescription;

public:
	FeedProviderModel();
	~FeedProviderModel();

	int getAutoIncrimentID(){return mAutoIncrimentID;}
	void setAutoIncrimentID(int aAutoIncrimentID){mAutoIncrimentID=aAutoIncrimentID;}

	string getFeedProviderName(){return mFeedProviderName;}
	void setFeedProviderName(string aFeedProviderName){mFeedProviderName.assign(aFeedProviderName);}

	string getDescription(){return mDescription;}
	void setDescription(string aDescription){mDescription.assign(aDescription);}

private:

};

#endif