#define WIN32_LEAN_AND_MEAN

#include "SymbolSpikeMappingView.h"

int					mFlagSymbolSpikeMapping						=	0;
int					mIndexSymbolSpikeMapping					=	0;
int                 mSelectItemOneClickSymbolSpikeMapping		=	0;
HWND				mListViewIdentifierSymbolSpikeMapping		=	NULL;
LVCOLUMN			mListViewColumSymbolSpikeMapping;
LVITEM				mListViewItemSymbolSpikeMapping;

SymbolSpikeMappingView::SymbolSpikeMappingView()
{
	mDBOperationController=new DBOperationController();
}

SymbolSpikeMappingView::~SymbolSpikeMappingView()
{
	delete mDBOperationController;
}

int SymbolSpikeMappingView::SpikeProtectionSettings(string CSVFileData)
{
	int lDBUpdateStatus;
	if(mDBOperationController)
	{
		lDBUpdateStatus = mDBOperationController->SpikeProtectionSettings(CSVFileData);
	}
	return lDBUpdateStatus;
}
void SymbolSpikeMappingView::InitialFillAndDisplayListViewSymbolSpikeMapping(HWND hWnd)
{
	memset((void*)&mListViewIdentifierSymbolSpikeMapping, 0, sizeof(mListViewIdentifierSymbolSpikeMapping));
	memset((void*)&mListViewColumSymbolSpikeMapping, 0, sizeof(mListViewColumSymbolSpikeMapping));
	memset((void*)&mListViewItemSymbolSpikeMapping, 0, sizeof(mListViewItemSymbolSpikeMapping));

	mListViewColumSymbolSpikeMapping.mask = LVCF_TEXT | LVCF_WIDTH;		// Mask
	mListViewColumSymbolSpikeMapping.cx = 0x60;							// column width

	mListViewIdentifierSymbolSpikeMapping=GetDlgItem(hWnd,IDC_LISTVIEW_SYMBOL_SPIKE_MAPPING);		 // get the ID of the ListView

	SendMessage(mListViewIdentifierSymbolSpikeMapping,LVM_SETTEXTBKCOLOR, 0,(LPARAM)CLR_NONE);
    SendMessage(mListViewIdentifierSymbolSpikeMapping,LVM_SETEXTENDEDLISTVIEWSTYLE,0,LVS_EX_FULLROWSELECT); // Set style

	mListViewColumSymbolSpikeMapping.pszText = "SymbolID";                         
	SendMessage(mListViewIdentifierSymbolSpikeMapping,LVM_INSERTCOLUMN,SYMBOLSPIKEMAPPING_FSYMBOLID,(LPARAM)&mListViewColumSymbolSpikeMapping);

	mListViewColumSymbolSpikeMapping.pszText = "Tick Percent";                         
	SendMessage(mListViewIdentifierSymbolSpikeMapping,LVM_INSERTCOLUMN,SYMBOLSPIKEMAPPING_TICKVALUECHANGEPERCENT,(LPARAM)&mListViewColumSymbolSpikeMapping);

	mListViewColumSymbolSpikeMapping.pszText="Tick Count";                            
	SendMessage(mListViewIdentifierSymbolSpikeMapping,LVM_INSERTCOLUMN,SYMBOLSPIKEMAPPING_TICKVALUECOUNT,(LPARAM)&mListViewColumSymbolSpikeMapping); 

	mListViewColumSymbolSpikeMapping.pszText="Pips Value";                            
	SendMessage(mListViewIdentifierSymbolSpikeMapping,LVM_INSERTCOLUMN,SYMBOLSPIKEMAPPING_PIPSVALUE,(LPARAM)&mListViewColumSymbolSpikeMapping); 

	mListViewItemSymbolSpikeMapping.mask = LVIF_TEXT;   // Text Style
	mListViewItemSymbolSpikeMapping.cchTextMax = 256;	// Max size of test
}

