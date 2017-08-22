#pragma once

#ifndef __USUALLYUTILITY_H_
#define __USUALLYUTILITY_H_


#include <string>
#include <vector>

#include <stdio.h>
#include <tchar.h>
#include <windows.h>
using namespace std;

#define SYSTEM_PATH "\\SYSTEM32\\"
#define ADB_PATH    "\\ADB\\"

#define ADB_NAME    "ADB.EXE"
#define CMD_NAME    "CMD.EXE"

#if !defined(UNICODE) && !defined(_UNICODE)
#define TSTRING std::string
#else
#define TSTRING std::wstring
#endif

#define _tstring TSTRING
#define tstring TSTRING

//初始化调试窗口显示
__inline void InitDebugConsole(const _TCHAR * ptText = _T("TraceDebugWindow"))
{
	if (!::AllocConsole())
	{
		_TCHAR tErrorInfos[16384] = { 0 };
		_sntprintf(tErrorInfos, sizeof(tErrorInfos) / sizeof(_TCHAR), _T("控制台生成失败! 错误代码:0x%X。"), GetLastError());
		::MessageBox(NULL, tErrorInfos, _T("错误提示"), 0);
		return;
	}
	::SetConsoleTitle(ptText);

	_tfreopen(_T("CONOUT$"), _T("wb"), stdout);
	_tfreopen(_T("CONIN$"), _T("rb"), stdin);
	_tfreopen(_T("CONERR$"), _T("wb"), stderr);
	_tsetlocale(LC_ALL, _T("chs"));
}

//释放掉调试窗口显示
__inline void ExitDebugConsole()
{
    fclose(stderr);
    fclose(stdin);
    fclose(stdout);

	::FreeConsole();
}

//	ANSI to Unicode
__inline std::wstring ANSIToUnicode(const std::string& str)
{
	int len = 0;
	len = str.length();
	int unicodeLen = ::MultiByteToWideChar(CP_ACP,
		0,
		str.c_str(),
		-1,
		NULL,
		0);
	wchar_t * pUnicode;
	pUnicode = new  wchar_t[(unicodeLen + 1)];
	memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP,
		0,
		str.c_str(),
		-1,
		(LPWSTR)pUnicode,
		unicodeLen);
	std::wstring rt;
	rt = (wchar_t*)pUnicode;
	delete pUnicode;
	return rt;
}

//Unicode to ANSI
__inline std::string UnicodeToANSI(const std::wstring& str)
{
	char* pElementText;
	int iTextLen;
	iTextLen = WideCharToMultiByte(CP_ACP,
		0,
		str.c_str(),
		-1,
		NULL,
		0,
		NULL,
		NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, sizeof(char) * (iTextLen + 1));
	::WideCharToMultiByte(CP_ACP,
		0,
		str.c_str(),
		-1,
		pElementText,
		iTextLen,
		NULL,
		NULL);
	std::string strText;
	strText = pElementText;
	delete[] pElementText;
	return strText;
}
//UTF - 8 to Unicode
__inline std::wstring UTF8ToUnicode(const std::string& str)
{
	int len = 0;
	len = str.length();
	int unicodeLen = ::MultiByteToWideChar(CP_UTF8,
		0,
		str.c_str(),
		-1,
		NULL,
		0);
	wchar_t * pUnicode;
	pUnicode = new wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
	::MultiByteToWideChar(CP_UTF8,
		0,
		str.c_str(),
		-1,
		(LPWSTR)pUnicode,
		unicodeLen);
	std::wstring rt;
	rt = (wchar_t*)pUnicode;
	delete pUnicode;
	return rt;
}
//Unicode to UTF - 8
__inline std::string UnicodeToUTF8(const std::wstring& str)
{
	char*   pElementText;
	int iTextLen;
	iTextLen = WideCharToMultiByte(CP_UTF8,
		0,
		str.c_str(),
		-1,
		NULL,
		0,
		NULL,
		NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, sizeof(char) * (iTextLen + 1));
	::WideCharToMultiByte(CP_UTF8,
		0,
		str.c_str(),
		-1,
		pElementText,
		iTextLen,
		NULL,
		NULL);
	std::string strText;
	strText = pElementText;
	delete[] pElementText;
	return strText;
}

