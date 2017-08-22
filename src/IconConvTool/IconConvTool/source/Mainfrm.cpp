////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "resource.h"
#include "mainfrm.h"

// Definitions for the CMainFrame class
CMainFrame::CMainFrame()
{
	//Set m_View as the view window of the frame
	SetView(m_View);

	// Set CMainFrame as our event sink
	m_EventSink.SetSink(this);

	// Set the registry key name, and load the initial window position
	// Use a registry key name like "CompanyName\\Application"
	LoadRegistrySettings(_T("InternetBrowser"));
}

CMainFrame::~CMainFrame()
{
	// Destructor for CMainFrame.
}

void CMainFrame::AddComboBoxBand(int Listbox_Height)
{
	// Create the ComboboxEx window
	m_ComboboxEx.Create(GetReBar());
    m_ComboboxEx.LimitText(MAXIMUM_REPARSE_DATA_BUFFER_SIZE - 1);

	// Put the window in a new rebar band
	REBARBANDINFO rbbi;
	ZeroMemory(&rbbi, sizeof(REBARBANDINFO));
	rbbi.cbSize		= sizeof(REBARBANDINFO);
	rbbi.fMask      = RBBIM_COLORS | RBBIM_CHILDSIZE | RBBIM_STYLE | RBBIM_CHILD | RBBIM_TEXT;
	rbbi.cyMinChild = Listbox_Height;
	rbbi.cyMaxChild = Listbox_Height;
	rbbi.cxMinChild = 200;
	rbbi.fStyle     = RBBS_BREAK | RBBS_VARIABLEHEIGHT | RBBS_GRIPPERALWAYS;
	rbbi.clrFore    = GetSysColor(COLOR_BTNTEXT);
	rbbi.clrBack    = GetReBarTheme().clrBand1;
	rbbi.hwndChild  = m_ComboboxEx.GetHwnd();
	rbbi.lpText     = (LPTSTR)_T("Address");

	GetReBar().InsertBand(-1, rbbi);
}

void CMainFrame::ConnectEvents()
{
	IUnknown* pUnk = GetBrowser().GetAXWindow().GetUnknown();
	if(!pUnk)
		return;

	IConnectionPoint* pcp;
	pcp = GetConnectionPoint(DIID_DWebBrowserEvents2);
	if(!pcp)
		return;

	pcp->Advise(&m_EventSink, &m_eventCookie);
	pcp->Release();
	pUnk->Release();
}

IConnectionPoint* CMainFrame::GetConnectionPoint(REFIID riid)
{
	IUnknown *pUnk = GetBrowser().GetAXWindow().GetUnknown();
	if(!pUnk)
		return NULL;

	IConnectionPointContainer* pcpc;
	IConnectionPoint* pcp = NULL;

	HRESULT hr = pUnk->QueryInterface(IID_IConnectionPointContainer, (void**)&pcpc);
	if(SUCCEEDED(hr))
	{
		pcpc->FindConnectionPoint(riid, &pcp);
		pcpc->Release();
	}

	pUnk->Release();

	return pcp;
}

void CMainFrame::OnBeforeNavigate2(DISPPARAMS* pDispParams)
{
	UNREFERENCED_PARAMETER(pDispParams);
}

void CMainFrame::OnCommandStateChange(DISPPARAMS* pDispParams)
{
	CToolBar& TB = GetToolBar();

	if (TB.IsWindow())
	{
		if ((pDispParams) && (pDispParams->cArgs == 2))
		{
			if (pDispParams->rgvarg[1].vt == (VT_I4) && pDispParams->rgvarg[0].vt == (VT_BOOL))
			{
				VARIANT_BOOL bEnable = pDispParams->rgvarg[0].boolVal;
				int nCommand = pDispParams->rgvarg[1].intVal;
				{
					switch (nCommand)
					{
					case 1: // Navigate forward:
						bEnable ? TB.EnableButton(IDM_FORWARD) : TB.DisableButton(IDM_FORWARD);

						break;
					case 2: // Navigate back:
						bEnable ? TB.EnableButton(IDM_BACK) : TB.DisableButton(IDM_BACK);
						break;
					}
				}
			}
		}
	}
}

