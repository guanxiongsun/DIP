// WaterMarkView.cpp : implementation of the CWaterMarkView class
//

#include "stdafx.h"
#include "WaterMark.h"

#include "WaterMarkDoc.h"
#include "WaterMarkView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWaterMarkView

IMPLEMENT_DYNCREATE(CWaterMarkView, CScrollView)

BEGIN_MESSAGE_MAP(CWaterMarkView, CScrollView)
	//{{AFX_MSG_MAP(CWaterMarkView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaterMarkView construction/destruction

CWaterMarkView::CWaterMarkView()
{
	// TODO: add construction code here
	m_index=-1;
}

CWaterMarkView::~CWaterMarkView()
{
}

BOOL CWaterMarkView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CWaterMarkView drawing

void CWaterMarkView::OnDraw(CDC* pDC)
{
	CWaterMarkDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(m_index<0)return;

	if(pDoc->m_flag[m_index])
	{
		pDoc->m_pBmp[m_index]->OnDraw(pDC);
		CSize size;
		size=pDoc->m_pBmp[m_index]->GetSize();
		SetScrollSizes(MM_TEXT,size);
	}

	// TODO: add draw code for native data here
}

void CWaterMarkView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

/////////////////////////////////////////////////////////////////////////////
// CWaterMarkView diagnostics

#ifdef _DEBUG
void CWaterMarkView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CWaterMarkView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CWaterMarkDoc* CWaterMarkView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWaterMarkDoc)));
	return (CWaterMarkDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWaterMarkView message handlers
