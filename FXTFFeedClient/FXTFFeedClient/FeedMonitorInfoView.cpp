#include "FeedMonitorInfoView.h"
#include "FeedClientCommon\FeedClientConstant.h"

FeedMonitorInfoView::FeedMonitorInfoView()
{
	isGUIInitialized=false;
	mDBOperationController=new DBOperationController();

	mDefaultColorCode = 0;
	mGreenColorCode	  = 1;
	mRedColorColde    = 2;
}

FeedMonitorInfoView::~FeedMonitorInfoView()
{

}

bool FeedMonitorInfoView::Initialize(HWND hParentWnd)
{
	mHandleWindowFeedMonitorInfoViewParentWindow = hParentWnd;
	//FeedMonitorInfoView::ReceiveQuotesFromServer();
	return true;
}

bool FeedMonitorInfoView::DrawFeedMonitorListViewBody()
{
	RECT rect;
	GetClientRect(mHandleWindowFeedMonitorInfoViewParentWindow, &rect);

	mHandleWindowFeedMonitorListView = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		WC_LISTVIEW,
		"", //caption not required
		WS_CHILD | WS_VISIBLE | LVS_REPORT,
		0,  
		0, 
		rect.right,	// width
		rect.bottom,// height
		mHandleWindowFeedMonitorInfoViewParentWindow,
		NULL,
		(HINSTANCE) GetWindowLong (mHandleWindowFeedMonitorInfoViewParentWindow, GWL_HINSTANCE),
		NULL);
		// Set the single row select style
		SendMessage(mHandleWindowFeedMonitorListView, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT); // Set style

	LVCOLUMN ListviewCol;
	memset((void*)&ListviewCol, 0, sizeof(ListviewCol));
    
	ListviewCol.mask = LVCF_TEXT | LVCF_WIDTH;		// Mask
	ListviewCol.cx = 0x65;							// column width

	//ListviewCol.pszText = "Index";
	//SendMessage(mHandleWindowFeedMonitorListView,LVM_INSERTCOLUMN,FEED_MONITOR_SYMBOL_INDEX,(LPARAM)&ListviewCol);
	ListviewCol.pszText = "Symbol";
	SendMessage(mHandleWindowFeedMonitorListView,LVM_INSERTCOLUMN,FEED_MONITOR_SYMBOL,(LPARAM)&ListviewCol);

	ListviewCol.pszText = "Bid(OCBC)";
	SendMessage(mHandleWindowFeedMonitorListView,LVM_INSERTCOLUMN,FEED_MONITOR_OCBC_BID,(LPARAM)&ListviewCol);
	ListviewCol.pszText = "Ask(OCBC)";
	SendMessage(mHandleWindowFeedMonitorListView,LVM_INSERTCOLUMN,FEED_MONITOR_OCBC_ASK,(LPARAM)&ListviewCol);
	ListviewCol.pszText = "Update(OCBC)";
	SendMessage(mHandleWindowFeedMonitorListView,LVM_INSERTCOLUMN,FEED_MONITOR_OCBC_DATE,(LPARAM)&ListviewCol);

	ListviewCol.pszText = "Bid(FXCM)";
	SendMessage(mHandleWindowFeedMonitorListView,LVM_INSERTCOLUMN,FEED_MONITOR_PQE_BID,(LPARAM)&ListviewCol);
	ListviewCol.pszText = "Ask(FXCM)";
	SendMessage(mHandleWindowFeedMonitorListView,LVM_INSERTCOLUMN,FEED_MONITOR_PQE_ASK,(LPARAM)&ListviewCol);
	ListviewCol.pszText = "Update(FXCM)";
	SendMessage(mHandleWindowFeedMonitorListView,LVM_INSERTCOLUMN,FEED_MONITOR_PQE_DATE,(LPARAM)&ListviewCol);

	ListviewCol.pszText = "Bid(FXCM-US)";
	SendMessage(mHandleWindowFeedMonitorListView,LVM_INSERTCOLUMN,FEED_MONITOR_FXCMUS_BID,(LPARAM)&ListviewCol);
	ListviewCol.pszText = "Ask(FXCM-US)";
	SendMessage(mHandleWindowFeedMonitorListView,LVM_INSERTCOLUMN,FEED_MONITOR_FXCMUS_ASK,(LPARAM)&ListviewCol);
	ListviewCol.pszText = "Update(FXCM-US)";
	SendMessage(mHandleWindowFeedMonitorListView,LVM_INSERTCOLUMN,FEED_MONITOR_FXCMUS_DATE,(LPARAM)&ListviewCol);

	//SendMessage(mHandleWindowFeedMonitorListView, LVM_SETBKCOLOR,0,(LPARAM)SetColor(200,200,200));
	return true;
}

