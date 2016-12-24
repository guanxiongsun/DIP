#if !defined(AFX_DLGRECMATCH_H__7947D7E1_3494_11D0_9E74_000021CDD41E__INCLUDED_)
#define AFX_DLGRECMATCH_H__7947D7E1_3494_11D0_9E74_000021CDD41E__INCLUDED_

//#include "GlobalApi.h"
#include "CDib.h"
#include "ImageProcessingDoc.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRecMatch.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgRecMatch dialog

class CDlgRecMatch : public CDialog
{
// Construction
public:
	CDib* m_pDibInit;		// ��ʼͼ��
	CDib* m_pDibModel;		// ģ��ͼ��
	CDib* m_pDibResult;		// ƥ����ͼ��
	CRect m_rectInitImage;		// ��ʼͼ����ʾ����
	CRect m_rectModelImage;		// ģ��ͼ����ʾ����
	CRect m_rectResltImage;		// ƥ���ͼ����ʾ����
	BOOL m_bCalImgLoc;		// ����ͼ��λ�õı�־λ��FALSE��ʾ��û�м���ͼ��λ��
	CImageProcessingDoc* m_pDoc;	// �ĵ���ָ��
	BOOL TemplateMatch(CDib* pDibSrc, CDib* pDibTemplate);	// ģ��ƥ��
	void CalImageLocation();	// ����ͼ��ȿؼ���λ�ô�С
	
	CDlgRecMatch(CWnd* pParent = NULL,CImageProcessingDoc* pDoc=NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgRecMatch)
	enum { IDD = IDD_DLG_RECOG_MATCH };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRecMatch)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgRecMatch)
	afx_msg void OnRecogOpenModel();
	afx_msg void OnRecogMatch();
	afx_msg void OnPaint();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGRECMATCH_H__7947D7E1_3494_11D0_9E74_000021CDD41E__INCLUDED_)
