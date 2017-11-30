#define WIN32_LEAN_AND_MEAN

#include "FeedSwitcher.h"


int	mFlagFeedSwitcher	=	0;
int	mIndexFeedSwitcher	=	0;
int mSelectItemOneClickFeedMonitor	= 0;
int mFlagPQE	= 0;
int mFlagOCBC	= 0;
int mFlagFXCM	= 0;
int mTotalSymbols = 0;

LVCOLUMN			mListViewColumFeedSwitcher;
LVITEM				mListViewItemFeedSwitcher;
HWND				mListViewIdentifierFeedSwitcher		= NULL;


FeedSwitcher::FeedSwitcher()
{
	mDBOperationController=new DBOperationController();
}

FeedSwitcher::~FeedSwitcher()
{

}

bool FeedSwitcher::FillAndDisplayListViewItems(HWND hWnd)
{
	memset((void*)&mListViewColumFeedSwitcher, 0, sizeof(mListViewColumFeedSwitcher));
	mListViewColumFeedSwitcher.mask = LVCF_TEXT | LVCF_WIDTH;			// Mask
	mListViewColumFeedSwitcher.cx = 0x60;								// column width

	mListViewIdentifierFeedSwitcher=GetDlgItem(hWnd,IDC_LIST);			// get the ID of the ListView

	SendMessage(mListViewIdentifierFeedSwitcher,LVM_SETTEXTBKCOLOR, 0,(LPARAM)CLR_NONE);
    SendMessage(mListViewIdentifierFeedSwitcher,LVM_SETEXTENDEDLISTVIEWSTYLE,0,LVS_EX_FULLROWSELECT); // Set style

	mListViewColumFeedSwitcher.pszText = "Symbol ID";                         
	SendMessage(mListViewIdentifierFeedSwitcher,LVM_INSERTCOLUMN,0,(LPARAM)&mListViewColumFeedSwitcher);
	
	mListViewColumFeedSwitcher.pszText = "Symbol";                         
	SendMessage(mListViewIdentifierFeedSwitcher,LVM_INSERTCOLUMN,1,(LPARAM)&mListViewColumFeedSwitcher);

	mListViewColumFeedSwitcher.pszText = "OCBC";                         
	SendMessage(mListViewIdentifierFeedSwitcher,LVM_INSERTCOLUMN,2,(LPARAM)&mListViewColumFeedSwitcher);

	mListViewColumFeedSwitcher.pszText="FXCM";                            
	SendMessage(mListViewIdentifierFeedSwitcher,LVM_INSERTCOLUMN,3,(LPARAM)&mListViewColumFeedSwitcher); 

	mListViewColumFeedSwitcher.pszText="FXCM-US";                            
	SendMessage(mListViewIdentifierFeedSwitcher,LVM_INSERTCOLUMN,4,(LPARAM)&mListViewColumFeedSwitcher); 

	mListViewItemFeedSwitcher.mask = LVIF_TEXT;  // Text Style
	mListViewItemFeedSwitcher.cchTextMax = 256;	// Max size of test

	//======================Fetching data from Database and then showing into ListView
	string lQuerySymbolOutModel="SELECT pAutoIncrimentID,fSymbolID,SymbolText,fSymbolDigit FROM tbl_symbols_out";
	list<SymbolOutModel> lListSymbolOutModel = mDBOperationController->loadSymbolOut(lQuerySymbolOutModel);
	int aTotalNumberOfRowOfSymbolOutModel = lListSymbolOutModel.size();
	mTotalSymbols = lListSymbolOutModel.size();
	list<SymbolOutModel>::iterator list_iter_symbolOut = lListSymbolOutModel.begin();

	char lListViewTempItem[255];

	for(int row=0; row<aTotalNumberOfRowOfSymbolOutModel; row++, list_iter_symbolOut++)
	{
		SymbolOutModel lSymbolOutModel=(SymbolOutModel)*list_iter_symbolOut;
		mListViewItemFeedSwitcher.iItem =	row;										// main item number, this can be considered as row number 

		mListViewItemFeedSwitcher.iSubItem = FEED_SWITCHER_INDEX;										// Put in first coluom
		sprintf_s(lListViewTempItem,"%d", lSymbolOutModel.getSymbolID());
		mListViewItemFeedSwitcher.pszText = lListViewTempItem;
		SendMessage(mListViewIdentifierFeedSwitcher,LVM_INSERTITEM,0,(LPARAM)&mListViewItemFeedSwitcher);				// Send to the Listview

		mListViewItemFeedSwitcher.iSubItem = FEED_SWITCHER_SYMBOL;									// Put in second coluom
		sprintf_s(lListViewTempItem,"%s", lSymbolOutModel.getSymbolOutText().c_str());
		mListViewItemFeedSwitcher.pszText = lListViewTempItem;
		SendMessage(mListViewIdentifierFeedSwitcher,LVM_SETITEM,0,(LPARAM)&mListViewItemFeedSwitcher);				// Send to the Listview 

		mListViewItemFeedSwitcher.iSubItem = FEED_SWITCHER_OCBC;									// Put in second coluom
		//sprintf_s(lListViewTempItem,"%s", 0);
		mListViewItemFeedSwitcher.pszText = "0";//lListViewTempItem;
		SendMessage(mListViewIdentifierFeedSwitcher,LVM_SETITEM,0,(LPARAM)&mListViewItemFeedSwitcher);				// Send to the Listview 

		mListViewItemFeedSwitcher.iSubItem = FEED_SWITCHER_FXCM;									// Put in second coluom
		//sprintf_s(lListViewTempItem,"%s", 1);
		mListViewItemFeedSwitcher.pszText = "0";//lListViewTempItem;
		SendMessage(mListViewIdentifierFeedSwitcher,LVM_SETITEM,0,(LPARAM)&mListViewItemFeedSwitcher);				// Send to the Listview 

		mListViewItemFeedSwitcher.iSubItem = FEED_SWITCHER_FXCMUS;									// Put in second coluom
		//sprintf_s(lListViewTempItem,"%s", 2);
		mListViewItemFeedSwitcher.pszText = "0";//lListViewTempItem;
		SendMessage(mListViewIdentifierFeedSwitcher,LVM_SETITEM,0,(LPARAM)&mListViewItemFeedSwitcher);				// Send to the Listview 
	}

	return true;
}


