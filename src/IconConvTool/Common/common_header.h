
// Convert.cpp : Defines the entry point for the console application.
//

#ifndef __COMMON_HEADER_H
#define __COMMON_HEADER_H

#pragma once

#include <stdio.h>
#include <tchar.h>
#include <time.h>
#include <string>
#if !defined(_UNICODE) && !defined(UNICODE)
#define tstring std::string
#define __MY__TEXT(quote) quote
#else
#define tstring std::wstring
#define __MY__TEXT(quote) L##quote
#endif

#define _MY_TEXT(x)	__MY__TEXT(x)
#define	_MY_T(x)    __MY__TEXT(x)

#include <vector>
typedef std::vector<tstring> TSTRINGVECTOR;
typedef std::vector<TSTRINGVECTOR> TSTRINGVECTORVECTOR;
typedef struct tagConfigureDatablock{
    _TCHAR tExchangeCode[2];//-空 1-郑州商品交易所 2-大连商品交易所 3-中国金融期货交易所 4-上海期货交易所 5-其它
    _TCHAR tContractVarietyCode[5];//a
    _TCHAR tStartDate[9];//19880101
    _TCHAR tFinalDate[9];//19880101

}CONFIGUREDATABLOCK, *PCONFIGUREDATABLOCK;

typedef enum{
 TFTYPE_0=0, //YYYYMMDDHHMMSS
 TFTYPE_1=1, //YYYY-MM-DD HH:MM:SS
 TFTYPE_2=2, //YYYYMMDD
 TFTYPE_3=3, //YYYY-MM-DD
 TFTYPE_4=4, //HHMMSS
 TFTYPE_5=5, //HH:MM:SS
 TFTYPE_6=6, //MM-DD HH:MM
} TIME_FORMAT_TYPE;

/*
 * We do not implement alternate representations. However, we always
 * check whether a given modifier is allowed for a certain conversion.
 */
#define ALT_E          0x01
#define ALT_O          0x02
#define LEGAL_ALT(x)   { if (alt_format & ~(x)) return (0); }
#define TM_YEAR_BASE   (1970)

static int conv_num(const _TCHAR **, int *, int, int);
//static int strncasecmp(char *s1, char *s2, size_t n);

static const _TCHAR *day[] = {
  _MY_T("Sunday"), _MY_T("Monday"), _MY_T("Tuesday"),
  _MY_T("Wednesday"), _MY_T("Thursday"), _MY_T("Friday"),
  _MY_T("Saturday")
};
static const _TCHAR *abday[] = {
  _MY_T("Sun"),_MY_T("Mon"),_MY_T("Tue"),
  _MY_T("Wed"),_MY_T("Thu"),_MY_T("Fri"),
  _MY_T("Sat")
};
static const _TCHAR *mon[] = {
  _MY_T("January"), _MY_T("February"), _MY_T("March"),
  _MY_T("April"), _MY_T("May"), _MY_T("June"),
  _MY_T("July"), _MY_T("August"), _MY_T("September"),
  _MY_T("October"), _MY_T("November"), _MY_T("December")
};
static const _TCHAR *abmon[12] = {
  _MY_T("Jan"), _MY_T("Feb"), _MY_T("Mar"),
  _MY_T("Apr"), _MY_T("May"), _MY_T("Jun"),
  _MY_T("Jul"), _MY_T("Aug"), _MY_T("Sep"),
  _MY_T("Oct"), _MY_T("Nov"), _MY_T("Dec")
};
static const _TCHAR *am_pm[] = {
  _MY_T("AM"), _MY_T("PM")
};

static int conv_num(const _TCHAR **buf, int *dest, int llim, int ulim)
{
 int result = 0;

 /* The limit also determines the number of valid digits. */
 int rulim = ulim;

 if (**buf < _MY_T('0') || **buf > _MY_T('9'))
  return (0);

 do {
  result *= 10;
  result += *(*buf)++ - _MY_T('0');
  rulim /= 10;
 } while ((result * 10 <= ulim) && rulim && **buf >= _MY_T('0') && **buf <= _MY_T('9'));

 if (result < llim || result > ulim)
  return (0);

 *dest = result;
 return (1);
}

