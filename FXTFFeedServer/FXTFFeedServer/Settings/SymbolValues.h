#if !defined SymbolValues_H
#define SymbolValues_H

#include "../stdafx.h"
#include <string>
#include <stdio.h>
#include <list>
#include <iostream>
#include <assert.h>

//-- Include Classes


#include "../Models/SymbolModel.h"
#include "../Models/SymbolOutModel.h"
#include "../Models/SymbolSourceMappingModel.h"
#include "../Models/SymbolStatusModel.h"
#include "../Models/SymbolSpikeMappingModel.h"
#include "../Models/SymbolSourceModel.h"
#include "../Common/FXTFLog.h"


class SymbolModel;
class SymbolStatusModel;
class SymbolOutModel;
class SymbolSourceMappingModel;
class SymbolSpikeMappingModel;
class SymbolSourceModel;

using namespace std;

class SymbolValues : public SymbolModel
{
private:
	int									mSpikedCount;
private:
	SymbolStatusModel					*mSymbolStatusModel;
	SymbolSpikeMappingModel				*mSymbolSpikeMapping;
	list<SymbolOutModel*>				 mSymbolOutTexts;
	list<SymbolSourceMappingModel*>		 mSymbolSourceModels;
	SymbolSourceModel					*mPreviousSymbolTick;
	SymbolSourceModel					*mCurrentSymbolTick;
	//SymbolSourceModel					*mCurrentSymbolRawData;

private:
	void checkBidAskValue(SymbolSourceModel *aSymbolSourceModel ,FXTFLog *aLogWriter);

public:
	SymbolValues();
	~SymbolValues();
	//--
	bool checkIsSpiked(FXTFLog* aLogWriter,SymbolSourceModel* aSymbolSourceModel);
	double calculatePercentage(double aPrevValue, double aCurrentValue);

	string valueOutInCaseOfSpike();
	string valueOutforMT4();

	//--
	SymbolStatusModel* getSymboStatusModel(){return mSymbolStatusModel;}
	void setSymbolStatusModel(SymbolStatusModel* aSymbolStatusModel){mSymbolStatusModel=aSymbolStatusModel;}

	SymbolSpikeMappingModel* getSymbolSpikeMappingModel(){return mSymbolSpikeMapping;}
	void setSymbolSpikeMapping(SymbolSpikeMappingModel* aSymbolSpikeMappingModel){mSymbolSpikeMapping=aSymbolSpikeMappingModel;}

	list<SymbolOutModel*> getSymbolOutTexts(){return mSymbolOutTexts;}
	void setSymbolOutTexts(list<SymbolOutModel*> aSymbolOutTexts){mSymbolOutTexts=aSymbolOutTexts;}
	void insertItemToSymbolOutTexts(SymbolOutModel* aSymbolOutModel);

	list<SymbolSourceMappingModel*> getSymbolSourceModels(){return mSymbolSourceModels;}
	void setSymbolSourceModels(list<SymbolSourceMappingModel*> aSymbolSourceModels){mSymbolSourceModels=aSymbolSourceModels;}
	void insertItemToSymbolSourceModels(SymbolSourceMappingModel* aSymbolSourceMapping);

//	SymbolSourceModel* getRawSymbolSourceModel(){return mCurrentSymbolRawData;}
//	void setRawSymbolSourceModel(SymbolSourceModel* aRawSymbolSourceModel){mCurrentSymbolRawData=aRawSymbolSourceModel;}

	SymbolSourceModel* getCurrentSymbolSourceModel(){return mCurrentSymbolTick;}
	void setCurrentSymbolSourceModel(SymbolSourceModel* aCurrentSymbolSourceModel){mCurrentSymbolTick=aCurrentSymbolSourceModel;}

	SymbolSourceModel* getPriviousSymbolSourceModel(){return mPreviousSymbolTick;}
	void setPriviousSymbolSourceModel(SymbolSourceModel* aPriviousSymbolSourceModel){mPreviousSymbolTick=aPriviousSymbolSourceModel;}

private:
	string getFeedProviderName();
};

#endif