INT_PTR CALLBACK CustomDialogBoxForListVIew(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	
	DBOperationController *mDBOperationController=new DBOperationController();
	
	FeedSwitcher *mFeedSwitcher = new FeedSwitcher();
	FeedClientSocketController *lFeedClientSocketController = SingletonFeedClient::GetInstance()->getFeedClientSocketController();

	switch (message)
	{
	case WM_NOTIFY:
		{
			switch(LOWORD(wParam))
			{
			    case IDC_LIST: 
					if(((LPNMHDR)lParam)->code == NM_CLICK)
					{
						char lSelectedSymbolName[100];
						mFlagPQE = 0; mFlagOCBC	= 0; mFlagFXCM = 0;
						mIndexFeedSwitcher=SendMessage(mListViewIdentifierFeedSwitcher,LVM_GETNEXTITEM,-1,LVNI_FOCUSED);
						if(mIndexFeedSwitcher==-1)
						{                      
						  break;
						}
						else
						{
							mIndexFeedSwitcher = mIndexFeedSwitcher + 1;
						}
						
						//======================Fetching data from SymbolSourceMappingModel and then showing value into the "FeedSwitcher"
						string lQuerySymbolSourceMappingModel="SELECT pAutoIncrimentID,fSymbolID,fFeedProviderID FROM tbl_symbol_source_mapping";
						list<SymbolSourceMappingModel> lSymbolSourceMappingModel = mDBOperationController->loadSymbolSourceMapping(lQuerySymbolSourceMappingModel);
						int aTotalSymbolSourceMapping = lSymbolSourceMappingModel.size();
						list<SymbolSourceMappingModel>::iterator lIteratorSymbolSourceMappingModel = lSymbolSourceMappingModel.begin();
						
						for(int row=0; row<aTotalSymbolSourceMapping; row++, lIteratorSymbolSourceMappingModel++ )
						{
							SymbolSourceMappingModel lRowWiseData =(SymbolSourceMappingModel)*lIteratorSymbolSourceMappingModel;
							
							if( mIndexFeedSwitcher == lRowWiseData.getSymbolID() )
							{
								if( lRowWiseData.getFeedProviderID() == 4 )
								{
									mFlagPQE = 1;
								}
								if( lRowWiseData.getFeedProviderID() == 2 )
								{
									mFlagOCBC = 1;
								}
								if( lRowWiseData.getFeedProviderID() == 3 )
								{
									mFlagFXCM = 1;
								}
							}
						}
						if( mFlagPQE == 0 )
						{
							CheckDlgButton(hWnd,IDC_CHECK1, BST_UNCHECKED);
						}
						else
						{
							CheckDlgButton(hWnd,IDC_CHECK1, BST_CHECKED);
						}
						if( mFlagOCBC == 0 )
						{
							CheckDlgButton(hWnd,IDC_CHECK2, BST_UNCHECKED);
						}
						else
						{
							CheckDlgButton(hWnd,IDC_CHECK2, BST_CHECKED);
						}
						if( mFlagFXCM == 0 )
						{
							CheckDlgButton(hWnd,IDC_CHECK3, BST_UNCHECKED);
						}
						else
						{
							CheckDlgButton(hWnd,IDC_CHECK3, BST_CHECKED);
						}

						string lQuerySymbolOutModel="SELECT pAutoIncrimentID,fSymbolID,SymbolText,fSymbolDigit FROM tbl_symbols_out";
						list<SymbolOutModel> lListSymbolOutModel = mDBOperationController->loadSymbolOut(lQuerySymbolOutModel);
						int aTotalSymbolOutModel = lListSymbolOutModel.size();
						list<SymbolOutModel>::iterator lIteratorSymbolOutModel = lListSymbolOutModel.begin();

						for(int row=0; row<aTotalSymbolOutModel; row++, lIteratorSymbolOutModel++ )
						{
							SymbolOutModel lRowWiseSymbolOut = (SymbolOutModel)*lIteratorSymbolOutModel;
							if( mIndexFeedSwitcher == lRowWiseSymbolOut.getSymbolID() )
							{
								sprintf_s(lSelectedSymbolName, "%s", lRowWiseSymbolOut.getSymbolOutText().c_str());
							}
						    SetDlgItemText(hWnd,IDC_SYMBOL_NAME, lSelectedSymbolName );
						}
					}
					/*if( ((LPNMHDR)lParam)->code == NM_DBLCLK )
					{
						mSelectItemOneClickFeedMonitor=SendMessage(mListViewIdentifierFeedSwitcher,LVM_GETNEXTITEM,-1,LVNI_FOCUSED);
						if(mSelectItemOneClickFeedMonitor==-1)
						{                      
						  break;
						}
						mIndexFeedSwitcher=mSelectItemOneClickFeedMonitor;
						mFlagFeedSwitcher=1;
						char tempTextMessage[100]={0};
						sprintf_s(tempTextMessage,"%d",mSelectItemOneClickFeedMonitor);

					}*/
					break;	
			}
		}
		break;
	case WM_INITDIALOG:
		{
			mFeedSwitcher->FillAndDisplayListViewItems(hWnd);
			//ShowWindow (GetDlgItem(hWnd,IDC_FEED_SOURCE), SW_HIDE );
			//ShowWindow( GetDlgItem(hWnd,IDC_CHECK1), SW_HIDE );
			//ShowWindow( GetDlgItem(hWnd,IDC_CHECK2), SW_HIDE );
			//ShowWindow( GetDlgItem(hWnd,IDC_CHECK3), SW_HIDE );
		}
		break;
	case WM_COMMAND:
		{
			switch(LOWORD(wParam)) // what we press on?
			{
				case IDC_BUTTON_SAVE:
					{
						char lCurrentFeedSwitcherSettings[100]={0};
						sprintf_s(lCurrentFeedSwitcherSettings,"%d",mIndexFeedSwitcher);

						int lIsDlgButtonChecked1 = IsDlgButtonChecked(hWnd,IDC_CHECK1);
						int lIsDlgButtonChecked2 = IsDlgButtonChecked(hWnd,IDC_CHECK2);
						int lIsDlgButtonChecked3 = IsDlgButtonChecked(hWnd,IDC_CHECK3);

						if(MessageBox(hWnd,"Do you want to save really to database?","Feed Switcher (Save)",MB_YESNO) == IDYES)
						{
							// insert=1 (int lQueryType, lQuerySymbolID, lQueryFeedProviderID)
							if( lIsDlgButtonChecked2 != mFlagOCBC && lIsDlgButtonChecked2 == 1 ) //insert //OCBC
							{
								int lInsertStatusOCBC =  mDBOperationController->setOperationToSymbolSourceMapping( 1, mIndexFeedSwitcher, 2);
								if(lInsertStatusOCBC)
								{
									SingletonFeedClient::GetInstance()->getFXTFLog()->LogOutLn("Symbol Switcher Updated: " + SingletonFeedClient::GetInstance()->getUsersModel()->getUsersName());
									int lSendMsgStatus = lFeedClientSocketController->sendFeedSwitcherSettingsToServer(mIndexFeedSwitcher, SingletonFeedClient::GetInstance()->getUsersModel()->getUsersName());
								}
							}
							if( lIsDlgButtonChecked3 != mFlagFXCM && lIsDlgButtonChecked3 == 1 ) //insert //FXCM
							{
								int lInsertStatusFXCM =  mDBOperationController->setOperationToSymbolSourceMapping( 1, mIndexFeedSwitcher, 3);
								if(lInsertStatusFXCM)
								{
									SingletonFeedClient::GetInstance()->getFXTFLog()->LogOutLn("Symbol Switcher Updated: " +  SingletonFeedClient::GetInstance()->getUsersModel()->getUsersName());
									int lSendMsgStatus = lFeedClientSocketController->sendFeedSwitcherSettingsToServer(mIndexFeedSwitcher, SingletonFeedClient::GetInstance()->getUsersModel()->getUsersName());
								}
							}
							if( lIsDlgButtonChecked1 != mFlagPQE && lIsDlgButtonChecked1 == 1 ) //insert //FXCM-US
							{
								int lInsertStatusPQE =  mDBOperationController->setOperationToSymbolSourceMapping( 1, mIndexFeedSwitcher, 4);
								if( lInsertStatusPQE )
								{
									SingletonFeedClient::GetInstance()->getFXTFLog()->LogOutLn("Symbol Switcher Updated: "+ SingletonFeedClient::GetInstance()->getUsersModel()->getUsersName());
									int lSendMsgStatus = lFeedClientSocketController->sendFeedSwitcherSettingsToServer(mIndexFeedSwitcher, SingletonFeedClient::GetInstance()->getUsersModel()->getUsersName());
								}
								
							}
							// delete=0 (int lQueryType, lQuerySymbolID, lQueryFeedProviderID)
							if( lIsDlgButtonChecked1 != mFlagPQE && lIsDlgButtonChecked1 == 0 ) //delete
							{
								int lDeleteStatusPQE =  mDBOperationController->setOperationToSymbolSourceMapping( 0, mIndexFeedSwitcher, 1);
								if(lDeleteStatusPQE)
								{
									SingletonFeedClient::GetInstance()->getFXTFLog()->LogOutLn("Symbol Switcher Updated: "+ SingletonFeedClient::GetInstance()->getUsersModel()->getUsersName());
									int lSendMsgStatus = lFeedClientSocketController->sendFeedSwitcherSettingsToServer(mIndexFeedSwitcher, SingletonFeedClient::GetInstance()->getUsersModel()->getUsersName());
								}
							}
							if( lIsDlgButtonChecked2 != mFlagOCBC && lIsDlgButtonChecked2 == 0 ) //delete
							{
								int lDeleteStatusOCBC =  mDBOperationController->setOperationToSymbolSourceMapping( 0, mIndexFeedSwitcher, 2);
								if(lDeleteStatusOCBC)
								{
									SingletonFeedClient::GetInstance()->getFXTFLog()->LogOutLn("Symbol Switcher Updated: " + SingletonFeedClient::GetInstance()->getUsersModel()->getUsersName());
									int lSendMsgStatus = lFeedClientSocketController->sendFeedSwitcherSettingsToServer(mIndexFeedSwitcher, SingletonFeedClient::GetInstance()->getUsersModel()->getUsersName());
								}
							}
							if( lIsDlgButtonChecked3 != mFlagFXCM && lIsDlgButtonChecked3 == 0 ) //delete
							{
								int lDeleteStatusFXCM =  mDBOperationController->setOperationToSymbolSourceMapping( 0, mIndexFeedSwitcher, 3);
								if(lDeleteStatusFXCM)
								{
									SingletonFeedClient::GetInstance()->getFXTFLog()->LogOutLn("Symbol Switcher Updated: " + SingletonFeedClient::GetInstance()->getUsersModel()->getUsersName());
									int lSendMsgStatus = lFeedClientSocketController->sendFeedSwitcherSettingsToServer(mIndexFeedSwitcher, SingletonFeedClient::GetInstance()->getUsersModel()->getUsersName());
								}
							}
						}
						else
						{
							// to do nothing
						}
					}
				break;
				case IDC_BUTTON_SAVE_ALL:
					{
						if(MessageBox(hWnd,"Do you want to save really to database?","Feed Switcher (Save)",MB_YESNO) == IDYES)
						{
							int lIsDlgButtonCheckedFXCMUS = IsDlgButtonChecked(hWnd,IDC_CHECK1);
							int lIsDlgButtonCheckedOCBC = IsDlgButtonChecked(hWnd,IDC_CHECK2);
							int lIsDlgButtonCheckedFXCM = IsDlgButtonChecked(hWnd,IDC_CHECK3);
							// mTotalSymbols;
							if(lIsDlgButtonCheckedFXCMUS)
							{
								int lInsertStatusFXCMUS =  mDBOperationController->setOperationToAllSymbolSourceMapping( mTotalSymbols, 4);
							}
							if(lIsDlgButtonCheckedOCBC)
							{
								int lInsertStatusOCBC =  mDBOperationController->setOperationToAllSymbolSourceMapping( mTotalSymbols, 2);
							}
							if(lIsDlgButtonCheckedFXCM)
							{
								int lInsertStatusFXCM =  mDBOperationController->setOperationToAllSymbolSourceMapping( mTotalSymbols, 3);
							}
						}
						else
						{
							//To do nothing;
							/*
							INSERT INTO tbl_symbol_source_mapping (fSymbolID, fFeedProviderID)
							SELECT '1', '1' from tbl_symbol_source_mapping
							WHERE NOT EXISTS ( SELECT fSymbolID,fFeedProviderID FROM tbl_symbol_source_mapping 
							WHERE fSymbolID= '1' AND fFeedProviderID= '1') LIMIT 1;
							*/
						}

					}
				break;

				case IDC_BUTTON_RESET:
					{
						if(MessageBox(hWnd,"Do you want to reset really?","Feed Switcher (Reset)",MB_YESNO) == IDYES)
						{
							CheckDlgButton(hWnd, IDC_CHECK1, BST_UNCHECKED);
							CheckDlgButton(hWnd, IDC_CHECK2, BST_UNCHECKED);
							CheckDlgButton(hWnd, IDC_CHECK3, BST_UNCHECKED);
						}
						else
						{
							//To do nothing;
						}
					}
				break;

				default:
				{
				     break;
				}
			}
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hWnd, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			break;
		}
	}
	return (INT_PTR)FALSE;
}

void FeedSwitcher::DisplayFeedSwitcherWindow(HINSTANCE hInst)
{
	DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_FEED_SWITCHER), NULL ,CustomDialogBoxForListVIew);
}