bool SymbolSpikeMappingView::FillAndDisplayListViewSymbolSpikeMapping(HWND hWnd)
{
	//If the List Item is exist then at first remove it and re-draw all the items one by one;
	SendMessage(mListViewIdentifierSymbolSpikeMapping,LVM_DELETEALLITEMS,0,(LPARAM)&mListViewItemSymbolSpikeMapping);

	string lQueryStringSelect="SELECT pAutoIncrimentID, fSymbolID, TickValueChangePercent, TickValueCount, PipsValue FROM tbl_symbol_spike_mapping";
	list<SymbolSpikeMappingModel> lListData = mDBOperationController->loadSymbolSpikeMappingInfo(lQueryStringSelect);
	int aTotalNumberOfRowOfQuotes = lListData.size();
	list<SymbolSpikeMappingModel>::iterator list_iter = lListData.begin();
	char lListViewTempItem[255];

	string lSymbolNamesQueryStringSelect="SELECT pAutoIncrimentID, pSymbolName, Description FROM tbl_symbols";
	list<SymbolModel> lListSymbolData = mDBOperationController->loadSymbolNames(lSymbolNamesQueryStringSelect);
	int aTotalNumberOfRowOfSymbol = lListSymbolData.size();
	list<SymbolModel>::iterator list_iter_symbol = lListSymbolData.begin();

	for(int row=0; row<aTotalNumberOfRowOfQuotes; row++, list_iter++,list_iter_symbol++)
	{
		SymbolSpikeMappingModel lTestModelClass=(SymbolSpikeMappingModel)*list_iter;
		mListViewItemSymbolSpikeMapping.iItem =	row;											// main item number, this can be considered as row number 
		
		SymbolModel lSymbolModelClass = (SymbolModel)*list_iter_symbol;

		mListViewItemSymbolSpikeMapping.iSubItem = SYMBOLSPIKEMAPPING_FSYMBOLID;												// Put in 2nd coluom
		sprintf_s(lListViewTempItem, "%s", lSymbolModelClass.getSymbolName().c_str());
		mListViewItemSymbolSpikeMapping.pszText = lListViewTempItem;
		SendMessage(mListViewIdentifierSymbolSpikeMapping,LVM_INSERTITEM,0,(LPARAM)&mListViewItemSymbolSpikeMapping);			// Send to the Listview
 
		mListViewItemSymbolSpikeMapping.iSubItem = SYMBOLSPIKEMAPPING_TICKVALUECHANGEPERCENT;									// Put in 3rd coluom
		sprintf_s(lListViewTempItem,"%f", lTestModelClass.getTickValueChangePercent());
		mListViewItemSymbolSpikeMapping.pszText = lListViewTempItem;
		SendMessage(mListViewIdentifierSymbolSpikeMapping,LVM_SETITEM,0,(LPARAM)&mListViewItemSymbolSpikeMapping);				// Send to the Listview
		
		mListViewItemSymbolSpikeMapping.iSubItem = SYMBOLSPIKEMAPPING_TICKVALUECOUNT;											// Put in 4th coluom
		sprintf_s(lListViewTempItem,"%d", lTestModelClass.getTickValueCount());
		mListViewItemSymbolSpikeMapping.pszText = lListViewTempItem;
		SendMessage(mListViewIdentifierSymbolSpikeMapping,LVM_SETITEM,0,(LPARAM)&mListViewItemSymbolSpikeMapping);				// Send to the Listview
		
		mListViewItemSymbolSpikeMapping.iSubItem = SYMBOLSPIKEMAPPING_PIPSVALUE;											    // Put in 5th coluom
		sprintf_s(lListViewTempItem,"%d", lTestModelClass.getPipsValue());
		mListViewItemSymbolSpikeMapping.pszText = lListViewTempItem;
		SendMessage(mListViewIdentifierSymbolSpikeMapping,LVM_SETITEM,0,(LPARAM)&mListViewItemSymbolSpikeMapping);				// Send to the Listview

	}
	return true;
}


