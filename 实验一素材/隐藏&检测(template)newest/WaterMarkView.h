// WaterMarkView.h : interface of the CWaterMarkView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WATERMARKVIEW_H__5065C0AD_C738_4EE4_B2D6_4ACA4535EC78__INCLUDED_)
#define AFX_WATERMARKVIEW_H__5065C0AD_C738_4EE4_B2D6_4ACA4535EC78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CWaterMarkView : public CScrollView
{
protected: // create from serialization only
	CWaterMarkView();
	DECLARE_DYNCREATE(CWaterMarkView)
// Attributes
public:
	CWaterMarkDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWaterMarkView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	int m_index;
	virtual ~CWaterMarkView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CWaterMarkView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in WaterMarkView.cpp
inline CWaterMarkDoc* CWaterMarkView::GetDocument()
   { return (CWaterMarkDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WATERMARKVIEW_H__5065C0AD_C738_4EE4_B2D6_4ACA4535EC78__INCLUDED_)
