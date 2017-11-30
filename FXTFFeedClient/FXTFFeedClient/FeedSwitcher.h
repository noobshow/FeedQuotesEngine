#if !defined FeedSwitcher_H
#define FeedSwitcher_H

#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include "tchar.h"
#include <iostream>
#include "resource.h"
#include "FeedClientCommon/FeedClientConstant.h"
#include "FeedClientCommon/SingletonFeedClient.h"
#include "FeedClientDBHandle/DBOperationController.h"
#include "DBHandle/MySQLDBOperation.h"
#include "FeedClientSocketController.h"

using namespace std;

class FeedSwitcher{
private:
	DBOperationController *mDBOperationController;

public:
	FeedSwitcher();;
	~FeedSwitcher();

	void DisplayFeedSwitcherWindow(HINSTANCE);
	bool FillAndDisplayListViewItems(HWND hWnd);
};

#endif