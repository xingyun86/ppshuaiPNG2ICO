/////////////////////////////////////////
// InternetBrowserApp.h

#ifndef INTERNETBROWSERAPP_H
#define INTERNETBROWSERAPP_H

#include "Mainfrm.h"


// Declaration of the CWinApp class
class CInternetBrowserApp : public CWinApp
{
public:
    CInternetBrowserApp();
    virtual ~CInternetBrowserApp() {}
	virtual BOOL InitInstance();
	CMainFrame& GetMainFrame() { return m_Frame; }

private:
    CMainFrame m_Frame;
};


// returns a reference to the CBrowserApp object
inline CInternetBrowserApp& GetInternetBrowserApp() { return static_cast<CInternetBrowserApp&>(GetApp()); }


#endif // BROWSERAPP_H

