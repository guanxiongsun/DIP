// RWJpegExpDoc.h : interface of the CRWJpegExpDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWJPEGEXPDOC_H__5625E0A2_02BC_4B09_80D5_8746103032AC__INCLUDED_)
#define AFX_RWJPEGEXPDOC_H__5625E0A2_02BC_4B09_80D5_8746103032AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include"Dib.h"
#include"Jpeg.h"

class CRWJpegExpDoc : public CDocument
{
protected: // create from serialization only
	CRWJpegExpDoc();
	DECLARE_DYNCREATE(CRWJpegExpDoc)

// Attributes
public:
  CDib * m_pDib;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRWJpegExpDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRWJpegExpDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRWJpegExpDoc)
	//afx_msg void OnFileOpen();
	afx_msg void OnFileSaveAs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RWJPEGEXPDOC_H__5625E0A2_02BC_4B09_80D5_8746103032AC__INCLUDED_)
