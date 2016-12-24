// WaterMark.h : main header file for the WATERMARK application
//

#if !defined(AFX_WATERMARK_H__B3E5607B_D8E7_42BF_A8E5_5596BF46C8CE__INCLUDED_)
#define AFX_WATERMARK_H__B3E5607B_D8E7_42BF_A8E5_5596BF46C8CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CWaterMarkApp:
// See WaterMark.cpp for the implementation of this class
//

class CWaterMarkApp : public CWinApp
{
public:
	CWaterMarkApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWaterMarkApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CWaterMarkApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WATERMARK_H__B3E5607B_D8E7_42BF_A8E5_5596BF46C8CE__INCLUDED_)