BOOL CMainFrame::OnFileExit()
{
	// Issue a close request to the frame
	PostMessage(WM_CLOSE);
	return TRUE;
}

BOOL CMainFrame::OnHelpAbout()
{
	// Display the help dialog
	OnHelp();
	return TRUE;
}

BOOL CMainFrame::OnBack()
{
	m_View.GoBack();
	return TRUE;
}

BOOL CMainFrame::OnForward()
{
	m_View.GoForward();
	return TRUE;
}

BOOL CMainFrame::OnRefresh()
{
	m_View.Refresh();
	return TRUE;
}

BOOL CMainFrame::OnStop()
{
	m_View.Stop();
	return TRUE;
}

//遍历元素列表
void EnumHtmlElements(IHTMLDocument2 *pHtmlDocument2)
{
    long lResult = S_OK;
    long lElementIndex = 0;
    long lElementLength = 0;
    VARIANT variantItem = {0};
    variantItem.vt = VT_I4;
    IDispatch * pDispatchItem = NULL;
    IHTMLElement *pHtmlElementItem = NULL;
    IHTMLElementCollection *pHtmlElementCollection = NULL;

    BSTR bstrTitle;
    lResult = pHtmlDocument2->get_title(&bstrTitle);
    _tprintf(_T("bstrTitle:%ws\r\n"), bstrTitle);

    lResult = pHtmlDocument2->get_all(&pHtmlElementCollection);
    _tprintf(_T("pHtmlElementCollection:0x%08X\r\n"), pHtmlElementCollection);
    if(!pHtmlElementCollection)
    {
        return;
    }

    lResult = pHtmlElementCollection->get_length(&lElementLength);
    _tprintf(_T("Element length: %d\r\n"), lElementLength);
    for(lElementIndex = 0; lElementIndex < lElementLength; lElementIndex++)
    {
        variantItem.lVal = lElementIndex;

        lResult = pHtmlElementCollection->item(variantItem, variantItem, &pDispatchItem);
        _tprintf(_T("0x%08X\r\n"), pDispatchItem);

        lResult = pDispatchItem->QueryInterface(IID_IHTMLElement, (void**)&pHtmlElementItem);
        _tprintf(_T("0x%08X\r\n"), pHtmlElementItem);
        BSTR bstrId = NULL;
        BSTR bstrClassName = NULL;
        BSTR bstrAttr = L"";
        VARIANT variantTmp = {0};
        WCHAR wName[16384] = {0};
        variantTmp.vt = VT_BSTR;
        variantTmp.bstrVal = wName;
        BSTR pInnerHtml = NULL;
        //lResult = pHtmlElementItem->get_innerHTML(&pInnerHtml);
        //_tprintf(_T("%ws\r\n"), pInnerHtml);
        lResult = pHtmlElementItem->get_id(&bstrId);
        _tprintf(_T("==%ws\r\n"), bstrId);
        if(bstrId && wcscmp(bstrId, L"username") == 0)
        {
            _tprintf(_T("============Found %ws\r\n"), bstrId);
            VARIANT variantUserName;
            variantUserName.vt = VT_BSTR;
            variantUserName.bstrVal = L"22sd55";
            pHtmlElementItem->setAttribute(L"value", variantUserName, 0);
            //break;
        }
        else if(bstrId && wcscmp(bstrId, L"passwd") == 0)
        {
            _tprintf(_T("============Found %ws\r\n"), bstrId);
            VARIANT variantPassWord;
            variantPassWord.vt = VT_BSTR;
            variantPassWord.bstrVal = L"asd123";
            pHtmlElementItem->setAttribute(L"value", variantPassWord, 0);
            //break;
        }

        bstrAttr = L"className";
        lResult = pHtmlElementItem->getAttribute(bstrAttr, 0, &variantTmp);
        _tprintf(_T("==%ws\r\n"), variantTmp.bstrVal);
        lResult = pHtmlElementItem->get_className(&bstrClassName);
        _tprintf(_T("==%ws\r\n"), bstrClassName);
        //lResult = pHtmlElementItem->get_tagName(&bstrClassName);
        //_tprintf(_T("%ws\r\n"), bstrClassName);
        if(bstrClassName && wcscmp(bstrClassName, L"za_button") == 0)
        {
            _tprintf(_T("============Found %ws\r\n"), bstrClassName);
            pHtmlElementItem->click();
            break;
        }
        else if(bstrClassName && wcscmp(bstrClassName, L"browser_nextBTN") == 0)
        {
            _tprintf(_T("============Found %ws\r\n"), bstrClassName);
            pHtmlElementItem->click();
            break;
        }

        if(pHtmlElementItem)
        {
            pHtmlElementItem->Release();
            pHtmlElementItem = NULL;
        }
        if(pDispatchItem)
        {
            pDispatchItem->Release();
            pDispatchItem = NULL;
        }
    }
    if(pHtmlElementCollection)
    {
        pHtmlElementCollection->Release();
        pHtmlElementCollection = NULL;
    }
}
//遍历框架
void EnumHtmlFrames(IHTMLDocument2 *pHtmlDocument2)
{
    long lResult = S_OK;
    long lFramesIndex = 0;
    long lFramesLength = 0;

    VARIANT variantItem;
    VARIANT variantResult;
    variantItem.vt = VT_I4;
    variantResult.vt = VT_DISPATCH;
    IDispatch * pDispatchItem = NULL;
    IHTMLWindow2 *pHtmlWindow2Item = NULL;
    IHTMLDocument2 *pHtmlDocument2Item = NULL;
    IHTMLFramesCollection2 *pHtmlFramesCollection2 = NULL;

    EnumHtmlElements(pHtmlDocument2);

    pHtmlDocument2->get_frames((IHTMLFramesCollection2 **)&pHtmlFramesCollection2);
    pHtmlFramesCollection2->get_length(&lFramesLength);
    _tprintf(_T("Frames length: %d\r\n"), lFramesLength);
    for(lFramesIndex = 0; lFramesIndex < lFramesLength; lFramesIndex++)
    {
        variantItem.lVal = lFramesIndex;

        lResult = pHtmlFramesCollection2->item(&variantItem, &variantResult);
        if(!variantResult.ppdispVal)
        {
            printf("ppdispVal=null--0x%08X\r\n", lResult);
            continue;
        }

        pDispatchItem = (IDispatch *)variantResult.ppdispVal;
        //IHTMLFrameBase2 * pHtmlFrameBase2 = NULL;
        //pDispatchItem->QueryInterface(IID_IHTMLFrameBase2, (void **)&pHtmlFrameBase2);
        //pHtmlFrameBase2->get_contentWindow(&pHtmlWindow2);
        lResult = pDispatchItem->QueryInterface(IID_IHTMLWindow2, (void **)&pHtmlWindow2Item);
        if(pHtmlWindow2Item == NULL)
        {
            printf("pHtmlWindow2Item=null--0x%08X\r\n", lResult);
            continue;
        }
        lResult = pHtmlWindow2Item->get_document(&pHtmlDocument2Item);
        if(pHtmlDocument2Item == NULL)
        {
            printf("pHtmlDocument2Item=null--0x%08X\r\n", lResult);
            continue;
        }

        //EnumHtmlElements(pHtmlDocument2Item);
        EnumHtmlFrames(pHtmlDocument2Item);

        if(pHtmlDocument2Item)
        {
            pHtmlDocument2Item->Release();
            pHtmlDocument2Item = NULL;
        }
        if(pHtmlWindow2Item)
        {
            pHtmlWindow2Item->Release();
            pHtmlWindow2Item = NULL;
        }
        if(pDispatchItem)
        {
            pDispatchItem->Release();
            pDispatchItem = NULL;
        }
    }
    if(pHtmlFramesCollection2)
    {
        pHtmlFramesCollection2->Release();
        pHtmlFramesCollection2 = NULL;
    }
}
BOOL CMainFrame::OnHome()
{
	m_View.GoHome();
    //m_View.Navigate2(_T("http://180.94.224.40/"));
	IDispatch *pDispatch = NULL;

    IWebBrowser2 *pWebBrowser2 = NULL;

    IHTMLDocument2 *pHtmlDocument2 = NULL;

    pWebBrowser2 = m_View.GetIWebBrowser2();
    pWebBrowser2->get_Document(&pDispatch);
    pDispatch->QueryInterface(IID_IHTMLDocument2, (void**)&pHtmlDocument2);

    EnumHtmlFrames(pHtmlDocument2);

    //BSTR bstrScript = L"login_fun();";
    //BSTR bstrScriptProperty = L"JavaScript";
    //VARIANT variantValue = {0};
    //pHtmlWindow2->execScript(bstrScript, bstrScriptProperty, &variantValue);
    //pHtmlWindow2->eval(bstrScript, bstrScriptProperty, &variantValue);
    //::MessageBox(NULL, _T("123"), _T("123"), MB_OK);

    if(pHtmlDocument2)
    {
        pHtmlDocument2->Release();
        pHtmlDocument2 = NULL;
    }
    if(pDispatch)
    {
        pDispatch->Release();
        pDispatch = NULL;
    }

	return TRUE;
}

