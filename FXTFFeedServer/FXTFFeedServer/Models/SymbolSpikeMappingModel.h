#if !defined SymbolSpikeMappingModel_H
#define SymbolSpikeMappingModel_H

#include "../stdafx.h"

class SymbolSpikeMappingModel
{
private:
	int			mAutoIncrimentID;
	int			mSymbolID;
	double		mTickValueChangePercent;
	int			mTickValueCount;
	int			mPipsValue;

public:
	SymbolSpikeMappingModel();
	~SymbolSpikeMappingModel();

	int getAutoIncrimentID(){return mAutoIncrimentID;}
	void setAutoIncrimentID(int aAutoIncrimentID){mAutoIncrimentID=aAutoIncrimentID;}

	int getSymbolID(){return mSymbolID;}
	void setSymbolID(int aSymbolID){mSymbolID=aSymbolID;}

	double getTickValueChangePercent(){return mTickValueChangePercent;}
	void setTickValueChangePercent(double aTickValueChangePercent){mTickValueChangePercent=aTickValueChangePercent;}

	int getTickValueCount(){return mTickValueCount;}
	void setTickValueCount(int aTickValueCount){mTickValueCount=aTickValueCount;}

	int getPipsValue(){return mPipsValue;}
	void setPipsValue(int aPipsValue){mPipsValue=aPipsValue;}

private:

};

#endif