extern __inline _TCHAR * _tcsptime(const _TCHAR *buf, const _TCHAR *fmt, struct tm *tm)
{
     _TCHAR c;
     const _TCHAR *bp;
     size_t len = 0;
     int alt_format = 0;
  int i = 0;
  int split_year = 0;

     bp = buf;

     while ((c = *fmt) != _MY_T('/0')) {
         /* Clear 'alternate' modifier prior to new conversion. */
         alt_format = 0;

         /* Eat up white-space. */
         if (_istspace(c)) {
    while (_istspace(*bp))
     bp++;

              fmt++;
              continue;
         }

         if ((c = *fmt++) != _MY_T('%'))
              goto literal;


again:        switch (c = *fmt++) {
         case _MY_T('%'): /* "%%" is converted to "%". */
literal:
              if (c != *bp++)
                   return (0);
              break;

         /*
          * "Alternative" modifiers. Just set the appropriate flag
          * and start over again.
          */
         case _MY_T('E'): /* "%E" alternative conversion modifier. */
              LEGAL_ALT(0);
              alt_format |= ALT_E;
              goto again;

         case _MY_T('O'): /* "%O" alternative conversion modifier. */
              LEGAL_ALT(0);
              alt_format |= ALT_O;
              goto again;

         /*
          * "Complex" conversion rules, implemented through recursion.
          */
         case _MY_T('c'): /* Date and time, using the locale's format. */
              LEGAL_ALT(ALT_E);
              if (!(bp = _tcsptime(bp, _MY_T("%x %X"), tm)))
                   return (0);
              break;

         case _MY_T('D'): /* The date as "%m/%d/%y". */
              LEGAL_ALT(0);
              if (!(bp = _tcsptime(bp, _MY_T("%m/%d/%y"), tm)))
                   return (0);
              break;

         case _MY_T('R'): /* The time as "%H:%M". */
              LEGAL_ALT(0);
              if (!(bp = _tcsptime(bp, _MY_T("%H:%M"), tm)))
                   return (0);
              break;

         case _MY_T('r'): /* The time in 12-hour clock representation. */
              LEGAL_ALT(0);
              if (!(bp = _tcsptime(bp, _MY_T("%I:%M:%S %p"), tm)))
                   return (0);
              break;

         case _MY_T('T'): /* The time as "%H:%M:%S". */
              LEGAL_ALT(0);
              if (!(bp = _tcsptime(bp, _MY_T("%H:%M:%S"), tm)))
                   return (0);
              break;

         case _MY_T('X'): /* The time, using the locale's format. */
              LEGAL_ALT(ALT_E);
              if (!(bp = _tcsptime(bp, _MY_T("%H:%M:%S"), tm)))
                   return (0);
              break;

         case _MY_T('x'): /* The date, using the locale's format. */
              LEGAL_ALT(ALT_E);
              if (!(bp = _tcsptime(bp, _MY_T("%m/%d/%y"), tm)))
                   return (0);
              break;

         /*
          * "Elementary" conversion rules.
          */
         case _MY_T('A'): /* The day of week, using the locale's form. */
         case _MY_T('a'):
              LEGAL_ALT(0);
              for (i = 0; i < 7; i++) {
                   /* Full name. */
                   len = _tcslen(day[i]);
                   if (_tcsnicmp((_TCHAR *)(day[i]), (_TCHAR *)bp, len) == 0)
                       break;

                   /* Abbreviated name. */
                   len = _tcslen(abday[i]);
                   if (_tcsnicmp((_TCHAR *)(abday[i]), (_TCHAR *)bp, len) == 0)
                       break;
              }

              /* Nothing matched. */
              if (i == 7)
                   return (0);

              tm->tm_wday = i;
              bp += len;
              break;

         case _MY_T('B'): /* The month, using the locale's form. */
         case _MY_T('b'):
         case _MY_T('h'):
              LEGAL_ALT(0);
              for (i = 0; i < 12; i++) {
                   /* Full name. */
                   len = _tcslen(mon[i]);
                   if (_tcsnicmp((_TCHAR *)(mon[i]), (_TCHAR *)bp, len) == 0)
                       break;

                   /* Abbreviated name. */
                   len = _tcslen(abmon[i]);
                   if (_tcsnicmp((_TCHAR *)(abmon[i]),(_TCHAR *) bp, len) == 0)
                       break;
              }

              /* Nothing matched. */
              if (i == 12)
                   return (0);

              tm->tm_mon = i;
              bp += len;
              break;

         case _MY_T('C'): /* The century number. */
              LEGAL_ALT(ALT_E);
              if (!(conv_num(&bp, &i, 0, 99)))
                   return (0);

              if (split_year) {
                   tm->tm_year = (tm->tm_year % 100) + (i * 100);
              } else {
                   tm->tm_year = i * 100;
                   split_year = 1;
              }
              break;

         case _MY_T('d'): /* The day of month. */
         case _MY_T('e'):
              LEGAL_ALT(ALT_O);
              if (!(conv_num(&bp, &tm->tm_mday, 1, 31)))
                   return (0);
              break;

         case _MY_T('k'): /* The hour (24-hour clock representation). */
              LEGAL_ALT(0);
              /* FALLTHROUGH */
         case _MY_T('H'):
              LEGAL_ALT(ALT_O);
              if (!(conv_num(&bp, &tm->tm_hour, 0, 23)))
                   return (0);
              break;

         case _MY_T('l'): /* The hour (12-hour clock representation). */
              LEGAL_ALT(0);
              /* FALLTHROUGH */
         case _MY_T('I'):
              LEGAL_ALT(ALT_O);
              if (!(conv_num(&bp, &tm->tm_hour, 1, 12)))
                   return (0);
              if (tm->tm_hour == 12)
                   tm->tm_hour = 0;
              break;

         case _MY_T('j'): /* The day of year. */
              LEGAL_ALT(0);
              if (!(conv_num(&bp, &i, 1, 366)))
                   return (0);
              tm->tm_yday = i - 1;
              break;

          case _MY_T('M'): /* The minute. */
              LEGAL_ALT(ALT_O);
              if (!(conv_num(&bp, &tm->tm_min, 0, 59)))
                   return (0);
              break;

         case _MY_T('m'): /* The month. */
              LEGAL_ALT(ALT_O);
              if (!(conv_num(&bp, &i, 1, 12)))
                   return (0);
              tm->tm_mon = i - 1;
              break;

    case _MY_T('p'): /* The locale's equivalent of AM/PM. */
      LEGAL_ALT(0);
      /* AM */
      if (_tcscmp(am_pm[0], bp) == 0) {
     if (tm->tm_hour > 11)
      return (0);

     bp += _tcslen(am_pm[0]);
     break;
      }
      /* PM */
      else if (_tcscmp(am_pm[1], bp) == 0) {
     if (tm->tm_hour > 11)
      return (0);

     tm->tm_hour += 12;
     bp += _tcslen(am_pm[1]);
     break;
      }

      /* Nothing matched. */
      return (0);

         case _MY_T('S'): /* The seconds. */
              LEGAL_ALT(ALT_O);
              if (!(conv_num(&bp, &tm->tm_sec, 0, 61)))
                   return (0);
              break;

         case _MY_T('U'): /* The week of year, beginning on sunday. */
         case _MY_T('W'): /* The week of year, beginning on monday. */
              LEGAL_ALT(ALT_O);
              /*
               * XXX This is bogus, as we can not assume any valid
               * information present in the tm structure at this
               * point to calculate a real value, so just check the
               * range for now.
               */
               if (!(conv_num(&bp, &i, 0, 53)))
                   return (0);
               break;

         case _MY_T('w'): /* The day of week, beginning on sunday. */
              LEGAL_ALT(ALT_O);
              if (!(conv_num(&bp, &tm->tm_wday, 0, 6)))
                   return (0);
              break;

         case _MY_T('Y'): /* The year. */
    LEGAL_ALT(ALT_E);
    if (!(conv_num(&bp, &i, 0, 9999)))
     return (0);

    tm->tm_year = i - TM_YEAR_BASE;
    break;

         case _MY_T('y'): /* The year within 100 years of the epoch. */
              LEGAL_ALT(ALT_E | ALT_O);
              if (!(conv_num(&bp, &i, 0, 99)))
                   return (0);

              if (split_year) {
                   tm->tm_year = ((tm->tm_year / 100) * 100) + i;
                   break;
              }
              split_year = 1;
              if (i <= 68)
                   tm->tm_year = i + 2000 - TM_YEAR_BASE;
              else
                   tm->tm_year = i + 1900 - TM_YEAR_BASE;
              break;

         /*
          * Miscellaneous conversions.
          */
         case _MY_T('n'): /* Any kind of white-space. */
         case _MY_T('t'):
              LEGAL_ALT(0);
              while (_istspace(*bp))
                   bp++;
              break;


         default: /* Unknown/unsupported conversion. */
              return (0);
         }
     }

     /* LINTED functional specification */
     return ((_TCHAR *)bp);
}
#define TIME_ZONE_HOUR ((_timezone > 0 ? _timezone : -_timezone) / 3600)//时区相差小时数

