// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "WaterMark.h"

#include "WaterMarkDoc.h"
#include "MainFrm.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_WATERMARK_BAR, OnWatermarkBar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_bSplitterCreated=FALSE;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable


	if (!m_dlgbar.Create(this,IDD_DLGBAR,WS_CHILD | WS_VISIBLE|CBRS_BOTTOM,10000))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	m_dlgbar.EnableDocking(CBRS_ALIGN_ANY);

	EnableDocking(CBRS_ALIGN_ANY);
//	DockControlBar(&m_dlgbar);
	CPoint startPoint(400,300);
	FloatControlBar(&m_dlgbar,startPoint);
	m_dlgbar.EnableDocking(0);

	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	m_bSplitterCreated = m_wndSplitter.CreateStatic(this, 1,2);
    m_bSplitterCreated&= m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(CWaterMarkView),CSize(50,50),pContext);
    m_bSplitterCreated&= m_wndSplitter.CreateView(0,1,RUNTIME_CLASS(CWaterMarkView),CSize(50,50),pContext);
    if(m_bSplitterCreated)
	{
		m_pView[0]=(CWaterMarkView *)m_wndSplitter.GetPane(0,0);
		m_pView[1]=(CWaterMarkView *)m_wndSplitter.GetPane(0,1);
		m_pView[0]->m_index=0;
		m_pView[1]->m_index=1;
	}
    return this->m_bSplitterCreated;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	if(nType==SIZE_MINIMIZED) return;
	CRect rect;
	GetClientRect( &rect );
	int w=rect.Width();

	if( m_bSplitterCreated )  // m_bSplitterCreated set in OnCreateClient
	{
		m_wndSplitter.SetColumnInfo(0, w/2-3, 10);
		m_wndSplitter.SetColumnInfo(1, w/2-3, 10);
		m_wndSplitter.RecalcLayout();
	}
}

void CMainFrame::OnWatermarkBar() 
{
	BOOL visible;
	visible=m_dlgbar.IsWindowVisible();
	ShowControlBar(&m_dlgbar,!visible,FALSE);
}




