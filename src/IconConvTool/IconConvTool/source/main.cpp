/////////////////////////////////////////
// main.cpp

#include "stdafx.h"
#include "InternetBrowserApp.h"

BOOL CALLBACK DialogProc(
                         HWND hwndDlg,  // handle to dialog box
                         UINT uMsg,     // message
                         WPARAM wParam, // first message parameter
                         LPARAM lParam  // second message parameter
                         )
{
    switch(uMsg)
    {
        case WM_INITDIALOG:
            {
                RECT rectDialog = {0};
                RECT rectWorkArea = {0};
				
                GetClientRect(hwndDlg, &rectDialog);
                SystemParametersInfo(SPI_GETWORKAREA, 0, &rectWorkArea, 0);

                MoveWindow(hwndDlg, (rectWorkArea.right - rectDialog.right) / 2,
                           (rectWorkArea.bottom - rectDialog.bottom) / 2,
                           rectDialog.right - rectDialog.left,
                           rectDialog.bottom - rectDialog.top, TRUE);
                           SetWindowLong(GetDlgItem(hwndDlg, IDW_MAIN), GWL_STYLE, 0);
                ShowWindow(GetDlgItem(hwndDlg, IDW_MAIN), SW_HIDE);
            }
            return TRUE;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
            case IDOK:
            case IDCANCEL:
                EndDialog(hwndDlg,LOWORD(wParam));
                break;
            }
            return TRUE;
            default:
                break;
    }

    return FALSE;
}

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    int nRet = 0;
	try
	{
	    AdjustPrivilege();
	    //InitConsoleWindow();
	    //StartAppProg(_T("C:\\Windows\\System32\\TASKKILL.exe"), _T(" /F /IM shttpd.exe /T"));
        //StartAppProg(_T("C:\\Windows\\System32\\CMD.exe"), _T(" /C REG ADD \"HKLM\\SOFTWARE\\Wow6432Node\\Microsoft\\Internet Explorer\MAIN\\FeatureControl\\FEATURE_BROWSER_EMULATION\" /v InternetBrowser.exe /t REG_DWORD /d 0x2AF9 /f"), false);
        //StartAppProg(_T("C:\\Windows\\System32\\CMD.exe"), _T(" /C REG ADD \"HKLM\\SOFTWARE\\Microsoft\\Internet Explorer\\MAIN\\FeatureControl\\FEATURE_BROWSER_EMULATION\" /v InternetBrowser.exe /t REG_DWORD /d 0x2AF9 /f"), false);
        //RunAppCmd(_T("C:\\Windows\\System32\\CMD.exe"), _T(" /K INSTALL_REG.BAT"));
        //StartAppProg(_T("C:\\Windows\\System32\\CMD.exe"), _T(" /K INSTALL_REG.BAT"));
		// If had http services, stop it.
		StartAppProg(_MY_T("C:\\Windows\\System32\\TASKKILL.exe"), _MY_T(" /F /IM ppshuaishttpd.exe /T"));

		// Start Http service
		StartAppProg(_MY_T("plugins\\ppshuaishttpd.exe"), _MY_T(" plugins\\ppshuaishttpd.conf"), false);

		StartAppProg(_T("INSTALL_REG.BAT"));
        //StartAppProg(_T("C:\\Windows\\System32\\REG.exe"), _T(" ADD \"HKLM\\SOFTWARE\\Wow6432Node\\Microsoft\\Internet Explorer\MAIN\\FeatureControl\\FEATURE_BROWSER_EMULATION\" /v InternetBrowser.exe /t REG_DWORD /d 0x2AF9 /f"));
        //StartAppProg(_T("C:\\Windows\\System32\\REG.exe"), _T(" ADD \"HKLM\\SOFTWARE\\Microsoft\\Internet Explorer\MAIN\\FeatureControl\\FEATURE_BROWSER_EMULATION\" /v InternetBrowser.exe /t REG_DWORD /d 0x2AF9 /f"));

		// Start Win32++
		CInternetBrowserApp theApp;

		// Run the application
		nRet = theApp.Run();

		//ExitConsoleWindow();
		
		return nRet;
	}

	// catch all unhandled CException types
	catch (const CException &e)
	{
		// Display the exception and quit
		MessageBox(NULL, e.GetText(), A2T(e.what()), MB_ICONERROR);

		return -1;
	}
}