extern __inline tstring T2S(time_t tt, TIME_FORMAT_TYPE tft)
{
 _TCHAR tTmp[128] = {0};
 struct tm *pTm = localtime(&tt);
 if (pTm)
 {
  switch(tft)
  {
  case TFTYPE_0:
   {
    _tcsftime(tTmp, sizeof(tTmp), _MY_T("%Y%m%d%H%M%S"), pTm);
   }
   break;
  case TFTYPE_1:
   {
    _tcsftime(tTmp, sizeof(tTmp), _MY_T("%Y-%m-%d %H:%M:%S"), pTm);
   }
   break;
  case TFTYPE_2:
   {
    _tcsftime(tTmp, sizeof(tTmp), _MY_T("%Y%m%d"), pTm);
   }
   break;
  case TFTYPE_3:
   {
    _tcsftime(tTmp, sizeof(tTmp), _MY_T("%Y-%m-%d"), pTm);
   }
   break;
  case TFTYPE_4:
   {
    _tcsftime(tTmp, sizeof(tTmp), _MY_T("%H%M%S"), pTm);
   }
   break;
  case TFTYPE_5:
   {
    _tcsftime(tTmp, sizeof(tTmp), _MY_T("%H:%M:%S"), pTm);
   }
   break;
  case TFTYPE_6:
   {
    _tcsftime(tTmp, sizeof(tTmp), _MY_T("%m-%d %H:%M"), pTm);
   }
   break;
  default:
   {

   }
   break;
  }
 }
 return tTmp;
}

