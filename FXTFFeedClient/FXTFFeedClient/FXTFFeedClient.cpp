// FXTFFeedClient.cpp : Defines the entry point for the application.
//
#define WIN32_LEAN_AND_MEAN

#include "FeedClientSocketController.h"
#include "FXTFFeedClient.h"

#define MAX_LOADSTRING 100

HINSTANCE					hInst;									// current instance
TCHAR						szTitle[MAX_LOADSTRING];				// The title bar text
TCHAR						szWindowClass[MAX_LOADSTRING];			// the main window class name

ATOM						MyRegisterClass(HINSTANCE hInstance);
BOOL						InitInstance(HINSTANCE, int);
LRESULT CALLBACK			WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK			About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK			Login(HWND, UINT, WPARAM, LPARAM);

FeedMonitorInfo				*g_ExtFeedMonitorInfo					= NULL;
HANDLE						g_ExtHandleFeedMonitorInfo				= NULL;
HWND						g_ExtHwndFeedMonitorInfo				= NULL; //This stores the MDI client area window handle

FeedSwitcher				*mFeedSwitcher							= NULL;
SymbolSpikeMappingView		*mSymbolSpikeMappingView				= NULL;
UsersModel					*mUsersModel							= NULL;

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR  lpCmdLine, _In_ int  nCmdShow)
{
	hInst=hInstance;
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	ConfigFileRead lConfigFileRead(INI_CONFIG_FILE_NAME);
	string mDBServerIPAddress	= lConfigFileRead.Get(INI_DATABASE_CONFIG_NAME,INI_DB_SERVER_IP_ADDRESS,"");
	string mDBUserName			= lConfigFileRead.Get(INI_DATABASE_CONFIG_NAME,INI_DB_USER_NAME,"");
	string mDBUserPassword		= lConfigFileRead.Get(INI_DATABASE_CONFIG_NAME,INI_DB_USER_PASSWORD,"");
	string mTargetDatabase		= lConfigFileRead.Get(INI_DATABASE_CONFIG_NAME,INI_DB_SCHEMA_NAME,"");

	FXTFLog mLogWriter;
	if (mLogWriter.CommonLogInit())
	{
		mLogWriter.Enable();
		mLogWriter.GetTimeStr();
		mLogWriter.LogOut("");
	};
	SingletonFeedClient::GetInstance()->setFXTFLog(&mLogWriter);
	SingletonFeedClient::GetInstance()->setMySQLDBOperation(new MySQLDBOperation(mDBServerIPAddress, mDBUserName, mDBUserPassword, mTargetDatabase));

	MSG msg;
	HACCEL hAccelTable;

	int loginValidation = DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_LOGIN), NULL, Login);

	if ( loginValidation )
	{
		SingletonFeedClient::GetInstance()->setUsersModel(mUsersModel);
		SingletonFeedClient::GetInstance()->setFeedMonitorInfo(NULL);
		SingletonFeedClient::GetInstance()->setFeedClientSocketController(new FeedClientSocketController());
	}

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_FXTFFEEDCLIENT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FXTFFEEDCLIENT));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_FXTF));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_FXTFFEEDCLIENT);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON_FXTF));

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	CLIENTCREATESTRUCT MDIClientCreateStruct; // Structure to be used for MDI client area (Feed Monitor)
	switch (message)
	{
	case WM_SIZE:
		// Redraw controls
		if(NULL != g_ExtHwndFeedMonitorInfo)
		{
			RECT rcClient;
			GetClientRect(hWnd, &rcClient);
			MoveWindow(g_ExtHwndFeedMonitorInfo, 
				0, 
				0, 
				rcClient.right, //width
				rcClient.bottom,
				//rcClient.bottom - giStatusBarHeight, //height
				true);
		}
		return 0; //return from here because if DefFrameProc is called, it would fill MDICLIENT fully
	case WM_CREATE:
		MDIClientCreateStruct.hWindowMenu	= NULL;
		MDIClientCreateStruct.idFirstChild	= 100000;
		g_ExtHwndFeedMonitorInfo = CreateWindow(TEXT("MDICLIENT"), // predefined value for MDI client area
									NULL, // no caption required
									WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE,
									0, // No need to give any x/y or height/width since this client
									0, 
									0,
									0,
									hWnd,
									NULL,
									hInst,
									(void *) &MDIClientCreateStruct);
		return 0;	
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_USERVIEW_FEEDMONITOR:
			{			
				if(NULL != g_ExtHandleFeedMonitorInfo)
				{
					return 0; 				// return if mutex is already created.
				}

				g_ExtHandleFeedMonitorInfo = CreateMutex(NULL, false, NULL);			
				g_ExtFeedMonitorInfo = new FeedMonitorInfo();  //Create the FeedMonitor Information Data and View
				if(g_ExtFeedMonitorInfo->CreateFeedMonitorInfoWindow())
				{
					g_ExtFeedMonitorInfo->InitializeViewAndData(); // If window is created then try to initialize view and show assiciated data

					SingletonFeedClient::GetInstance()->setFeedMonitorInfo(g_ExtFeedMonitorInfo);
				}
				else
				{
					delete g_ExtFeedMonitorInfo; // delete and reset to NULL if its not possible to create View and Data
					g_ExtFeedMonitorInfo = NULL;
				}
			}
			//break;
			return 0;
		case ID_USERVIEW_FEEDSWITCHER:
			{
			//MessageBox(hWnd,"Under Construntion.......\nThis feature will be integrated near future.\nThank you for your patient.","USERVIEW FEEDSWITCHER",MB_OK|MB_ICONINFORMATION);
			mFeedSwitcher = new FeedSwitcher();
			mFeedSwitcher->DisplayFeedSwitcherWindow(hInst);
			delete mFeedSwitcher;
			
			}
			break;
		case ID_ADMINSETTINGS_FEEDER:
			{
				MessageBox(hWnd,"Under Construntion.......\nThis feature will be integrated near future.\nThank you for your patient.","ADMIN SETTINGS FEEDER",MB_OK|MB_ICONINFORMATION);
			}
			break;
		case ID_ADMINSETTINGS_SPIKE:
			mSymbolSpikeMappingView = new SymbolSpikeMappingView();
			mSymbolSpikeMappingView->DisplaySymbolSpikeMappingViewWindow(hInst);
			break;
		case ID_ADMINSETTINGS_SYMBOL:
			MessageBox(hWnd,"Under Construntion.......\nThis feature will be integrated near future.\nThank you for your patient.","ADMIN SETTINGS SYMBOL",MB_OK|MB_ICONINFORMATION);
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefFrameProc(hWnd, g_ExtHwndFeedMonitorInfo, message, wParam, lParam); //Frame window calls DefFrameProc rather than DefWindowProc
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK Login(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	DBOperationController *lDBOperationController = new DBOperationController();

	//static HBRUSH lBackgroundColorLogin = CreateSolidBrush( RGB( 0, 60, 117 ) );
	//static HBRUSH lBackgroundColorLogin = CreateSolidBrush( RGB( 255, 255, 117 ) );
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;
	//case WM_CTLCOLORDLG:
	//	return (INT_PTR) lBackgroundColorLogin;
	//case WM_NCPAINT:
	//	return (INT_PTR) lBackgroundColorLogin;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK )
		{
			char inputUserName[100];
			char inputPassword[100];
			GetDlgItemText(hDlg,IDC_LOGIN_USERNAME,inputUserName,100);
			GetDlgItemText(hDlg,IDC_LOGIN_PASSWORD,inputPassword,100);
			string username(inputUserName);
			string password(inputPassword);

			string lUsersQuerySelect="SELECT * FROM tbl_users where pLoginID='" + username + "' AND Password='" + password + "'";
			mUsersModel = lDBOperationController->loadLoginUsersName(lUsersQuerySelect);

			if( mUsersModel == NULL )
			{
				SetDlgItemText(hDlg,IDC_LOGIN_USERNAME,"");
				SetDlgItemText(hDlg,IDC_LOGIN_PASSWORD,"");
				MessageBox(hDlg,"Please input the correct Username and Password","Wrong UserName or Password",MB_OK|MB_ICONINFORMATION);
			}
			else
			{
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
		}
		if (LOWORD(wParam) == IDCANCEL)
		{
			PostQuitMessage(0);
		}
		break;
	}
	return (INT_PTR)FALSE;
}