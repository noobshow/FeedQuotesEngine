#if !defined SymbolSourceMappingModel_H
#define SymbolSourceMappingModel_H

#include "../stdafx.h"

class SymbolSourceMappingModel
{
private:
	int			mAutoIncrimentID;
	int			mSymbolID;
	int			mFeedProviderID;
public:
	SymbolSourceMappingModel();
	~SymbolSourceMappingModel();

	int getAutoIncrimentID(){return mAutoIncrimentID;}
	void setAutoIncrimentID(int aAutoIncrimentID){mAutoIncrimentID=aAutoIncrimentID;}

	int getSymbolID(){return mSymbolID;}
	void setSymbolID(int aSymbolID){mSymbolID=aSymbolID;}

	int getFeedProviderID(){return mFeedProviderID;}
	void setFeedProviderID(int aFeedProviderID){mFeedProviderID=aFeedProviderID;}

private:

};



#endif