__inline std::string TToA(tstring tsT)
{
    std::string str = "";

    #if !defined(UNICODE) && !defined(_UNICODE)
    str = tsT;
    #else
    str = UnicodeToANSI(tsT);
    #endif

    return str;
}

__inline std::wstring TToW(tstring tsT)
{
    std::wstring wstr = L"";

    #if !defined(UNICODE) && !defined(_UNICODE)
    wstr = ANSIToUnicode(tsT);
    #else
    wstr = tsT;
    #endif

    return wstr;
}

__inline tstring AToT(std::string str)
{
    tstring ts = _T("");

    #if !defined(UNICODE) && !defined(_UNICODE)
    ts = str;
    #else
    ts = ANSIToUnicode(str);
    #endif

    return ts;
}

__inline tstring WToT(std::wstring wstr)
{
    tstring ts = _T("");

    #if !defined(UNICODE) && !defined(_UNICODE)
    ts = UnicodeToANSI(wstr);
    #else
    ts = wstr;
    #endif

    return ts;
}

//显示在屏幕中央
__inline void CenterWindowInScreen(HWND hWnd)
{
	RECT rcWindow = { 0 };
	RECT rcScreen = { 0 };
	SIZE szAppWnd = { 300, 160 };
	POINT ptAppWnd = { 0, 0 };

	// Get workarea rect.
	BOOL fResult = SystemParametersInfo(SPI_GETWORKAREA,   // Get workarea information
		0,              // Not used
		&rcScreen,    // Screen rect information
		0);             // Not used

	GetWindowRect(hWnd, &rcWindow);
	szAppWnd.cx = rcWindow.right - rcWindow.left;
	szAppWnd.cy = rcWindow.bottom - rcWindow.top;

	//居中显示
	ptAppWnd.x = (rcScreen.right - rcScreen.left - szAppWnd.cx) / 2;
	ptAppWnd.y = (rcScreen.bottom - rcScreen.top - szAppWnd.cy) / 2;
	MoveWindow(hWnd, ptAppWnd.x, ptAppWnd.y, szAppWnd.cx, szAppWnd.cy, TRUE);
}

//utf8 转 Unicode
__inline std::wstring Utf82Unicode(const std::string& utf8string)
{
	int widesize = ::MultiByteToWideChar(CP_UTF8, 0, utf8string.c_str(), -1, NULL, 0);
	if (widesize == ERROR_NO_UNICODE_TRANSLATION || widesize == 0)
	{
		return std::wstring(L"");
	}

	std::vector<wchar_t> resultstring(widesize);

	int convresult = ::MultiByteToWideChar(CP_UTF8, 0, utf8string.c_str(), -1, &resultstring[0], widesize);

	if (convresult != widesize)
	{
		return std::wstring(L"");
	}

	return std::wstring(&resultstring[0]);
}

//unicode 转为 ascii
__inline std::string WideByte2Acsi(std::wstring& wstrcode)
{
	int asciisize = ::WideCharToMultiByte(CP_OEMCP, 0, wstrcode.c_str(), -1, NULL, 0, NULL, NULL);
	if (asciisize == ERROR_NO_UNICODE_TRANSLATION || asciisize == 0)
	{
		return std::string("");
	}
	std::vector<char> resultstring(asciisize);
	int convresult = ::WideCharToMultiByte(CP_OEMCP, 0, wstrcode.c_str(), -1, &resultstring[0], asciisize, NULL, NULL);

	if (convresult != asciisize)
	{
		return std::string("");
	}

	return std::string(&resultstring[0]);
}

//utf-8 转 ascii
__inline std::string UTF_82ASCII(std::string& strUtf8Code)
{
	std::string strRet("");
	//先把 utf8 转为 unicode
	std::wstring wstr = Utf82Unicode(strUtf8Code);
	//最后把 unicode 转为 ascii
	strRet = WideByte2Acsi(wstr);
	return strRet;
}

///////////////////////////////////////////////////////////////////////


