#if !defined FilterProcessController_H
#define FilterProcessController_H

#include "../MainThreadArgument.h"
#include "../ClientHandle/ClientController.h"
#include "../MT4Handle/MT4Controller.h"
#include "../Settings/SettingsController.h"
#include "../Settings/SymbolValues.h"
#include "SendingThreadHandler.h"
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <mutex>
#include <list>
#include <algorithm>

using namespace std;

class FilterProcessController
{
private:
	//Members
	MainThreadArgument* mMainThreadArgument;
	mutex				mSpikeAndSendingThreadSafe;
	bool				mIsSendingToMT4;
	bool				mIsSendingToClient;
	list<string>		mMT4Ticks;
	vector<string>		mClientTicks;
public:
	mutex				mClientSendingThreadLock;
private:
	//Function
	void LoadSetting();
	void FilterFeedData(SymbolValues *aSymbolValue,SymbolSourceModel* aSymbolSourceModel);
	string FilterEngine(string aFeedProvide,string aFeedMessage);

	bool CheckSettingForFXCM();
	bool CheckSettingForOCBC();
	bool CheckSettingForPQE();

	//
	void SplitProviderAndFeed(char* apFeedProvider,char* apFeedMessage,string aSorceString);
	void initializeCharArray(char* aCharArray,int aLength);
	// May Need more Function to

	void sendToClientUnfilterData(SymbolSourceModel* aSymbolSourceModel);
	SymbolValues* getSymbolValueForFeedSymbol(SymbolSourceModel* aSymbolSourceModel);
	void updateAvailableValuesForTickSend(SymbolValues* aSymbolValues);

	// --
	void createSendingThread();
	static DWORD WINAPI sendingToMT4Thread(LPVOID  aThreadData);
	static DWORD WINAPI sendingToClientThread(LPVOID aThreadData);

public:
	FilterProcessController(MainThreadArgument* aMainThreadArgument);
	~FilterProcessController();
	
	//Settings Load request
	void RequestToLoadSettingFromAdminClient(int aRequestCondition); // We will not load all setting for any request from Clien. We will load only Respective area. 


	//Data From Feed Provider, When we add New Provider New Function Will be added here
	void FeedDataFromFXCM(SymbolSourceModel *aSymbolSourceModel); //This is Only For Test Purpose
	void FeedDataFromOCBC(SymbolSourceModel *aSymbolSourceModel); 
	void FeedDataFromPQE(SymbolSourceModel *aSymbolSourceModel); 
	void FeedDataFromFXCMUS(SymbolSourceModel *aSymbolSourceModel);

	//Sending Data------- May Need Improove when Myltiple Provider and Multiple Client
	void SendDataToClientForMonitor(string aFeedMessage);
	void SendDataToMT4Server(string aFeedMessage);

	//--
	void testMutexImplimentetion();

	bool IsSendingToMT4(){return mIsSendingToMT4;}
	void setSendingToMT4Status(bool aIsSendingToMT4){mIsSendingToMT4=aIsSendingToMT4;}
	
	bool IsSendingToClient(){return mIsSendingToClient;}
	void setSendingToClientStatus(bool aIsSendingToClient){mIsSendingToClient=aIsSendingToClient;}


	list<string> getAvailableTicksToSend(){return mMT4Ticks;};
	void ClearItems(){ mMT4Ticks.clear();};
	void RemoveItems(string aValueItem){ mMT4Ticks.remove(aValueItem);};

	vector<string> getAvailableTicksToSendClient(){return mClientTicks;};
	int getSizeClientTick(){return mClientTicks.size();};
	std::vector<string>::iterator RemoveItemsClient(std::vector<string>::iterator aValueItem){ 
		return mClientTicks.erase(aValueItem);
	};
	std::vector<string>::iterator getBeginClient(){return mClientTicks.begin();}
	std::vector<string>::iterator getEndClient(){return mClientTicks.end();}
	std::vector<string>::iterator setPrevClient(std::vector<string>::iterator aValueItem){return std::prev(aValueItem);}
	void clearClientTickItems(){mClientTicks.clear();}

private:

};

#endif