// FormatJpeg.cpp : implementation file
//

#include "stdafx.h"
#include "rwjpegexp.h"
#include "FormatJpeg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFormatJpeg dialog


CFormatJpeg::CFormatJpeg(CWnd* pParent /*=NULL*/)
	: CDialog(CFormatJpeg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormatJpeg)
	m_nQuality = 75;
	m_strQuality = _T("75");
	//}}AFX_DATA_INIT
	m_bColor=TRUE;
}


void CFormatJpeg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormatJpeg)
	DDX_Control(pDX, IDC_SLIDER1, m_Quality);
	DDX_Slider(pDX, IDC_SLIDER1, m_nQuality);
	DDX_Text(pDX, IDC_VALUE, m_strQuality);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormatJpeg, CDialog)
	//{{AFX_MSG_MAP(CFormatJpeg)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormatJpeg message handlers

BOOL CFormatJpeg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_Quality.SetRange(0, 100, TRUE);
	m_strQuality.Format("%d", m_nQuality);
	UpdateData(FALSE);

	((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(m_bColor);
	((CButton *)GetDlgItem(IDC_RADIO2))->SetCheck(! m_bColor);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFormatJpeg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
		UpdateData();
	m_strQuality.Format("%d", m_nQuality);
	UpdateData(FALSE);
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CFormatJpeg::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	m_bColor=TRUE;
	
}

void CFormatJpeg::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	m_bColor=FALSE;
	
}