BOOL CMainFrame::OnEditCut()
{
	if (GetFocus() == *GetComboEdit())
		GetComboEdit()->Cut();
	else
		m_View.ExecWB( OLECMDID_CUT, OLECMDEXECOPT_DODEFAULT, NULL, NULL );

	return TRUE;
}

BOOL CMainFrame::OnEditCopy()
{
	if (GetFocus() == *GetComboEdit())
		GetComboEdit()->Copy();
	else
		m_View.ExecWB( OLECMDID_COPY, OLECMDEXECOPT_DODEFAULT, NULL, NULL );

	return TRUE;
}

BOOL CMainFrame::OnEditPaste()
{
	if (GetFocus() == *GetComboEdit())
		GetComboEdit()->Paste();
	else
		m_View.ExecWB( OLECMDID_PASTE, OLECMDEXECOPT_DODEFAULT, NULL, NULL );

	return TRUE;
}

BOOL CMainFrame::OnEditDelete()
{
#if defined(__GNUC__)
	OLECMDID OLECMDID_DELETE = (OLECMDID)33;
#endif

	if (GetFocus() == *GetComboEdit())
		GetComboEdit()->Clear();
	else
		m_View.ExecWB( OLECMDID_DELETE, OLECMDEXECOPT_DODEFAULT, NULL, NULL );

	return TRUE;
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// Respond to menu and and toolbar input
	switch(LOWORD(wParam))
	{
	case IDM_FILE_EXIT:		 return OnFileExit();
	case IDM_HELP_ABOUT:	 return OnHelpAbout();
	case IDM_BACK:			 return OnBack();
	case IDM_FORWARD:		 return OnForward();
	case IDM_REFRESH:		 return OnRefresh();
	case IDM_STOP:			 return OnStop();
	case IDM_HOME:			 return OnHome();
	case IDM_EDIT_CUT:		 return OnEditCut();
	case IDM_EDIT_COPY:		 return OnEditCopy();
	case IDM_EDIT_PASTE:	 return OnEditPaste();
	case IDM_EDIT_DELETE:	 return OnEditDelete();
	case IDW_VIEW_STATUSBAR: return OnViewStatusBar();
	case IDW_VIEW_TOOLBAR:	 return OnViewToolBar();
	}

	// Handle notification WM_COMMAND from ComboboxEx
	if((HWND)lParam == m_ComboboxEx.GetHwnd())
	{
		switch(HIWORD(wParam))
		{
		case CBN_SELCHANGE:
			// User made selection from list
			{
				// Get text from edit box
				CString strText = m_ComboboxEx.GetWindowText();

				m_ComboboxEx.AddUsedtoList(strText);

				// Navigate to web page
				m_View.Navigate(strText);
			}
			return TRUE;
		}
	}

	return FALSE;
}