extern __inline time_t S2T(tstring ts, TIME_FORMAT_TYPE tft)
{
 struct tm tm = {0};
 switch(tft)
 {
 case TFTYPE_0:
  {
   _tcsptime(ts.c_str(), _MY_T("%Y%m%d%H%M%S"), &tm);
  }
  break;
 case TFTYPE_1:
  {
   _tcsptime(ts.c_str(), _MY_T("%Y-%m-%d %H:%M:%S"), &tm);
  }
  break;
 case TFTYPE_2:
  {
   _tcsptime(ts.c_str(), _MY_T("%Y%m%d"), &tm);
  }
  break;
 case TFTYPE_3:
  {
   _tcsptime(ts.c_str(), _MY_T("%Y-%m-%d"), &tm);
  }
  break;
 case TFTYPE_4:
  {
   _tcsptime(ts.c_str(), _MY_T("%H%M%S"), &tm);
   tm.tm_mon += 0;
   tm.tm_mday += 1;
   tm.tm_hour += TIME_ZONE_HOUR;
  }
  break;
 case TFTYPE_5:
  {
   _tcsptime(ts.c_str(), _MY_T("%H:%M:%S"), &tm);
   tm.tm_mon += 0;
   tm.tm_mday += 1;
   tm.tm_hour += TIME_ZONE_HOUR;
  }
  break;
 case TFTYPE_6:
  {
   _tcsptime(ts.c_str(), _MY_T("%m-%d %H:%M"), &tm);
  }
  break;
 default:
  {

  }
  break;
 }
 tm.tm_year += TM_YEAR_BASE - 1900;
 return mktime(&tm);
}

