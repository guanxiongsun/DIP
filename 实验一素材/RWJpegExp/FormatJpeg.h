#if !defined(AFX_FORMATJPEG_H__604FD2C8_AD30_44D5_A84F_72CB06FA6CF6__INCLUDED_)
#define AFX_FORMATJPEG_H__604FD2C8_AD30_44D5_A84F_72CB06FA6CF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FormatJpeg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormatJpeg dialog

class CFormatJpeg : public CDialog
{
// Construction
public:
	CFormatJpeg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFormatJpeg)
	enum { IDD = IDD_DLG_JPEG };
	CSliderCtrl	m_Quality;
	int		m_nQuality;
	CString	m_strQuality;
	//}}AFX_DATA
    BOOL m_bColor;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormatJpeg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFormatJpeg)
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMATJPEG_H__604FD2C8_AD30_44D5_A84F_72CB06FA6CF6__INCLUDED_)