void CMainFrame::OnDocumentBegin(DISPPARAMS* pDispParams)
{
	UNREFERENCED_PARAMETER(pDispParams);
	TRACE("OnDocumentBegin\n");
}

int CMainFrame::OnCreate(CREATESTRUCT& cs)
{
	// OnCreate controls the way the frame is created.
	// Overriding CFrame::OnCreate is optional.
	// Uncomment the lines below to change frame options.

	SetUseIndicatorStatus(FALSE);	// Don't show keyboard indicators in the StatusBar
	SetUseMenuStatus(FALSE);			// Don't show menu descriptions in the StatusBar
	SetUseReBar(FALSE);				// Don't use a ReBar
	SetUseThemes(FALSE);				// Don't use themes
	SetUseToolBar(FALSE);			// Don't use a ToolBar

	// Call the base function first
	CFrame::OnCreate(cs);
	CFrame::SetFrameMenu((HMENU)NULL);

	ConnectEvents();

	return 0;
}

void CMainFrame::OnDocumentComplete(DISPPARAMS* pDispParams)
{
	UNREFERENCED_PARAMETER(pDispParams);
	GetStatusBar().SetPartText(0, _T("Done"));
}

void CMainFrame::OnDownloadBegin(DISPPARAMS* pDispParams)
{
	UNREFERENCED_PARAMETER(pDispParams);
}

