#include "FeedMonitorInfo.h"

extern HANDLE				g_ExtHandleFeedMonitorInfo;
extern FeedMonitorInfo	    *g_ExtFeedMonitorInfo;
extern HINSTANCE			hInst;
extern HWND					g_ExtHwndFeedMonitorInfo;

FeedMonitorInfo::FeedMonitorInfo()
{
	mHwndFeedMonitorInfo = NULL;
	mFeedMonitorInfoView = NULL;
}

FeedMonitorInfo::~FeedMonitorInfo()
{
	mHwndFeedMonitorInfo = NULL;
}

bool FeedMonitorInfo::InitializeViewAndData()
{
	mFeedMonitorInfoView= new FeedMonitorInfoView();
	mFeedMonitorInfoView->Initialize(mHwndFeedMonitorInfo);
	mFeedMonitorInfoView->DrawFeedMonitorListViewBody();
	mFeedMonitorInfoView->FillRowFeedMonitorListView();

	return true;
}

LRESULT CALLBACK FeedMonitorInfoWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_SIZE:
		// Redraw controls
		RECT rect;
		GetClientRect(hWnd, &rect);

			MoveWindow(g_ExtFeedMonitorInfo->mFeedMonitorInfoView->mHandleWindowFeedMonitorListView, //Handle to the window.
				0, // [in] Specifies the new position of the left side of the window. 
				0, // [in] Specifies the new position of the top of the window. 
				rect.right,	// Specifies the new width of the window.
				rect.bottom,// Specifies the new height of the window. 
				true); // Boolean that specifies whether the window is to be repainted. 
		break; //Do not return since this should be processed by DefMDIChildProc
	case WM_CLOSE:
		if(NULL != g_ExtHandleFeedMonitorInfo)
		{
			CloseHandle(g_ExtHandleFeedMonitorInfo);
			g_ExtHandleFeedMonitorInfo = NULL;
		}
		break; // Go on to call DefMDIChildProc
	case WM_DESTROY:
		break;
	}
	return DefMDIChildProc(hWnd, message, wParam, lParam); //Frame window calls DefFrameProc rather than DefWindowProc
}

HWND FeedMonitorInfo::CreateFeedMonitorInfoWindow()
{
	//Register FeedMonitorInformation Window
	WNDCLASSEX wndclassMainFrame;
	wndclassMainFrame.cbSize		= sizeof(WNDCLASSEX);
	wndclassMainFrame.style			= CS_HREDRAW | CS_VREDRAW;
	wndclassMainFrame.lpfnWndProc	= FeedMonitorInfoWndProc;
	wndclassMainFrame.cbClsExtra	= 0;
	wndclassMainFrame.cbWndExtra	= 0;
	wndclassMainFrame.hInstance		= hInst;
	wndclassMainFrame.hIcon			= NULL;
	wndclassMainFrame.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wndclassMainFrame.hbrBackground	= (HBRUSH) GetStockObject(GRAY_BRUSH);
	wndclassMainFrame.lpszMenuName	= NULL;
	wndclassMainFrame.lpszClassName	= TEXT("FXTFFeedMonitorInfoWnd");
	wndclassMainFrame.hIconSm		= NULL;
	
	if(!RegisterClassEx(&wndclassMainFrame))
	{
		// return if RegisterClassEx fails
		DWORD dw_LastError = GetLastError();
		if(ERROR_CLASS_ALREADY_EXISTS != dw_LastError)
		{
			// return if the error is other than "error class already exists" 
			return 0;
		}
	}
	// Create Window
	if(NULL != mHwndFeedMonitorInfo)
	{
		return 0;
	}
	MDICREATESTRUCT    MDIChildCreateStruct;
	MDIChildCreateStruct.szClass = TEXT("FXTFFeedMonitorInfoWnd");
	MDIChildCreateStruct.szTitle = TEXT("Quotes Information");
	MDIChildCreateStruct.hOwner	 = hInst;
	MDIChildCreateStruct.x		 = CW_USEDEFAULT;
	MDIChildCreateStruct.y		 = CW_USEDEFAULT;
	MDIChildCreateStruct.cx		 = CW_USEDEFAULT;
	MDIChildCreateStruct.cy		 = CW_USEDEFAULT;
	MDIChildCreateStruct.style	 = 0;
	MDIChildCreateStruct.lParam	 = 0;
	mHwndFeedMonitorInfo = (HWND) SendMessage(g_ExtHwndFeedMonitorInfo,
												WM_MDICREATE, 
												0,
												(LPARAM) (LPMDICREATESTRUCT) &MDIChildCreateStruct) ;
	// return if its not possible to create the child window
	if(NULL == mHwndFeedMonitorInfo)
	{
		return 0;
	}
	return mHwndFeedMonitorInfo;
}