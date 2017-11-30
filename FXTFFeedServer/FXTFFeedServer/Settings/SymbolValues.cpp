
#include "SymbolValues.h"


SymbolValues::SymbolValues()
{
	mSpikedCount=0;
	mSymbolStatusModel=NULL;
	mSymbolSpikeMapping=NULL;
	mPreviousSymbolTick=NULL;
	mCurrentSymbolTick=NULL;
	//mCurrentSymbolRawData=NULL;
}

SymbolValues::~SymbolValues()
{
}

void SymbolValues::insertItemToSymbolOutTexts(SymbolOutModel* aSymbolOutModel)
{
	mSymbolOutTexts.push_back(aSymbolOutModel);
	int k=0;
}

void SymbolValues::insertItemToSymbolSourceModels(SymbolSourceMappingModel* aSymbolSourceMapping)
{
	mSymbolSourceModels.push_back(aSymbolSourceMapping);
	int k=0;
}

bool SymbolValues::checkIsSpiked(FXTFLog* aLogWriter,SymbolSourceModel* aSymbolSourceModel)
{
	checkBidAskValue(aSymbolSourceModel,aLogWriter);
	//aLogWriter->LogOutLn("TICK: Provider ID :"+to_string(aSymbolSourceModel->getFeedProviderID())+" Symbol :"+aSymbolSourceModel->getFeedSymbol()+" Bid :"+to_string(aSymbolSourceModel->getBidValue())+" Ask : "+to_string(aSymbolSourceModel->getAskValue()));
	bool lIsSpiked=false;
	bool lReturned=false;
	if (mPreviousSymbolTick==NULL)
	{
		assert(aSymbolSourceModel!=NULL);
		mPreviousSymbolTick=new SymbolSourceModel(*aSymbolSourceModel);
		mCurrentSymbolTick=mPreviousSymbolTick;
		lReturned=false;
	}
	
	if (!aSymbolSourceModel)
	{
		aLogWriter->LogOutLn("ERROR: Something wrong source Feed Not Initialized");
		return true;
	}
	assert(aSymbolSourceModel!=NULL);
	double lTickValueChangedPercentMid=calculatePercentage(mPreviousSymbolTick->getMidValue(),aSymbolSourceModel->getMidValue());

	//cout <<"Mid Value Changed : "<<lTickValueChangedPercentMid<<endl;
	//aLogWriter->LogOutLn("INFO: Mid Changed : Symbol : "+getSymbolName()+" Value :"+ to_string(lTickValueChangedPercentMid)+"%"+ " Prev : "+to_string(mPreviousSymbolTick->getMidValue())+" Current "+to_string(aSymbolSourceModel->getMidValue()));
	
	if (lTickValueChangedPercentMid>=mSymbolSpikeMapping->getTickValueChangePercent())
	{
		mSpikedCount++;
		lIsSpiked=true;
	}else
	{
		mSpikedCount=0;
		if (mPreviousSymbolTick)
		{
			delete mPreviousSymbolTick;
			mPreviousSymbolTick=NULL;
		}
		if (aSymbolSourceModel)
		{
			try
			{
				assert(aSymbolSourceModel!=NULL);
				mPreviousSymbolTick=new SymbolSourceModel(*aSymbolSourceModel);
				mCurrentSymbolTick=mPreviousSymbolTick;
				lReturned= false;
			}catch (bad_alloc& badAlloc)
			  {
				cout << "!!!!!!!!!!!!bad_alloc caught, not enough memory: " << badAlloc.what() << endl;
				aLogWriter->LogOutLn("ERROR :bad_alloc caught, not enough memory");
				lReturned= true;
			  }	
		}
	}

	if (lIsSpiked)
	{
		if (mSpikedCount>mSymbolSpikeMapping->getTickValueCount())
		{
			mSpikedCount=0;
			if (mPreviousSymbolTick)
			{
				delete mPreviousSymbolTick;
				mPreviousSymbolTick=NULL;
			}
			
			try
			{
				assert(aSymbolSourceModel!=NULL);
				mPreviousSymbolTick=new SymbolSourceModel(*aSymbolSourceModel);
				mCurrentSymbolTick=mPreviousSymbolTick;
				lReturned= false;
			}
			catch (bad_alloc& badAlloc)
			{
				cout << "!!!!!!!!!!!!bad_alloc caught, not enough memory: " << badAlloc.what() << endl;
				aLogWriter->LogOutLn("ERROR :bad_alloc caught, not enough memory");
				lReturned= true;
			}
		}else
		{
			lReturned=true;
		}
	}

	if (lReturned)
	{
		if (aSymbolSourceModel)
		{
			//cout << "Spiked Tick :" <<to_string(aSymbolSourceModel->getFeedProviderID())+" "+aSymbolSourceModel->getFeedSymbol()+" "+to_string(aSymbolSourceModel->getBidValue())+" "+to_string(aSymbolSourceModel->getAskValue())<<endl;
			aLogWriter->LogOutLn("SPIKED : Provider ID : "+to_string(aSymbolSourceModel->getFeedProviderID())+" "+aSymbolSourceModel->getFeedSymbol()+" "+to_string(aSymbolSourceModel->getBidValue())+" "+to_string(aSymbolSourceModel->getAskValue())+" Changed : "+to_string(lTickValueChangedPercentMid)+"%");
		}
	}
		
	if (aSymbolSourceModel)
	{
		delete aSymbolSourceModel;
		aSymbolSourceModel=NULL;
	}	
	return lReturned;
}

void SymbolValues::checkBidAskValue(SymbolSourceModel *aSymbolSourceModel, FXTFLog *aLogWriter)
{
	if (aSymbolSourceModel->getBidValue()>aSymbolSourceModel->getAskValue())
	{
		aLogWriter->LogOutLn("Negetive Spread : Swapping BID and ASK Value : "+aSymbolSourceModel->getFeedSymbol());

		double  lAskValue=aSymbolSourceModel->getAskValue();
		aSymbolSourceModel->setAskValue(aSymbolSourceModel->getBidValue());
		aSymbolSourceModel->setBidValue(lAskValue);
	}
}

//http://ncalculators.com/number-conversion/percentage-difference-calculator.htm
double SymbolValues::calculatePercentage(double aPrevValue, double aCurrentValue)
{
	return fabs(((aPrevValue-aCurrentValue)/((aPrevValue+aCurrentValue)/2))*100);
}

string SymbolValues::valueOutInCaseOfSpike()
{
	return "";
}

string SymbolValues::valueOutforMT4()
{
	string lstrSendString="";
	for(std::list<SymbolOutModel*>::iterator list_iter1 = mSymbolOutTexts.begin(); list_iter1 != mSymbolOutTexts.end(); list_iter1++)
	{
		SymbolOutModel* lSymbolOutModel=(SymbolOutModel*)*list_iter1;
		lstrSendString+=lSymbolOutModel->getSymbolOutText()+" 0 "+to_string(mCurrentSymbolTick->getBidValue())+" "+to_string(mCurrentSymbolTick->getAskValue())+"\n\r";
	}
	return lstrSendString;
}

string SymbolValues::getFeedProviderName()
{
	return "";
}