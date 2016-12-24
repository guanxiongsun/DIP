/////////////////////////////////////////////////////////////////////////////
// CDlgArith dialog
#include "stdafx.h"
#include "ImageProcessing.h"
#include "DlgCoding.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define fPro4Zero  0.25;
#define fPro4One   0.75;
CDlgArith::CDlgArith(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgArith::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgArith)
	m_ArithSerial = _T("");
	m_ArithOutput = _T("");
	m_ArithDecode = _T("");
	//}}AFX_DATA_INIT
}


void CDlgArith::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgArith)
	DDX_Control(pDX, IDC_EDIT1, m_ConArithSer);
	DDX_Control(pDX, IDCODING, m_coding);
	DDX_Control(pDX, IDDECODING, m_decoding);
	DDX_Text(pDX, IDC_EDIT1, m_ArithSerial);
	DDV_MaxChars(pDX, m_ArithSerial, 15);
	DDX_Text(pDX, IDC_EDIT2, m_ArithOutput);
	DDX_Text(pDX, IDC_EDIT4, m_ArithDecode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgArith, CDialog)
	//{{AFX_MSG_MAP(CDlgArith)
	ON_BN_CLICKED(IDDECODING, OnDecoding)
	ON_BN_CLICKED(IDCODING, OnCoding)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgArith message handlers

//void CDlgArith::OnOK() 
//{
//	CDialog::OnOK();
//}
///////////////////////////////////////////////////////////////
// DlgCodingIArith dialog



/***********************************************************
 ʵ�ֶ��Ѿ���������ֽ��н���Ĺ���
 *********************************************************
*/

void CDlgArith::OnDecoding() 
{
	// ��ֵ���еĳ���
	int nOutLength;

	// ��������ĳ���
	int nInLength;

	// ������������޺�����  
	double dHigh=1.0;
	double dLow=0.0;

	// ��������ĳ���
	double dRange=1.0;

	// �ж϶�ֵ�����Ƿ�ȫ��
	int nNo1=0;
	
	// ѭ������
	int i;
	
	// �����Ʊ�ʾΪʮ����
	double dTenCode=0;
	
	// �м����
	double d2Pow;
	double dTemp;

	// ��������
	UpdateData(TRUE);

	// ������ʾ���
	m_ArithDecode = _T("");
	
	// ��ʾ����
	UpdateData(FALSE);
	
	// ��������ĳ���
	nInLength =m_ArithOutput.GetLength();
	
	// ������������ת��Ϊʮ���ƣ����ж��Ƿ�Ϊ��
	for (i=0; i<nInLength;i++ )
	{
		// �����Ƶ�ÿλ��Ӧʮ���Ƶ�ֵ
		d2Pow = pow(0.5,i+1);
		if(m_ArithOutput.Mid(i,1)=='1')
		{
			dTenCode=dTenCode+d2Pow;
			nNo1++;
		}
	}

	// ��ֵ���еĳ���
	nOutLength = m_ArithSerial.GetLength();

	// ��ȫ��ͷ�ȫ�������������
	if(nNo1!=0)
	{
		for(i=0;i<nOutLength;i++)
		{
			dTemp=dLow+dRange*fPro4Zero;

			if(dTemp>dTenCode)
			{
				// ���0
				m_ArithDecode=m_ArithDecode+'0';
				
				// �������������޵ļ���
				dLow=dLow;
				dHigh=dLow+dRange*fPro4Zero;
				
				// ���䷶Χ
				dRange=dHigh-dLow;
			}
			else
			{
				// ���1
				m_ArithDecode=m_ArithDecode+'1';
				
				// �������������޵ļ���
				dLow=dLow+dRange*fPro4Zero;
				dHigh=dHigh;

				// ���䷶Χ
				dRange=dHigh-dLow;
			}
		}
	}
	else
	{
		for(i=0;i<nOutLength;i++)
			m_ArithDecode=m_ArithDecode+'1';
	}
	

	// �����������
	UpdateData(FALSE);

	// �������������ǰ����������
	m_decoding.EnableWindow(FALSE);

	// ��������
	m_ConArithSer.EnableWindow(TRUE);
	
	// �������
	m_coding.EnableWindow(TRUE);
	
}