//通用版将wstring转化为string
extern __inline std::string WtoA(std::wstring wstr, unsigned codepage = CP_ACP)
{
     int nwstrlen = WideCharToMultiByte(codepage, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
     if (nwstrlen > 0)
     {
          std::string str(nwstrlen + 1, '\0');
          WideCharToMultiByte(codepage, 0, wstr.c_str(), -1, (LPSTR)str.c_str(), nwstrlen, NULL, NULL);
          return str;
     }

     return ("");
}

//通用版将string转化为wstring
extern __inline std::wstring AtoW(std::string str, unsigned codepage = CP_ACP)
{
     int nstrlen = MultiByteToWideChar(codepage, 0, str.c_str(), -1, NULL, 0);
     if (nstrlen > 0)
     {
      std::wstring wstr(nstrlen + 1, '\0');
      MultiByteToWideChar(codepage, 0, str.c_str(), -1, (LPWSTR)wstr.c_str(), nstrlen);
      return wstr;
     }

     return (L"");
}

//将X编码转化为Y编码
extern __inline std::string CodePage_XtoY(std::string str, unsigned src_codepage, unsigned dst_codepage)
{
    return WtoA(AtoW(str, src_codepage), dst_codepage);
}

//将UTF8转化为GB2312
extern __inline std::string UTF8ToGB2132(std::string str)
{
    return CodePage_XtoY(str, CP_UTF8, CP_ACP);
}

//将GB2312转化为UTF8
extern __inline std::string GB2132ToUTF8(std::string str)
{
    return CodePage_XtoY(str, CP_ACP, CP_UTF8);
}

//ANSI将wstring转化为string
extern __inline std::string ANSI_WtoA(std::wstring wstr)
{
    return WtoA(wstr, CP_ACP);
}

//ANSI将string转化为wstring
extern __inline std::wstring ANSI_AtoW(std::string str)
{
    return AtoW(str, CP_ACP);
}

//获取指定两个字符串之间的字符串数据
extern __inline tstring string_reader(tstring tsData,
                                      tstring tsStart, tstring tsFinal,
                                      bool bTakeStart = false, bool bTakeFinal = false)
{
    tstring tsTmpData = _MY_T("");
    tstring::size_type stStartPos = tstring::npos;
    tstring::size_type stFinalPos = tstring::npos;
    stStartPos = tsData.find(tsStart);
    if(stStartPos != tstring::npos)
    {
        stFinalPos = tsData.find(tsFinal, stStartPos + tsStart.length());
        if(stFinalPos != tstring::npos)
        {
            if(!bTakeStart)
            {
                stStartPos += tsStart.length();
            }
            if(bTakeFinal)
            {
                stFinalPos += tsFinal.length();
            }
            tsTmpData = tsData.substr(stStartPos, stFinalPos - stStartPos);
        }
    }

    return tsTmpData;
}
extern __inline tstring string_replace_all(tstring &tsData, tstring tsDst, tstring tsSrc)
{
    tstring::size_type stPos = tstring::npos;
    while((stPos = tsData.find(tsSrc)) != tstring::npos)
    {
        tsData.replace(stPos, tsSrc.length(), tsDst);
    }

    return tsData;
}

extern __inline void string_split_to_vector(TSTRINGVECTOR & tsv, tstring tsData, tstring tsSplitter)
{
    tstring tsTmp = _T("");
    tstring::size_type stIdx = 0;
    tstring::size_type stPos = tstring::npos;
    tstring::size_type stSize = tsData.length();

    while((stPos = tsData.find(tsSplitter, stIdx)) != tstring::npos)
    {
        tsTmp = tsData.substr(stIdx, stPos - stIdx);
        if(!tsTmp.length())
        {
            tsTmp = _T("");
        }
        tsv.push_back(tsTmp);

        stIdx = stPos + tsSplitter.length();
    }

    if(stIdx < stSize)
    {
        tsTmp = tsData.substr(stIdx, stSize - stIdx);
        if(!tsTmp.length())
        {
            tsTmp = _T("");
        }
        tsv.push_back(tsTmp);
    }
}

extern __inline void LoadConfigure(CONFIGUREDATABLOCK &cdb, tstring tsFileName)
{
    FILE *pFile = _tfopen(tsFileName.c_str(), _MY_T("rb"));
    if(pFile)
    {
        _ftscanf(pFile, _MY_T("exchange_code=%[^\r\n]\r\n"), cdb.tExchangeCode);
        _ftscanf(pFile, _MY_T("contract_variety_code=%[^\r\n]\r\n"), cdb.tContractVarietyCode);
        _ftscanf(pFile, _MY_T("start_date=%[^\r\n]\r\n"), cdb.tStartDate);
        _ftscanf(pFile, _MY_T("final_date=%[^\r\n]\r\n"), cdb.tFinalDate);

        _tprintf(_MY_T("%s-%s-%s-%s\n"), cdb.tContractVarietyCode, cdb.tExchangeCode, cdb.tStartDate, cdb.tFinalDate);
        fclose(pFile);
        pFile = 0;
    }
}

#define GLOBAL_PUBLIC_CONFIG    "global_public.config"
typedef struct tagInterfaceConfigure{
    //新版和旧版
    _TCHAR tVersions[128];
    //最大支持32个地址
    _TCHAR tAddrList[32][32];
    //用户名称
    _TCHAR tUserName[128];
    //用户密码
    _TCHAR tPassWord[128];
    //符号文件名
    _TCHAR tTeamDBFileName[MAX_PATH];
    //数据文件名
    _TCHAR tDataDBFileName[MAX_PATH];
    //插件版本注册文件
    _TCHAR tIEWebRegistryFile[2][MAX_PATH];
    //插件版本
    _TCHAR tIEWebVersion[128];
    //核心处理延迟
    _TCHAR tCoreKernelDelay[128];
    //核心处理配置信息
    _TCHAR tCoreKernelConfig[MAX_PATH];
    //核心处理引擎
    _TCHAR tCoreKernelEngine[MAX_PATH];
    //HTTPBody文件
    _TCHAR tHttpBodyFileName[MAX_PATH];
    //HTTPHeader文件
    _TCHAR tHttpHeaderFileName[MAX_PATH];
    //HTTPEncodingUncompressMethodType文件
    _TCHAR tHttpEncodingUncompressMethodType[128];
    //HTTPCookie文件
    _TCHAR tHttpCookieFileName[MAX_PATH];
    //HTTPCookieJar文件
    _TCHAR tHttpCookieJarFileName[MAX_PATH];
    //cookie数据字符串文件名
    _TCHAR tHttpCookieStringFileName[MAX_PATH];
    //可用水位
    _TCHAR tValiableTotalWaterLevel[128];
    //可用比率
    _TCHAR tValiableReturnRates[128];
}INTERFACECONFIGURE, *PINTERFACECONFIGURE;


extern __inline void RunAppCmd(_TCHAR *pFile, _TCHAR *pParam,
               ULONG fMask = SEE_MASK_NO_CONSOLE,
               int nShowFlag = SW_SHOW, bool bWait = true, _TCHAR *pVerb = NULL,
               _TCHAR *pDirectory = NULL, HWND hWnd = NULL, HINSTANCE hInstance = NULL)
{
    SHELLEXECUTEINFO sei = {0};
    sei.cbSize = sizeof(SHELLEXECUTEINFO);
    sei.fMask = fMask;
    sei.hwnd = hWnd;
    sei.lpVerb = pVerb;
    sei.lpFile = pFile;
    sei.lpParameters = pParam;
    sei.lpDirectory = pDirectory;
    sei.nShow = nShowFlag;
    sei.hInstApp = hInstance;
    ::ShellExecuteEx(&sei);

    if(bWait)
    {
        ::WaitForSingleObject(sei.hProcess, INFINITE);
    }
}

extern __inline void AdjustPrivilege()
{
    HANDLE hToken;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
    {
        TOKEN_PRIVILEGES tp;
        tp.PrivilegeCount = 1;
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
        if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid))
        {
            AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
        }
        CloseHandle(hToken);
    }
}

