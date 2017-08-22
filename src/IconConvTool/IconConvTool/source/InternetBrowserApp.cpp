////////////////////////////////////////
// BrowserApp.cpp

#include "stdafx.h"
#include "InternetBrowserApp.h"


// Definitions for the CInternetBrowserApp class
CInternetBrowserApp::CInternetBrowserApp()
{
    // Constructor for CBrowserApp
}

BOOL CInternetBrowserApp::InitInstance()
{
    // This function is called automatically when the application starts

    // Create the Frame
    m_Frame.Create();

	return TRUE;
}