void CMainFrame::OnDownloadComplete(DISPPARAMS* pDispParams)
{
	UNREFERENCED_PARAMETER(pDispParams);
}

void CMainFrame::OnInitialUpdate()
{
	// The frame is now created.
	// Place any additional startup code here.

	//m_View.GoHome();
	m_View.Navigate(_T(DEFAULT_URLPATH));

	//m_ComboboxEx.SetWindowText(_T(DEFAULT_URLPATH));
    //this->SendMessage(WM_COMMAND, (WPARAM)MAKEWPARAM(IDC_COMBOBOXEX,CBN_SELCHANGE), (LPARAM)m_ComboboxEx.GetHwnd());
}

void CMainFrame::OnNavigateComplete2(DISPPARAMS* pDispParams)
{
	CString str = _T("NavigateComplete2: ");

	if (pDispParams->rgvarg[0].vt == (VT_BYREF|VT_VARIANT))
	{
		VARIANT vtURL;
		vtURL = *pDispParams->rgvarg[0].pvarVal;
		vtURL.vt = VT_BSTR;

		str += vtURL.bstrVal;
		TRACE(str);
		VariantClear(&vtURL);
	}

	BSTR bstrUrlName;

	HRESULT hr = GetBrowser().GetIWebBrowser2()->get_LocationURL(&bstrUrlName);
	if (FAILED(hr))
		return;

	// Update the URL in the ComboboxEx edit box.
	//m_ComboboxEx.SetWindowText(W2T(bstrUrlName));
	m_View.SetFocus();
}

void CMainFrame::OnNewWindow2(DISPPARAMS* pDispParams)
{
	UNREFERENCED_PARAMETER(pDispParams);
	TRACE("NewWindow2\n");
}