INT_PTR CALLBACK  DialogSymbolSpikeMapping(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	
	SymbolSpikeMappingView *mSymbolSpikeMappingView = new SymbolSpikeMappingView();
	FeedClientSocketController *lFeedClientSocketController = SingletonFeedClient::GetInstance()->getFeedClientSocketController();

	switch (message)
	{
	// Mouse Selection or double click into List row has been implemented here;
	/*case WM_NOTIFY:
		{
			switch(LOWORD(wParam))
			{
			    case IDC_LISTVIEW_SYMBOL_SPIKE_MAPPING: 
					if(((LPNMHDR)lParam)->code == NM_CLICK)
					{
						mSelectItemOneClickSymbolSpikeMapping=SendMessage(mListViewIdentifierSymbolSpikeMapping,LVM_GETNEXTITEM,-1,LVNI_FOCUSED);
						if(mSelectItemOneClickSymbolSpikeMapping==-1)
						{                      
						  break;
						}
						mIndexSymbolSpikeMapping=mSelectItemOneClickSymbolSpikeMapping;
						mFlagSymbolSpikeMapping=1;
						char tempTextMessage[100]={0};
						sprintf_s(tempTextMessage,"%d",mSelectItemOneClickSymbolSpikeMapping);
						//GetDlgItemText(hWnd,IDC_SHOW_SEL_ROW,tempTextMessage,100);
						SetDlgItemText(hWnd,IDC_SHOW_SEL_ROW,tempTextMessage);
					}
					if( ((LPNMHDR)lParam)->code == NM_DBLCLK )
					{
						mSelectItemOneClickSymbolSpikeMapping=SendMessage(mListViewIdentifierSymbolSpikeMapping,LVM_GETNEXTITEM,-1,LVNI_FOCUSED);
						if(mSelectItemOneClickSymbolSpikeMapping==-1)
						{                      
						  break;
						}
						mIndexSymbolSpikeMapping=mSelectItemOneClickSymbolSpikeMapping;
						mFlagSymbolSpikeMapping=1;
						char tempTextMessage[100]={0};
						sprintf_s(tempTextMessage,"%d",mSelectItemOneClickSymbolSpikeMapping);
						MessageBox(hWnd,tempTextMessage,"TES0001",MB_OK);
					}
					break;
			}
		}
		break;
		*/
	case WM_INITDIALOG:
		mSymbolSpikeMappingView->InitialFillAndDisplayListViewSymbolSpikeMapping(hWnd);
		mSymbolSpikeMappingView->FillAndDisplayListViewSymbolSpikeMapping(hWnd);
		break;
	case WM_COMMAND:
		{
			switch(LOWORD(wParam)) // what we press on?
			{
				case IDC_BUTTON_SPIKE_SETTINGS_NFP:
					{
						if(MessageBox(hWnd,"Do you want to set NFP really?","NFP Spike Setting",MB_YESNO) == IDYES)
						{
							int lDBUpdateStatus = mSymbolSpikeMappingView->SpikeProtectionSettings("SpikeSettingsCSV/spike-settings-nfp.csv");					
							mSymbolSpikeMappingView->FillAndDisplayListViewSymbolSpikeMapping(hWnd);
							int lSendMsgStatus = lFeedClientSocketController->sendSpikeSettingsToServer("NFP");

							if ( lDBUpdateStatus)
							{
								MessageBox(hWnd,"Database has been updated and send info to server for NFP setting.\nNFP settings successfully completed...","NFP Spike Settings",MB_OK|MB_ICONINFORMATION);
								SingletonFeedClient::GetInstance()->getFXTFLog()->LogOutLn("NFP settings successfully completed...UserName: "+SingletonFeedClient::GetInstance()->getUsersModel()->getUsersName());
							}
						}
						else
						{
							//To do nothing;
						}

					}
				break;
				case IDC_BUTTON_SPIKE_SETTINGS_NORMAL:
					{
						if(MessageBox(hWnd,"Do you want to set Normal really?","Normal Spike Setting",MB_YESNO) == IDYES)
						{
							int lDBUpdateStatus = mSymbolSpikeMappingView->SpikeProtectionSettings("SpikeSettingsCSV/spike-settings-normal.csv");					
							mSymbolSpikeMappingView->FillAndDisplayListViewSymbolSpikeMapping(hWnd);
							int lSendMsgStatus = lFeedClientSocketController->sendSpikeSettingsToServer("NORMAL");

							if ( lDBUpdateStatus )
							{
								MessageBox(hWnd,"Database has been updated and send info to server for Normal setting.\n Normal settings successfully completed...","Normal Spike Settings",MB_OK|MB_ICONINFORMATION);
								SingletonFeedClient::GetInstance()->getFXTFLog()->LogOutLn("Normal settings successfully completed.... UserName: "+SingletonFeedClient::GetInstance()->getUsersModel()->getUsersName());
							}
						}
						else
						{
							//To do nothing;
						}
					}	
				break;
				case IDC_BUTTON_SPIKE_SETTINGS_WIDE:
					{
						if(MessageBox(hWnd,"Do you want to set Wide really?","Wide Spike Setting",MB_YESNO) == IDYES)
						{
							int lDBUpdateStatus = mSymbolSpikeMappingView->SpikeProtectionSettings("SpikeSettingsCSV/spike-settings-wide.csv");
							mSymbolSpikeMappingView->FillAndDisplayListViewSymbolSpikeMapping(hWnd);
							int lSendMsgStatus = lFeedClientSocketController->sendSpikeSettingsToServer("HIGH");

							if ( lDBUpdateStatus)
							{
								MessageBox(hWnd,"Database has been updated and send info to server for Wide setting.\n Wide setting successfully completed...","Wide Spike Setting",MB_OK|MB_ICONINFORMATION);
								SingletonFeedClient::GetInstance()->getFXTFLog()->LogOutLn("Wide setting successfully completed... UserName: "+SingletonFeedClient::GetInstance()->getUsersModel()->getUsersName());
							}
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

void SymbolSpikeMappingView::DisplaySymbolSpikeMappingViewWindow(HINSTANCE hInst)
{
	DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_SPIKE_SETTINGS), NULL, DialogSymbolSpikeMapping);
}