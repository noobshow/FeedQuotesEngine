#if !defined SettingsController_H
#define SettingsController_H

#include "SymbolValues.h"
#include "../Models/SettingsVariableModel.h"
#include "../MainThreadArgument.h"
#include "../Models/FeedSourceStatusModel.h"

class SettingsController
{
private:
	MainThreadArgument				*mMainThreadArgument;

	list<SymbolValues*>				mAvailableSymbolsToHandle;
	SettingsVariableModel			mSettingsVariableModel;
	list<FeedSourceStatusModel*>	mAvailableFeedProviders;
public:
	SettingsController(MainThreadArgument	*aMainThreadArgument);
	~SettingsController();

	list<SymbolValues*> getAvailableSymbol(){return mAvailableSymbolsToHandle;};
	list<FeedSourceStatusModel*> getAvailableFeedProviders(){return mAvailableFeedProviders;};

	//---
	void loadAvailableFeedProviders(); 
	void updateSymbolSpikeMappingInfo();
	void updateSymbolSourceMappingModel();

private:
	void loadAvailableSymbolInfo();
	void loadSymbolStatusInfo(list<SymbolValues*> aAvailableSymbolsToHandle);
	void loadSymbolSpikeMappingInfo(list<SymbolValues*> aAvailableSymbolsToHandle);
	void loadSymbolOutTexts(list<SymbolValues*> aAvailableSymbolsToHandle);
	void loadSymbolSourceMappingModel(list<SymbolValues*> aAvailableSymbolsToHandle);


	//--
	


};

#endif