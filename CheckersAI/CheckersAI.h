
// CheckersAI.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

//#include "Precomp.h"
#include "resource.h"		// main symbols
#include "GridCtrl\GridCtrl.h"

// CCheckersAIApp:
// See CheckersAI.cpp for the implementation of this class
//

class CCheckersAIApp : public CWinApp
{
public:
	CCheckersAIApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CCheckersAIApp theApp;
