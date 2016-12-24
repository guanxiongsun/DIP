// DlgHistShow1.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessing.h"
#include "DlgHistShow1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgHistShow dialog


CDlgHistShow::CDlgHistShow(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHistShow::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgHistShow)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgHistShow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgHistShow)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgHistShow, CDialog)
	//{{AFX_MSG_MAP(CDlgHistShow)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgHistShow message handlers

BOOL CDlgHistShow::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	// ����ֱ��ԭͼ�����ص�ָ��
	unsigned char * lpSrc;

	// ѭ������
	int i,j;

	// ��ȡ����ֱ��ͼ�ı�ǩ
	CWnd* pWnd = GetDlgItem(IDC_DLG_HIST_SHOW);

	// ����õ�ֱ��ͼ
	// ͼ��ĸ߶ȺͿ��
	CSize sizeImage;
	sizeImage = m_pDib->GetDimensions();


	// ���ͼ�����ݴ洢�ĸ߶ȺͿ��
	CSize sizeSaveImage;
	sizeSaveImage = m_pDib->GetDibSaveDim();

	// ���ü���Ϊ0
	for (i = 0; i < 256; i ++)
	{
		// ����
		m_nHist[i] = 0;
	}
	
	// ��������Ҷ�ֵ�ļ��������õ�ֱ��ͼ
	for (i = 0; i < sizeImage.cy; i ++)
	{
		for (j = 0; j < sizeImage.cx; j ++)
		{
			lpSrc = (unsigned char *)m_pDib->m_lpImage + sizeSaveImage.cx * i + j;
			
			// ������1
			m_nHist[*(lpSrc)]++;
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgHistShow::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here


	// ѭ������
	int i;
	// ��ȡ����ֱ��ͼ�ı���ı�ǩ
	CWnd* pWnd = GetDlgItem(IDC_DLG_HIST_SHOW);

	// ��ȡ�豸������
	CDC* pDC = pWnd->GetDC();
	pWnd->Invalidate();
	pWnd->UpdateWindow();

	pDC->Rectangle(0, 0, 330, 300);

	// �������ʶ���
	CPen* pPenRed = new CPen;

	// ������ɫ���ʣ����ڻ��������ᣩ
	pPenRed->CreatePen(PS_SOLID, 1, RGB(255,0,0));

	// ѡ���ɫ���ʣ���������ǰ�Ļ���
	CPen* pOldPen = pDC->SelectObject(pPenRed);

	// ����������
	pDC->MoveTo(10,10);
	
	// ���ƴ�ֱ��
	pDC->LineTo(10, 280);

	// ����ˮƽ��
	pDC->LineTo(320, 280);

	// ����X��̶�ֵ
	CString strTemp;
	strTemp.Format("0");
	pDC->TextOut(10, 283, strTemp);
	strTemp.Format("50");
	pDC->TextOut(60, 283, strTemp);
	strTemp.Format("100");
	pDC->TextOut(110, 283, strTemp);
	strTemp.Format("150");
	pDC->TextOut(160, 283, strTemp);
	strTemp.Format("200");
	pDC->TextOut(210, 283, strTemp);
	strTemp.Format("255");
	pDC->TextOut(265, 283, strTemp);
	
	// ����X��̶�
	for (i = 0; i < 256; i += 5)
	{
		if ((i & 1) == 0)
		{
			// 10�ı���
			pDC->MoveTo(i + 10, 280);
			pDC->LineTo(i + 10, 284);
		}
		else
		{
			// 5����������
			pDC->MoveTo(i + 10, 280);
			pDC->LineTo(i + 10, 282);
		}
	}
	
	// ����X���ͷ
	pDC->MoveTo(315,275);
	pDC->LineTo(320,280);
	pDC->LineTo(315,285);
	
	// ����Y���ͷ
	pDC->MoveTo(10,10);
	pDC->LineTo(5,15);
	pDC->MoveTo(10,10);
	pDC->LineTo(15,15);

	// ֱ��ͼ��������ֵ
	LONG lMaxCount = 0;

	// ����������ֵ
	for (i = 0; i <= 255; i ++)
	{
		// �ж��Ƿ���ڵ�ǰ���ֵ
		if (m_nHist[i] > lMaxCount)
		{
			// �������ֵ
			lMaxCount = m_nHist[i];
		}
	}
	
	// ���������ֵ
	pDC->MoveTo(10, 25);
	pDC->LineTo(14, 25);
	strTemp.Format("%d", lMaxCount);
	pDC->TextOut(11, 26, strTemp);
	
	// �������ʶ���
	CPen* pPenBlue = new CPen;

	// ������ɫ���ʣ����ڻ���ֱ��ͼ��
	pPenBlue->CreatePen(PS_SOLID, 1, RGB(0,0,255));

	// ѡ����ɫ����
	pDC->SelectObject(pPenBlue);

	// �ж��Ƿ���ڼ���ֵ
	if(lMaxCount > 0){
		// ����ֱ��ͼ
		for (i = 0; i <= 255; i ++)
		{
			pDC->MoveTo(i + 10, 280);
			pDC->LineTo(i + 10, 281 - (int) (m_nHist[i] * 256 / lMaxCount));
		}
	}

	// �ָ���ǰ�Ļ���
	pDC->SelectObject(pOldPen);	
	
	// ɾ���µĻ���
	delete pPenRed;
	delete pPenBlue;
}