//创建分解文件后，运行各分解文件时的进程
extern __inline void StartAppProg(const _TCHAR * pFilename, _TCHAR * pArgument = _T(""), bool bSynchronous = true)
{
     PROCESS_INFORMATION pi = {0};
     STARTUPINFO si = {0};

     memset(&si, 0, sizeof(si));
     si.cb = sizeof(si);
     CreateProcess(pFilename,   // No module name (use command line)
                pArgument,      // Command line
                NULL,           // Process handle not inheritable
                NULL,           // Thread handle not inheritable
                FALSE,          // Set handle inheritance to FALSE
                CREATE_NO_WINDOW | NORMAL_PRIORITY_CLASS, // No creation flags
                NULL,           // Use parent's environment block
                NULL,           // Use parent's starting directory
                &si,            // Pointer to STARTUPINFO structure
                &pi);           // Pointer to PROCESS_INFORMATION structure

	 //异步执行时，执行后不删除分解后的文件;同步执行时，执行后删除分解后的文件
     if(bSynchronous)  //同步执行
     {
        WaitForSingleObject(pi.hProcess, INFINITE);
     }

     //_tunlink(pFilename);
}

#include <io.h>
#include <fcntl.h>
#include <stdio.h>
extern __inline void InitConsoleWindow(void)
{
	tstring tsIniFilePath = _T(".\\DebugConfig.ini");

	int nDebugFlag = GetPrivateProfileInt(_T("DEBUG_FLAG"), _T("DEBUG"), 1, tsIniFilePath.c_str());

	if(!nDebugFlag)
	{
		return;//不弹出调试窗口
	}
	int hDesc = 0;
	FILE *pFile = 0;
	if(AllocConsole())
	{
		hDesc=_open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
		pFile=_fdopen(hDesc, "w");
		if (pFile)
		{
			*stdout = *pFile;
			setvbuf(stdout, NULL, _IONBF, 0);
			// test code
			_tprintf(_MY_T("Debuggee console started!\n"));
		}
	}
}

