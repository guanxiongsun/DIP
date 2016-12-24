// RWJpegExp.h : main header file for the RWJPEGEXP application
//

#if !defined(AFX_RWJPEGEXP_H__B064F1FA_D1C0_42AB_8F0F_913957E6382A__INCLUDED_)
#define AFX_RWJPEGEXP_H__B064F1FA_D1C0_42AB_8F0F_913957E6382A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CRWJpegExpApp:
// See RWJpegExp.cpp for the implementation of this class
//

class CRWJpegExpApp : public CWinApp
{
public:
	CRWJpegExpApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRWJpegExpApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CRWJpegExpApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RWJPEGEXP_H__B064F1FA_D1C0_42AB_8F0F_913957E6382A__INCLUDED_)
