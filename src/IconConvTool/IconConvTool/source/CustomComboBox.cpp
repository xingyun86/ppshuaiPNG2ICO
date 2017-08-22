//////////////////////////////////////////////
// ComboBoxEx.cpp

#include "stdafx.h"
#include "InternetBrowserApp.h"
#include "CustomComboBox.h"

void CCustomComboBox::PreCreate(CREATESTRUCT& cs)
{
	cs.style = WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | WS_CLIPCHILDREN;
	cs.cy = 100;	// required to display list
	cs.hMenu = (HMENU)IDC_COMBOBOXEX;
}

void CCustomComboBox::AddUsedtoList(CString &strText)
{
    int nIdx = 0;
    bool bFound = false;
    int nCount = this->GetCount();
    for(nIdx = 0; nIdx < nCount; nIdx++)
    {
        _TCHAR tTmp[MAXIMUM_REPARSE_DATA_BUFFER_SIZE] = {0};
        this->GetLBText(nIdx, tTmp);
        if(!strText.CompareNoCase(tTmp))
        {
            bFound = true;
            break;
        }
    }

    if(!bFound)
    {
        // Insert text into the list box.
        COMBOBOXEXITEM CBXitem;
        ZeroMemory(&CBXitem, sizeof(COMBOBOXEXITEM));
        CBXitem.mask = CBEIF_TEXT;
        CBXitem.pszText = (LPTSTR)strText.c_str();
        this->InsertItem(&CBXitem);
    }
}