bool FeedMonitorInfoView::FillRowFeedMonitorListView()
{
	string lQueryStringSelect="SELECT pAutoIncrimentID,pSymbolName,Description FROM tbl_symbols";
	mListDataFromDB = mDBOperationController->loadSymbolNames(lQueryStringSelect);
	int aTotalNumberOfRowOfQuotes = mListDataFromDB.size();
	std::list<SymbolModel>::iterator list_iter = mListDataFromDB.begin();

	string lQuerySymbolOutModel="SELECT pAutoIncrimentID,fSymbolID,SymbolText,fSymbolDigit FROM tbl_symbols_out";
	mListSymbolOutModel = mDBOperationController->loadSymbolOut(lQuerySymbolOutModel);
	int aTotalNumberOfRowOfSymbolOutModel = mListSymbolOutModel.size();
	list<SymbolOutModel>::iterator list_iter_symbolOut = mListSymbolOutModel.begin();

	//LVITEM mFeedMonitorLVITEM;		   // ListView Item struct
	mFeedMonitorLVITEM.mask = LVIF_TEXT;   // Text Style
	mFeedMonitorLVITEM.cchTextMax = 256;   // Max size of test
    
	char lListViewTempItem[255];
	for (int row=0; row<aTotalNumberOfRowOfQuotes; row++, list_iter++)
	{
		SymbolModel lTestModelClass=(SymbolModel)*list_iter;

		mFeedMonitorLVITEM.iItem	= row;

		mFeedMonitorLVITEM.iSubItem = FEED_MONITOR_SYMBOL;
		sprintf_s(lListViewTempItem,"%s", lTestModelClass.getSymbolName().c_str());
		mFeedMonitorLVITEM.pszText  = lListViewTempItem;
		SendMessage(mHandleWindowFeedMonitorListView,LVM_INSERTITEM,0,(LPARAM)&mFeedMonitorLVITEM);
	}
	isGUIInitialized=true;

	return true;
}

void FeedMonitorInfoView::receiveQuotesMessage(string aStrQuotes)
{
	cout <<aStrQuotes<<endl;
	updateListViewItemAccordingToSymbol(aStrQuotes);
}

void FeedMonitorInfoView::updateListViewItemAccordingToSymbol(string aStrQuotes)
{
	int lColor = 0;
	vector<string> lInputStrings = splitStringQuotesMessage(aStrQuotes);
	string lCurrentTime = SingletonFeedClient::GetInstance()->getFXTFLog()->GetTimeStr();
	string lCurrentTimeHourFromatter = lCurrentTime.substr(10,9);

	int lIndex=findSymbolIndexInListView(lInputStrings[1]);
	int lRoundOfSymbolRate = findSymbolDigit(lIndex+1);

	if (lIndex==-1)
	{
		// Write Log Here about Something wrong	
		return;
	}
	if (lRoundOfSymbolRate==-1)
	{
		// Write Log Here about Something wrong
		return;
	}
	if (atoi(lInputStrings[0].c_str())==2)
	{
		lColor = checkCurrentRateStatus(lIndex, 1, lInputStrings[2]);
		//updateListViewItems(lIndex, 1, lInputStrings[2], lColor);
		updateListViewItems(lIndex, 1, lInputStrings[2].substr( 0, lInputStrings[2].find(".") + 1 + lRoundOfSymbolRate ), lColor);

		lColor = checkCurrentRateStatus(lIndex, 2, lInputStrings[3]);
		//updateListViewItems(lIndex, 2, lInputStrings[3], lColor);
		updateListViewItems(lIndex, 2, lInputStrings[3].substr( 0, lInputStrings[3].find(".") + 1 + lRoundOfSymbolRate ), lColor);

		updateListViewItems(lIndex, 3, lCurrentTimeHourFromatter, mDefaultColorCode);
	}
	else if (atoi(lInputStrings[0].c_str())==3)
	{
		lColor = checkCurrentRateStatus(lIndex, 4, lInputStrings[2]);
		updateListViewItems(lIndex, 4, lInputStrings[2].substr( 0, lInputStrings[2].find(".") + 1 + lRoundOfSymbolRate ), lColor);

		lColor = checkCurrentRateStatus(lIndex, 5, lInputStrings[3]);
		updateListViewItems(lIndex, 5, lInputStrings[3].substr( 0, lInputStrings[3].find(".") + 1 + lRoundOfSymbolRate ), lColor);
		
		updateListViewItems(lIndex, 6, lCurrentTimeHourFromatter, mDefaultColorCode);
	}
	else if (atoi(lInputStrings[0].c_str())==4)  //FXCM-US
	{
		lColor = checkCurrentRateStatus(lIndex, 7, lInputStrings[2]);
		updateListViewItems(lIndex, 7, lInputStrings[2].substr( 0, lInputStrings[2].find(".") + 1 + lRoundOfSymbolRate ), lColor);

		lColor = checkCurrentRateStatus(lIndex, 8, lInputStrings[3]);
		updateListViewItems(lIndex, 8, lInputStrings[3].substr( 0, lInputStrings[3].find(".") + 1 + lRoundOfSymbolRate ), lColor);
		
		updateListViewItems(lIndex, 9, lCurrentTimeHourFromatter, mDefaultColorCode);
	}
}