//ascii 转 Unicode
__inline std::wstring Acsi2WideByte(std::string& strascii)
{
	int widesize = MultiByteToWideChar(CP_ACP, 0, (char*)strascii.c_str(), -1, NULL, 0);
	if (widesize == ERROR_NO_UNICODE_TRANSLATION || widesize == 0)
	{
		return std::wstring(L"");
	}
	std::vector<wchar_t> resultstring(widesize);
	int convresult = MultiByteToWideChar(CP_ACP, 0, (char*)strascii.c_str(), -1, &resultstring[0], widesize);


	if (convresult != widesize)
	{
		return std::wstring(L"");
	}

	return std::wstring(&resultstring[0]);
}


//Unicode 转 Utf8
__inline std::string Unicode2Utf8(const std::wstring& widestring)
{
	int utf8size = ::WideCharToMultiByte(CP_UTF8, 0, widestring.c_str(), -1, NULL, 0, NULL, NULL);
	if (utf8size == 0)
	{
		return std::string("");
	}

	std::vector<char> resultstring(utf8size);

	int convresult = ::WideCharToMultiByte(CP_UTF8, 0, widestring.c_str(), -1, &resultstring[0], utf8size, NULL, NULL);

	if (convresult != utf8size)
	{
		return std::string("");
	}

	return std::string(&resultstring[0]);
}

//ascii 转 Utf8
__inline std::string ASCII2UTF_8(std::string& strAsciiCode)
{
	std::string strRet("");
	//先把 ascii 转为 unicode
	std::wstring wstr = Acsi2WideByte(strAsciiCode);
	//最后把 unicode 转为 utf8
	strRet = Unicode2Utf8(wstr);
	return strRet;
}

__inline int code_convert(char *from_charset, char *to_charset, const char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
	/*const char **pin = &inbuf;
	char **pout = &outbuf;

	iconv_t cd = iconv_open(to_charset, from_charset);
	if (cd == 0) return -1;
	memset(outbuf, 0, outlen);
	if (iconv(cd, (char **)pin, &inlen, pout, &outlen) == -1) return -1;
	iconv_close(cd);*/
	return 0;
}

/* UTF-8 to GBK  */
__inline int u2g(const char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
	return code_convert("UTF-8", "GBK", inbuf, inlen, outbuf, outlen);
}

/* GBK to UTF-8 */
__inline int g2u(const char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
	return code_convert("GBK", "UTF-8", inbuf, inlen, outbuf, outlen);
}

//显示在父窗口中央
__inline void CenterWindowInParent(HWND hWnd, HWND hParentWnd)
{
	RECT rcWindow = { 0 };
	RECT rcParent = { 0 };
	SIZE szAppWnd = { 300, 160 };
	POINT ptAppWnd = { 0, 0 };

	GetWindowRect(hParentWnd, &rcParent);
	GetWindowRect(hWnd, &rcWindow);
	szAppWnd.cx = rcWindow.right - rcWindow.left;
	szAppWnd.cy = rcWindow.bottom - rcWindow.top;

	//居中显示
	ptAppWnd.x = (rcParent.right - rcParent.left - szAppWnd.cx) / 2;
	ptAppWnd.y = (rcParent.bottom - rcParent.top - szAppWnd.cy) / 2;
	MoveWindow(hWnd, ptAppWnd.x, ptAppWnd.y, szAppWnd.cx, szAppWnd.cy, TRUE);
}

//传入应用程序文件名称、参数、启动类型及等待时间启动程序
typedef enum LaunchType {
	LTYPE_0 = 0, //立即
	LTYPE_1 = 1, //直等
	LTYPE_2 = 2, //延迟(设定等待时间)
}LAUNCHTYPE;

