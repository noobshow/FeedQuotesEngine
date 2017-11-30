#if !defined FeedMonitorSymbolModel_H
#define FeedMonitorSymbolModel_H

#include "stdafx.h"
#include <string>
#include <stdio.h>

using namespace std;

class FeedMonitorSymbolModel
{
private:
	int			mFeedMonitorProviderID;
	string		mFeedMonitorSymbol;
	double		mFeedMonitorBidValue;
	double		mFeedMonitorAskValue;
public:
	FeedMonitorSymbolModel();
	~FeedMonitorSymbolModel();

	int getFeedMonitorProviderID(){return mFeedMonitorProviderID;}
	void setFeedMonitorProviderID(int aFeedMonitorProviderID){mFeedMonitorProviderID=aFeedMonitorProviderID;}
	
	string getFeedMonitorSymbol(){return mFeedMonitorSymbol;}
	void setFeedMonitorSymbol(string aFeedMonitorSymbol){mFeedMonitorSymbol=aFeedMonitorSymbol;}
	
	double getFeedMonitorBidValue(){return mFeedMonitorBidValue;}
	void setFeedMonitorBidValue(double aFeedMonitorBidValue){mFeedMonitorBidValue=aFeedMonitorBidValue;}
	
	double getFeedMonitorAskValue(){return mFeedMonitorAskValue;}
	void setFeedMonitorAskValue(double aFeedMonitorAskValue){mFeedMonitorAskValue=aFeedMonitorAskValue;}

};

#endif