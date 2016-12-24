#if !defined(AFX_DLGEHNLINTRANS_H__C3556721_3387_11D0_9E74_000021CDD41E__INCLUDED_)
#define AFX_DLGEHNLINTRANS_H__C3556721_3387_11D0_9E74_000021CDD41E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEhnLinTrans.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgEhnLinTrans dialog

class CDlgEhnLinTrans : public CDialog
{
// Construction
public:
	// ��ǰ����϶�״̬��0��ʾδ�϶���1��ʾ�����϶���һ�㣬2��ʾ�����϶��ڶ��㡣
	int		m_nIsDraging;
	
	// ��Ӧ����¼��ľ�������
	CRect	m_rectMouse;
	
	// ��ʶ�Ƿ��Ѿ�������Ƥ����
	BOOL	m_bDrawed;
	
	// ��������������ʱ��λ��
	CPoint	m_pointMsClick;
	
	// ��������϶�ʱ��λ��
	CPoint	m_pointMsMove;

	CDlgEhnLinTrans(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgEhnLinTrans)
	enum { IDD = IDD_DLG_ENHANCE_LINTRANS };
	int		m_nX1;
	int		m_nX2;
	int		m_nY1;
	int		m_nY2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEhnLinTrans)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgEhnLinTrans)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnKillfocusEditLintransX1();
	afx_msg void OnKillfocusEditLintransX2();
	afx_msg void OnKillfocusEditLintransY1();
	afx_msg void OnKillfocusEditLintransY2();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEHNLINTRANS_H__C3556721_3387_11D0_9E74_000021CDD41E__INCLUDED_)
