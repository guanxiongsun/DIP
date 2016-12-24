#if !defined(AFX_DLGSMOOTH_H__C3C270A3_299B_4E53_ADC8_FEE54D770B97__INCLUDED_)
#define AFX_DLGSMOOTH_H__C3C270A3_299B_4E53_ADC8_FEE54D770B97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSmooth.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSmooth dialog

class CDlgSmooth : public CDialog
{
// Construction
public:

	void UpdateEdit();

	// ģ������
	double *m_pdbTemp;

	// ����ģ������
	int		m_nTemType;

	CDlgSmooth(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSmooth)
	enum { IDD = IDD_DLG_ENHANCE_SMOOTH };
	
	// ģ������X����
	int		m_nSmthTemCenX;

	// ģ������Y����
	int		m_nSmthTemCenY;

	// ģ��ϵ��
	double		m_dbSmthTemCoef;

	// ģ��߶�
	int		m_nSmthTemHeight;

	// ģ����
	int		m_nSmthTemWidth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSmooth)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSmooth)
	afx_msg void OnRadioAvertem();
	afx_msg void OnRadioGuasstem();
	afx_msg void OnRadioSelftem();
	afx_msg void OnKillfocusEditSelftemHeight();
	afx_msg void OnKillfocusEditSelftemWidth();
	virtual void OnOK();
	afx_msg void OnChangeEditSelftemWidth();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSMOOTH_H__C3C270A3_299B_4E53_ADC8_FEE54D770B97__INCLUDED_)
