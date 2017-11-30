#if !defined FixHandleController_H
#define FixHandleController_H

#include "../MainThreadArgument.h"
#include "FXCMFixController.h"
#include "OCBCFixController.h"
#include "FXCMUSFixController.h"
#include "../Settings/SettingsController.h"

class FixHandleController
{
private:
	//Members
	MainThreadArgument*		mMainThreadArgument;  
	FXCMFixController*		mFXCMFixController;
	FXCMUSFixController*    mFXCMUSFixController;
	OCBCFixController*		mOCBCFixController;
	list<FeedSourceStatusModel*>	mAvailableFeedProviders;
private:
	//Functions
	void LoadFixHandleSettings();
	void StartFixProviders();
	void StartFIXProviderFXCM();
	void StartFIXProviderOCBC();
	void StartFIXProviderFXCMUS();

	bool CheckSettingsForFXCMProvider();
	bool CheckSettingsForOCBCProvider();
	bool CheckSettingsForFXCMUSProvider();
	//--
	bool getFeedProviderEnableStatus(string aParamValue);
	int getFeedProviderID(string aParamValue);

	//--

public:
	//Members
	void FixHandleSettingsUpdateRequest(int aRequestCondition);

public:
	FixHandleController(MainThreadArgument*  aMainThreadArgument);
	~FixHandleController();
};

#endif