extern __inline void ExitConsoleWindow(void)
{
	FreeConsole();
}

#define EPSINON                         0.00000001f //定义精度系数
#define DOUBLE_EQUAL_ZERO(x)            ((x >= -EPSINON && x <= EPSINON) ? 0x01 : 0x00) //如果等于0，则返回0x01，否则返回0x00
#define DOUBLE_GREATER_ZERO(x)          ((x > EPSINON) ? 0x01 : 0x00) //如果大于0，则返回0x01，否则返回0x00
#define DOUBLE_LESS_ZERO(x)             ((x < -EPSINON) ? 0x01 : 0x00) //如果小于0，则返回0x01，否则返回0x00
#define DOUBLE_GREATER_EQUAL_ZERO(x)    ((x > EPSINON || DOUBLE_EQUAL_ZERO(x)) ? 0x01 : 0x00) //如果大于等于0，则返回0x01，否则返回0x00
#define DOUBLE_LESS_EQUAL_ZERO(x)       ((x < -EPSINON || DOUBLE_EQUAL_ZERO(x)) ? 0x01 : 0x00) //如果小于等于0，则返回0x01，否则返回0x00
#define DOUBLE_COMPARE_ZERO(x)          ((DOUBLE_GREATER_ZERO(x) ? 0x01 : (DOUBLE_LESS_ZERO(x) ? (-0x01) : 0x00)) //如果等于0，则返回0x00;如果大于0，则返回0x01;如果小于0，则返回-0x01

/// ListCtrl排序用的结构
typedef struct tagSortData
{
    int nSortFlag;//排序标识：0或1-升序-降序
    int nSortItem;//排序列
    unsigned long ptrListCtrl;//列表控件指针
} SORTDATA, * PSORTDATA;
#endif //#ifndef __COMMON_HEADER_H