//传入应用程序文件名称、参数、启动类型及等待时间启动程序
__inline static BOOL LaunchAppProg(tstring tsAppProgName, tstring tsArguments = _T(""), bool bNoUI = true, LAUNCHTYPE type = LTYPE_0, DWORD dwWaitTime = WAIT_TIMEOUT)
{
	BOOL bRet = FALSE;
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	DWORD dwCreateFlags = CREATE_NO_WINDOW;
	LPTSTR lpArguments = NULL;

	if (tsArguments.length())
	{
		lpArguments = (LPTSTR)tsArguments.c_str();
	}

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	if (!bNoUI)
	{
		dwCreateFlags = 0;
	}

	// Start the child process.
	bRet = CreateProcess(tsAppProgName.c_str(),   // No module name (use command line)
		lpArguments,        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		dwCreateFlags,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory
		&si,            // Pointer to STARTUPINFO structure
		&pi);           // Pointer to PROCESS_INFORMATION structure
	if (bRet)
	{
		switch (type)
		{
		case LTYPE_0:
		{
			// No wait until child process exits.
		}
		break;
		case LTYPE_1:
		{
			// Wait until child process exits.
			WaitForSingleObject(pi.hProcess, INFINITE);
		}
		break;
		case LTYPE_2:
		{
			// Wait until child process exits.
			WaitForSingleObject(pi.hProcess, dwWaitTime);
		}
		break;
		default:
			break;
		}


		// Close process and thread handles.
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	else
	{
		printf("CreateProcess failed (%d).\n", GetLastError());
	}
	return bRet;
}
__inline DWORD GetProgramDir(_TCHAR tFilePath[MAX_PATH] = _T(""))
{
    DWORD dwSize = 0;
    _TCHAR * pT = NULL;
    dwSize = GetModuleFileName(GetModuleHandle(NULL), tFilePath, MAX_PATH);
    if(dwSize)
    {
        pT = _tcsrchr(tFilePath, _T('\\'));
        if(pT)
        {
            *pT = _T('\0');
            dwSize = (DWORD)(pT - tFilePath);
        }
    }
    return dwSize;
}

__inline UINT GetSystemDir(_TCHAR tFilePath[MAX_PATH] = _T(""))
{
    UINT uSize = 0;
    _TCHAR * pT = NULL;
    uSize = GetSystemDirectory(tFilePath, MAX_PATH);
    if(uSize)
    {
        pT = _tcsrchr(tFilePath, _T('\\'));
        if(pT)
        {
            *pT = _T('\0');
            uSize = (DWORD)(pT - tFilePath);
        }
    }
    return uSize;
}

__inline void DebugTrace(LPCTSTR lpszFormat, ...)
{
#if !defined(MAX_DEBUGTRACE_NUM)
#define MAX_DEBUGTRACE_NUM 65536
	va_list args;
	tstring tstr(MAX_DEBUGTRACE_NUM, _T('\0'));
	va_start(args, lpszFormat);
	_vsntprintf((_TCHAR *)tstr.c_str(), tstr.size(), lpszFormat, args);
	OutputDebugString(tstr.c_str());
	va_end(args);
#undef MAX_DEBUGTRACE_NUM
#endif // MAX_DEBUGTRACE_NUM
}

#define DEBUG_TRACE DebugTrace


__inline void LogDebugPrint(LPCTSTR lpszFormat, ...)
{
#if !defined(MAX_DEBUGPRINT_NUM)
#define MAX_DEBUGPRINT_NUM 65536
	va_list args;
	tstring tstr(MAX_DEBUGPRINT_NUM, _T('\0'));
	va_start(args, lpszFormat);
	_vsntprintf((_TCHAR *)tstr.c_str(), tstr.size(), lpszFormat, args);
	_ftprintf(stdout, _T("%s"), tstr.c_str());
	va_end(args);
#undef MAX_DEBUGPRINT_NUM
#endif // MAX_DEBUGPRINT_NUM
}

#define LOG_DEBUG_PRINT LogDebugPrint

__inline void LogErrorPrint(LPCTSTR lpszFormat, ...)
{
#if !defined(MAX_ERRORPRINT_NUM)
#define MAX_ERRORPRINT_NUM 65536
	va_list args;
	tstring tstr(MAX_ERRORPRINT_NUM, _T('\0'));
	va_start(args, lpszFormat);
	_vsntprintf((_TCHAR *)tstr.c_str(), tstr.size(), lpszFormat, args);
	_ftprintf(stderr, _T("%s"), tstr.c_str());
	va_end(args);
#undef MAX_ERRORPRINT_NUM
#endif // MAX_ERRORPRINT_NUM
}

#define LOG_ERROR_PRINT LogErrorPrint

#endif //__USUALLYUTILITY_H_