LRESULT CMainFrame::OnNotify(WPARAM wParam, LPARAM lParam)
{
	switch (((LPNMHDR)lParam)->code)
	{
	case CBEN_ENDEDIT:
		{
			switch (((PNMCBEENDEDIT)lParam)->iWhy)
			{
			case CBENF_RETURN:
				// User hit return in edit box
				{
					// Get text from edit box
					CString strText = m_ComboboxEx.GetWindowText();

					m_ComboboxEx.AddUsedtoList(strText);

					// Navigate to the web page
					m_View.Navigate(strText);
					return FALSE;
				}
			}
		}
	}

	return CFrame::OnNotify(wParam, lParam);
}

void CMainFrame::OnProgressChange(DISPPARAMS* pDispParams)
{
	CString str;

	if (pDispParams->cArgs != 0)
	{
		if (pDispParams->cArgs > 1 && pDispParams->rgvarg[1].vt == VT_I4)
		{
			str.Format(_T("Progress = %d"), pDispParams->rgvarg[1].lVal);
		}

		if (pDispParams->rgvarg[0].vt == VT_I4)
		{
			CString str2;
			str2.Format(_T(", ProgressMax = %d\n"), pDispParams->rgvarg[0].lVal);
			str = str + str2;
		}

		TRACE(str);
   }
}

void CMainFrame::OnPropertyChange(DISPPARAMS* pDispParams)
{
	CString str;
	if (pDispParams->cArgs > 0 && pDispParams->rgvarg[0].vt == VT_BSTR)
		str.Format(_T("Property Change: %s\n"), (LPCTSTR)W2T(pDispParams->rgvarg[0].bstrVal));

	TRACE(str);
}

void CMainFrame::OnStatusTextChange(DISPPARAMS* pDispParams)
{
	LPOLESTR lpStatusText = pDispParams->rgvarg->bstrVal;

	if (GetStatusBar().IsWindow() && lpStatusText)
	{
        tstring tsStatusText = _MY_T("");
	    tsStatusText = W2T(lpStatusText);
		if (lstrcmp(W2T(lpStatusText), _T("")))
		{
		    _tprintf(_T("Current Request URL:%s\r\n"), tsStatusText.c_str());
		    if(tsStatusText.length())
            {
                tstring tsTmpUrl = _MY_T("");
                tstring tsIdInfo = _MY_T("");
                tstring::size_type stPos = tstring::npos;

                tsIdInfo = _MY_T("/iovation/vindex.php");
                stPos = m_tsURL.find(tsIdInfo);
                if(stPos != tstring::npos)
                {
                    stPos = tsStatusText.find(_MY_T("完成"));
                    if(stPos != tstring::npos)
                    {
                        this->OnHome();
                    }
                }
                tsIdInfo = _MY_T("app/member/FT_browse/body_var.php?uid=");
                stPos = tsStatusText.find(tsIdInfo);
                if(stPos != tstring::npos)
                {
                    IDispatch *pDispatch = NULL;

                    IWebBrowser2 *pWebBrowser2 = NULL;

                    IHTMLDocument2 *pHtmlDocument2 = NULL;

                    pWebBrowser2 = m_View.GetIWebBrowser2();
                    pWebBrowser2->get_Document(&pDispatch);
                    pDispatch->QueryInterface(IID_IHTMLDocument2, (void**)&pHtmlDocument2);
                    BSTR bstrCookie = NULL;
                    pHtmlDocument2->get_cookie(&bstrCookie);
                    FILE *pFile = fopen("cookie.dat", "wb");
                    if(pFile)
                    {
                        _ftprintf(pFile, _MY_T("%ws"), bstrCookie);
                        fclose(pFile);
                        pFile = 0;
                    }
                    if(pHtmlDocument2)
                    {
                        pHtmlDocument2->Release();
                        pHtmlDocument2 = NULL;
                    }
                    if(pDispatch)
                    {
                        pDispatch->Release();
                        pDispatch = NULL;
                    }

                    tstring tsStartText = _MY_T("http://");
                    tstring tsFinalText = _MY_T("&rtype=r&langx=zh-cn&mtype=3&page_no=0&league_id=&hot_game=");
                    tstring::size_type stStartPos = tstring::npos;
                    tstring::size_type stFinalPos = tstring::npos;
                    stStartPos = tsStatusText.find(tsStartText);
                    stFinalPos = tsStatusText.find(tsFinalText);
                    if(stStartPos != tstring::npos && stFinalPos != tstring::npos && stFinalPos > stStartPos)
                    {
                        tsTmpUrl = tsStatusText.substr(stStartPos, stFinalPos - stStartPos + tsFinalText.length());
                        FILE *pF = fopen("url.txt", "wb");
                        if(pF)
                        {
                            _ftprintf(pF, _T("%s"), tsTmpUrl.c_str());
                            fclose(pF);
                            pF = 0;
                        }
                    }
                }


                m_tsURL = tsStatusText;//记录本次的URL地址
            }
			GetStatusBar().SetPartText(0, W2T(lpStatusText));
		}
		else
			GetStatusBar().SetPartText(0, _T("Done"));
	}
}