/***********************************************************
 ʵ�ֶ�������������н��б���Ĺ���
 *********************************************************
*/

void CDlgArith::OnCoding() 
{
	
	// �����ֵ���г���
	int nInLength;

	// ������ֳ���
	int nOutLength;

	// ѭ������
	int i;

	// �м����
	double dTemp;

	// ������������޺�����  
	double dHigh=1.0;
	double dLow=0.0;

	// ��������ĳ���
	double dRange=1.0;

	// �ۻ�����
	double dAccuPro=1.0;
	
    // �������������
	UpdateData(TRUE);
	
	// �������ı�������
	m_ArithOutput = _T("");
	
	// ���
	UpdateData(FALSE);
	
	// ����밴ť��Ч
	m_decoding.EnableWindow(FALSE);
	
	// ����Ķ�ֵ���г���
	nInLength = m_ArithSerial.GetLength();
 
	//  �Զ�ֵ���н��б���
	for (i = 0; i < nInLength; i++ )
	{
		
		// �������Ϊ1
		if(m_ArithSerial.Mid(i,1) == "1")
		{
			// �������������޵ļ���
			dHigh = dHigh;
			dLow  = dLow + dRange*fPro4Zero;

			// �������䳤��
			dRange = dHigh - dLow;

			// ��ֵ���г��ָ��ʵļ���
			dAccuPro = dAccuPro * fPro4One;			
		}
		// �������Ϊ0
		else if(m_ArithSerial.Mid(i,1) == "0")
		{
			// �������������޵ļ���
			dHigh = dLow + dRange*fPro4Zero;
			dLow  = dLow;

			// �������䳤��
			dRange = dHigh - dLow;

			// ��ֵ���г��ָ��ʵļ���
			dAccuPro = dAccuPro * fPro4Zero;			
		}
		// �������Ƕ�ֵ����
		else
		{
			// ��������
			MessageBox("�������ֵ����!", "ϵͳ��ʾ" , 
			MB_ICONINFORMATION | MB_OK);
			return;
		}
	}

	// ����������ֵĳ���
	dTemp = floor( -log10(dAccuPro) / log10(2) ) + 1;
	nOutLength = (int)dTemp;

	dTemp = dLow;
	double d2Pow;
	
	// ��ʮ���Ƶ�С��ת���ɶ����Ƶ�С����ʾ
	for (i = 0; i < nOutLength; i++ )
	{
		// ������С����i��Ӧ��ʮ����ֵ
		d2Pow = pow(0.5,i + 1);
		
		// �жϵ�ǰλ��ֵ0����1
		if(dTemp >= d2Pow)
		{
			m_ArithOutput = m_ArithOutput + "1";
			dTemp = dTemp - d2Pow;
		}
		else 
			m_ArithOutput = m_ArithOutput + "0";
	}
	
	// ת�����Ƿ�������
	if(dTemp > 0)
	{
		// ������С�����н�λ
		for(i = nOutLength-1; i >= 0; i--)
		{
			// ��λ��1ת��Ϊ0
			if(m_ArithOutput.Mid(i,1) == '1')
			{
				m_ArithOutput.Delete(i,1);
				m_ArithOutput.Insert(i,"0");
			}
			// ��λ��ɣ�����0λת��Ϊ1
			else
			{
				m_ArithOutput.Delete(i,1);
				m_ArithOutput.Insert(i,"1");
				break;
			}
		}
	}
     
	// ������ɣ����ݸ���
	UpdateData(FALSE);

	// ������н���
	m_decoding.EnableWindow(TRUE);

	// ����ǰ��ֹ����
	m_coding.EnableWindow(FALSE);

	// ����ǰ��ֹ�����µĶ���������
	m_ConArithSer.EnableWindow(FALSE);

}
