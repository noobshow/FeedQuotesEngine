#if !defined FeedMonitorInfo_H
#define FeedMonitorInfo_H

#include "FeedMonitorInfoView.h"

class FeedMonitorInfo
{
	HWND mHwndFeedMonitorInfo;	  //Feed Information as MDI Child window handle of Main FeedServerClient window
public:		
	FeedMonitorInfo();	
	~FeedMonitorInfo();	
	FeedMonitorInfoView	*mFeedMonitorInfoView;
	FeedMonitorInfoView * getFeedMonitorInfoView(){ return mFeedMonitorInfoView;}
	HWND CreateFeedMonitorInfoWindow();
	bool InitializeViewAndData();
};

#endif