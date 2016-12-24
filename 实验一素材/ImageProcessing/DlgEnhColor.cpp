// DlgEnhColor.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessing.h"
#include "DlgEnhColor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgEnhColor dialog


CDlgEnhColor::CDlgEnhColor(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEnhColor::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgEnhColor)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgEnhColor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEnhColor)
	DDX_Control(pDX, IDC_LIST_ENH_COLOR, m_lstColor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEnhColor, CDialog)
	//{{AFX_MSG_MAP(CDlgEnhColor)
	ON_LBN_DBLCLK(IDC_LIST_ENH_COLOR, OnDblclkListEnhColor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEnhColor message handlers

BOOL CDlgEnhColor::OnInitDialog() 
{
	// ѭ������
	int		i;
	
	// ����Ĭ��OnInitDialog����
	CDialog::OnInitDialog();
	
	// ���α��ɫ����
	for (i = 0; i < m_nColorCount; i++)
	{
		m_lstColor.AddString(m_lpColorName + i * m_nNameLen);
	}

	// ѡ�г�ʼ�����
	m_lstColor.SetCurSel(m_nColor);
	
	// ����TRUE
	return TRUE;
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgEnhColor::OnDblclkListEnhColor() 
{
	// ˫���¼���ֱ�ӵ���OnOK()��Ա����
	OnOK();
}


void CDlgEnhColor::OnOK() 
{	
	// �û�����ȷ����ť
	m_nColor = m_lstColor.GetCurSel();
	
	// ����Ĭ�ϵ�OnOK()����
	CDialog::OnOK();
}
