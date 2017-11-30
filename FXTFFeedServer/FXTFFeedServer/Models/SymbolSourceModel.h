#if !defined SymbolSourceModel_H
#define SymbolSourceModel_H

#include "../stdafx.h"
#include <string>
#include <stdio.h>

using namespace std;

class SymbolSourceModel
{
private:
	int			mFeedProviderID;
	int			mSymbolID;
	string		mFeedSymbol;
	double		mBidValue;
	double		mAskValue;

public:
	SymbolSourceModel();
	~SymbolSourceModel();
	
	int getFeedProviderID(){return mFeedProviderID;}
	void setFeedProviderID(int aFeedProviderID){mFeedProviderID=aFeedProviderID;}

	int getSymbolID(){return mSymbolID;}
	void setSymbolID(int aSymbolID){mSymbolID=aSymbolID;}

	string getFeedSymbol(){return mFeedSymbol;}
	void setFeedSymbol(string aFeedSymbol){mFeedSymbol.assign(aFeedSymbol);}

	double getBidValue(){return mBidValue;}
	void setBidValue(double aBidValue){mBidValue=aBidValue;}

	double getAskValue(){return mAskValue;}
	void setAskValue(double aAskValue){mAskValue=aAskValue;}

	double getMidValue(){return ((mAskValue+mBidValue)/2);}

private:

};

#endif