#if !defined(AFX_DLGMEDIAN_H__63E4C51C_3AFA_4A57_85C2_CB444BEEAE62__INCLUDED_)
#define AFX_DLGMEDIAN_H__63E4C51C_3AFA_4A57_85C2_CB444BEEAE62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMedian.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMedian dialog

class CDlgMedian : public CDialog
{
// Construction
public:
	

	CDlgMedian(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgMedian)
	enum { IDD = IDD_DLG_ENHANCE_MEDIAN };

	// �˲�������λ��X����
	int		m_nFilterCenX;

	// �˲�������λ��Y����
	int		m_nFilterCenY;

	// �˲����߶�
	int		m_nFilterHeight;

	// �˲������
	int		m_nFilterWidth;

	// �˲�������
	int		m_nFilterType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMedian)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMedian)
	afx_msg void OnRadioMedian2dimen();
	afx_msg void OnRadioMedianHorizon();
	afx_msg void OnRadioMedianVertical();
	afx_msg void OnRadioMedianSelfdef();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMEDIAN_H__63E4C51C_3AFA_4A57_85C2_CB444BEEAE62__INCLUDED_)
