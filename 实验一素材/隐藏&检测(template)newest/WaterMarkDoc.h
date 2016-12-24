// WaterMarkDoc.h : interface of the CWaterMarkDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WATERMARKDOC_H__80F538E7_50DD_4D5D_BD80_BD06CC10CC42__INCLUDED_)
#define AFX_WATERMARKDOC_H__80F538E7_50DD_4D5D_BD80_BD06CC10CC42__INCLUDED_


#include "StegBmp.h"	// Added by ClassView
#include "JpegSteg.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWaterMarkDoc : public CDocument
{
protected: // create from serialization only
	CWaterMarkDoc();
	DECLARE_DYNCREATE(CWaterMarkDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWaterMarkDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	enum FILE_TYPE{NONE,BMP,JPG};
	FILE_TYPE m_fileType[2];
	CBmp *m_pBmp[2];
	BOOL m_flag[2];
	CBmp m_sourceBmp;
	CStegBmp m_resultBmp;

	CJpegSteg m_jpg;
	CString m_srcJpg;
	CString m_destJpg;

	virtual ~CWaterMarkDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CWaterMarkDoc)
	afx_msg void OnAddMark();
	afx_msg void OnAbstractMark();
	afx_msg void OnFileOpenDest();
	afx_msg void OnDetect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WATERMARKDOC_H__80F538E7_50DD_4D5D_BD80_BD06CC10CC42__INCLUDED_)
