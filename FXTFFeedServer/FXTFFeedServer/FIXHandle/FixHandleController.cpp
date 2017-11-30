#include "../stdafx.h"
#include "FixHandleController.h"

FixHandleController::FixHandleController(MainThreadArgument* aMainThreadArgument )
{
	mMainThreadArgument=aMainThreadArgument;
	aMainThreadArgument->setFixHandleController(this);
	mFXCMFixController=NULL;
	mFXCMUSFixController=NULL;
	mOCBCFixController=NULL;
	LoadFixHandleSettings();
	StartFixProviders();
}


FixHandleController::~FixHandleController(void)
{
	delete mFXCMFixController;
	delete mFXCMUSFixController;
	delete mOCBCFixController;
}

void FixHandleController::LoadFixHandleSettings()
{
	if (mMainThreadArgument->getSettingsController()->getAvailableFeedProviders().size()==0)
	{
		mMainThreadArgument->getSettingsController()->loadAvailableFeedProviders();
		mAvailableFeedProviders= mMainThreadArgument->getSettingsController()->getAvailableFeedProviders();
	}else
	{
		mAvailableFeedProviders= mMainThreadArgument->getSettingsController()->getAvailableFeedProviders();
	}
}

bool FixHandleController::CheckSettingsForFXCMProvider()
{
	//Setting will be check here
	return getFeedProviderEnableStatus("FXCM");
}

bool FixHandleController::CheckSettingsForOCBCProvider()
{
	//Setting will be check here
	return getFeedProviderEnableStatus("OCBC");
}

bool FixHandleController::CheckSettingsForFXCMUSProvider()
{
	//Setting will be check here
	return getFeedProviderEnableStatus("FXCM-US");
}

void FixHandleController::StartFixProviders()
{
	// Load Fix Handeling Settings
	if (CheckSettingsForFXCMProvider())
	{
		StartFIXProviderFXCM();
	}

	if (CheckSettingsForOCBCProvider())
	{
		StartFIXProviderOCBC();
	}

	if (CheckSettingsForFXCMUSProvider())
	{
		StartFIXProviderFXCMUS();
	}

}

void FixHandleController::StartFIXProviderFXCM()
{
	cout << "******************* Starting FXCM Feed provider *******************"<<endl;
	mFXCMFixController=new FXCMFixController(mMainThreadArgument);
	mFXCMFixController->setFeedProviderID(getFeedProviderID("FXCM"));
	mFXCMFixController->initializaFIXConnection();
}

void FixHandleController::StartFIXProviderOCBC()
{
	cout << "******************* Starting OCBC Feed provider *******************"<<endl;
	mOCBCFixController=new OCBCFixController(mMainThreadArgument);
	mOCBCFixController->setFeedProviderID(getFeedProviderID("OCBC"));
	mOCBCFixController->initializaFIXConnection();
}

void FixHandleController::StartFIXProviderFXCMUS()
{
	cout << "******************* Starting FXCM-US Feed provider *******************"<<endl;
	mFXCMUSFixController=new FXCMUSFixController(mMainThreadArgument);
	mFXCMUSFixController->setFeedProviderID(getFeedProviderID("FXCM-US"));
	mFXCMUSFixController->initializaFIXConnection();
}

bool FixHandleController::getFeedProviderEnableStatus(string aParamValue)
{
	for(std::list<FeedSourceStatusModel*>::iterator list_iter = mAvailableFeedProviders.begin(); list_iter != mAvailableFeedProviders.end(); list_iter++)
	{
		FeedSourceStatusModel* lFeedSourceStatusModel=(FeedSourceStatusModel*)*list_iter;
		if (lFeedSourceStatusModel->isEnabled())
		{
			if(lFeedSourceStatusModel->getFeedProviderName().compare(aParamValue)==0)
			{
				return true;
			}
		}
	}
	return false;
}

int FixHandleController::getFeedProviderID(string aParamValue)
{
	for(std::list<FeedSourceStatusModel*>::iterator list_iter = mAvailableFeedProviders.begin(); list_iter != mAvailableFeedProviders.end(); list_iter++)
	{
		FeedSourceStatusModel* lFeedSourceStatusModel=(FeedSourceStatusModel*)*list_iter;
		if (lFeedSourceStatusModel->isEnabled())
		{
			if(lFeedSourceStatusModel->getFeedProviderName().compare(aParamValue)==0)
			{
				return lFeedSourceStatusModel->getFeedProviderID();
			}
		}
	}
	return -1;
}