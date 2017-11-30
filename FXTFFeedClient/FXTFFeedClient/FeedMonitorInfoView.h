#if !defined FeedMonitorInfoView_H
#define FeedMonitorInfoView_H

#include <windows.h>
#include <commctrl.h>
#include <iostream>
#include <string.h>
#include "FeedClientDBHandle\DBOperationController.h"
#include "FeedClientCommon\SingletonFeedClient.h"

class FeedMonitorInfoView
{
	HWND mHandleWindowFeedMonitorInfoViewParentWindow;
	DBOperationController *mDBOperationController;
	list<SymbolModel> mListDataFromDB;
	list<SymbolOutModel> mListSymbolOutModel;
	bool isGUIInitialized;
	LVITEM mFeedMonitorLVITEM;			   // ListView Item struct

public:
	
	HWND mHandleWindowFeedMonitorListView;
	FeedMonitorInfoView();
	~FeedMonitorInfoView();
	bool Initialize(HWND hParentWnd);
	bool DrawFeedMonitorListViewBody();
	bool FillRowFeedMonitorListView();

	void receiveQuotesMessage(string aStrQuotes); 

	int findSymbolDigit(int aIntSymbolIndex);
	void updateListViewItemAccordingToSymbol(string aStrQuotes);
	int findSymbolIndexInListView(string aStrQuotes);
	vector<string> splitStringQuotesMessage(string aInputMessage);
	void updateListViewItems(int aIndex,int aSubIndex,string aStrValue,int aColorCode);
	int checkCurrentRateStatus(int aiItem, int aiSubItem, string aRate);
	LONG SetColor(int aColorX,int aColorY ,int aColorZ);

	double mPreviousBidValue;
	double mCurrentBidValue;
	double mPreviousAskValue;
	double mCurrentAskValue;

	int mDefaultColorCode;
	int mGreenColorCode;
	int mRedColorColde;
};

#endif