void CMainFrame::OnTitleChange(DISPPARAMS* pDispParams)
{
	TRACE("TitleChange: \n");
    CString str;

	if (pDispParams->cArgs > 0 && pDispParams->rgvarg[0].vt == VT_BSTR)
	{
        str = CString(pDispParams->rgvarg[0].bstrVal) + _T(" - ") + LoadString(IDW_MAIN);
		TRACE(W2T(pDispParams->rgvarg[0].bstrVal));
	}
	else
		str = LoadString(IDW_MAIN);

	SetWindowText(str);
}

void CMainFrame::SetupToolBar()
{
	// Set the Resource IDs for the toolbar buttons
	AddToolBarButton( IDM_BACK   );
	AddToolBarButton( IDM_FORWARD );
	AddToolBarButton( 0 );				// Separator
	AddToolBarButton( IDM_REFRESH );
	AddToolBarButton( IDM_STOP );
	AddToolBarButton( 0 );				// Separator
	AddToolBarButton( IDM_HOME );

	// Set the image lists for normal, hot and disabled buttons
    int BitsPerPixel = GetDesktopWindow().GetDC().GetDeviceCaps(BITSPIXEL);
	if (GetWinVersion() >= 2501 && BitsPerPixel == 32)
	{
		// Load the 32bit bitmaps if we can, otherwise load 24bit ones.
		CBitmap bm(IDB_TOOLBAR32_NORM);
		if (bm.GetHandle())
			SetToolBarImages(RGB(0,0,0), IDB_TOOLBAR32_NORM, IDB_TOOLBAR32_HOT, IDB_TOOLBAR32_DIS);
		else
			SetToolBarImages(RGB(255,0,255), IDB_TOOLBAR24_NORM, IDB_TOOLBAR24_HOT, IDB_TOOLBAR24_DIS);
	}
	else
	{
		// Use 24bit bitmaps for Win2000 and below
		SetToolBarImages(RGB(255,0,255), IDB_TOOLBAR24_NORM, IDB_TOOLBAR24_HOT, IDB_TOOLBAR24_DIS);
	}

	std::vector<UINT> IconData;
	if (IsReBarUsed())
	{
		int Height = 22;
		AddComboBoxBand(Height);

		// Set the icons for popup menu items
		IconData.push_back ( IDM_FILE_NEW  );
		IconData.push_back ( IDM_FILE_OPEN );
		IconData.push_back ( IDM_FILE_SAVE );
		IconData.push_back ( IDM_EDIT_CUT  );
		IconData.push_back ( IDM_EDIT_COPY );
		IconData.push_back ( IDM_EDIT_PASTE);
		IconData.push_back ( IDM_FILE_PRINT);
		IconData.push_back ( IDM_HELP_ABOUT);
	}

	AddMenuIcons(IconData, RGB(192, 192, 192), IDW_MAIN, 0);
}

LRESULT CMainFrame::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{
		//Additional messages to be handled go here
//	}

	// pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}

