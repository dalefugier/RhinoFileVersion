/////////////////////////////////////////////////////////////////////////////
// RhinoFileVersionApp.h : main header file for the RhinoFileVersion DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CRhinoFileVersionApp
// See RhinoFileVersionApp.cpp for the implementation of this class
//

class CRhinoFileVersionApp : public CWinApp
{
public:
	CRhinoFileVersionApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
