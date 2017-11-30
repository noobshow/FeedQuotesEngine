#if !defined SymbolSpikeMappingView_H
#define SymbolSpikeMappingView_H

#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include "tchar.h"
#include <iostream>
#include "resource.h"
#include "FeedClientCommon/FeedClientConstant.h"
#include "FeedClientCommon\SingletonFeedClient.h"
#include "FeedClientDBHandle\DBOperationController.h"

#include "DBHandle\MySQLDBOperation.h"
//#include "Models\SymbolSpikeMappingModel.h"
//#include "FeedClientDBHandle\DBOperationController.h"
//#include "FeedClientCommon\SingletonFeedClient.h"
#include "FeedClientSocketController.h"

#include "Common\Constant.h"

using namespace std;

class SymbolSpikeMappingView
{
private:
	DBOperationController *mDBOperationController;

public:
	SymbolSpikeMappingView();
	~SymbolSpikeMappingView();

	void DisplaySymbolSpikeMappingViewWindow(HINSTANCE);
	bool FillAndDisplayListViewSymbolSpikeMapping(HWND hWnd);
	void InitialFillAndDisplayListViewSymbolSpikeMapping(HWND hWnd);
	int  SpikeProtectionSettings(string CSVFileData);

};

#endif