LONG FeedMonitorInfoView::SetColor(int aColorX,int aColorY ,int aColorZ )
{
    //Black=0,0,0 Red=255,0,0;Green=0,255,0
    static CHOOSECOLOR cc ;
    static COLORREF    crCustColors[16] ;

    cc.lStructSize    = sizeof (CHOOSECOLOR) ;
    cc.hwndOwner      = NULL ;
    cc.hInstance      = NULL ;
    cc.rgbResult      = RGB (aColorX, aColorY, aColorZ);
    cc.lpCustColors   = crCustColors ;
    cc.Flags          = CC_RGBINIT | CC_FULLOPEN ;
    cc.lCustData      = 0 ;
    cc.lpfnHook       = NULL ;
    cc.lpTemplateName = NULL ;

    return cc.rgbResult;
}

void FeedMonitorInfoView::updateListViewItems(int aIndex,int aSubIndex,string aStrValue, int aColorCode)
{
	mFeedMonitorLVITEM.mask = LVIF_TEXT;    // Text Style
	mFeedMonitorLVITEM.cchTextMax = 256; 
	mFeedMonitorLVITEM.iItem	= aIndex;   //This is index number
	mFeedMonitorLVITEM.iSubItem = aSubIndex;
	mFeedMonitorLVITEM.pszText = (LPSTR)aStrValue.c_str();

	if( aColorCode == 0 ) //Black
		SendMessage(mHandleWindowFeedMonitorListView, LVM_SETTEXTCOLOR,0,(LPARAM)SetColor(0,0,0));
	if( aColorCode == 1 ) //Green
		SendMessage(mHandleWindowFeedMonitorListView, LVM_SETTEXTCOLOR,0,(LPARAM)SetColor(74,159,96));
	if( aColorCode == 2 ) //Red
		SendMessage(mHandleWindowFeedMonitorListView, LVM_SETTEXTCOLOR,0,(LPARAM)SetColor(255,0,0));
	
	SendMessage(mHandleWindowFeedMonitorListView,LVM_SETITEM,0,(LPARAM)&mFeedMonitorLVITEM);

}

int FeedMonitorInfoView::findSymbolDigit(int aIntSymbolIndex)
{
	if (&mListSymbolOutModel!=NULL && isGUIInitialized)
	{
		if (mListSymbolOutModel.size()>0)
		{
			int lCount=0;
			for(std::list<SymbolOutModel>::iterator list_iter = mListSymbolOutModel.begin(); list_iter != mListSymbolOutModel.end(); list_iter++)
			{
				SymbolOutModel lSymbolOutModel=(SymbolOutModel)*list_iter;
				if ( lSymbolOutModel.getSymbolID() ==  aIntSymbolIndex )
				{
					int lMatchedSymbol = lSymbolOutModel.getSymbolDigit();
					return lMatchedSymbol;
				}
				lCount++;
			}
		}
	}
	return -1;
}

int FeedMonitorInfoView::findSymbolIndexInListView(string aStrSymbol)
{
	if (&mListDataFromDB!=NULL && isGUIInitialized)
	{
		if (mListDataFromDB.size()>0)
		{
			int lCount=0;
			for(std::list<SymbolModel>::iterator list_iter = mListDataFromDB.begin(); list_iter != mListDataFromDB.end(); list_iter++)
			{
				SymbolModel lSymbolModel=(SymbolModel)*list_iter;
				if ((lSymbolModel.getSymbolName().compare(aStrSymbol)==0))
				{
					return lCount;
				}
				lCount++;
			}
		}
	}
	return -1;
}

int FeedMonitorInfoView::checkCurrentRateStatus(int aiItem, int aiSubItem, string aRate)
{
	int lColorCodeNo;
	char lListViewTempItem[255];
	ListView_GetItemText(mHandleWindowFeedMonitorListView , aiItem, aiSubItem, LPSTR(lListViewTempItem), 255);
	mPreviousBidValue = atof(lListViewTempItem);
	mCurrentBidValue  = atof(aRate.c_str());

	if(mCurrentBidValue>mPreviousBidValue)
		lColorCodeNo = 1;
	else
		lColorCodeNo = 2;

	return lColorCodeNo;
}
vector<string> FeedMonitorInfoView::splitStringQuotesMessage(string aInputMessage)
{
	vector<string> lVectorQuotesMessage;
	istringstream lQuotesMessage(aInputMessage);
	string lSplittedQuotesMessage;
	int lIntIndexOfQuotesMessage = 0;
	while( getline( lQuotesMessage, lSplittedQuotesMessage, ' ' )  )
	{
		lVectorQuotesMessage.push_back(lSplittedQuotesMessage);
	}
	return lVectorQuotesMessage;
}