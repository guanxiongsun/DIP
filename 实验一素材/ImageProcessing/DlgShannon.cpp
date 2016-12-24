// DlgShannon.cpp : implementation file
//

#include "stdafx.h"
#include "imageprocessing.h"
#include "DlgCoding.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgShannon dialog


CDlgShannon::CDlgShannon(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgShannon::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgShannon)
	m_dEntropy = 0.0;
	m_dAvgCodeLen = 0.0;
	m_dEfficiency = 0.0;
	//}}AFX_DATA_INIT
}


void CDlgShannon::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgShannon)
	DDX_Control(pDX, IDC_LST_Table, m_lstTable);
	DDX_Text(pDX, IDC_EDIT1, m_dEntropy);
	DDX_Text(pDX, IDC_EDIT2, m_dAvgCodeLen);
	DDX_Text(pDX, IDC_EDIT3, m_dEfficiency);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgShannon, CDialog)
	//{{AFX_MSG_MAP(CDlgShannon)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgShannon message handlers

BOOL CDlgShannon::OnInitDialog() 
{
	// ����Ĭ�ϵ�OnInitDialog()����
	CDialog::OnInitDialog();

	// ��ʼ������
	m_dEntropy = 0.0;
	m_dAvgCodeLen = 0.0;

	// �ַ�������
	CString	str;
	str = "1";
	
	// ѭ������
	LONG	i;
	LONG	j;

	// �ؼ�ListCtrl��ITEM
	LV_ITEM lvItem;

	// ����ؼ�ListCtrl����ӵ�ITEM���
	int		nItem2View;	
	
	// �м����
	double	dT;
	LONG	lTemp;
	
	// ��������м���������
	double * dTemp;
	dTemp = new double[m_nColorNum];
	
	// ����������ŻҶ�ֵ����λ��֮���ӳ��
	LONG *	l4Turn;
	l4Turn = new LONG[m_nColorNum];
	
	// ��ǰ��������ĸ��ʺ�
	double	dAccum;
	dAccum = 1.0;

	// �Ѿ�����Ҷ�ֵ���ʵ�ͳ�ƺ�
	double	dSum;
	dSum    = 0;
	
	// �ѱ���Ҷ�ֵ
	LONG	lCount = 0;
		
	// ��ʼλ��
	LONG	lBegin;
	
	// ָʾ�����Ƿ��Ѿ����һ��
	BOOL	* bFinished;
	bFinished = new BOOL[m_nColorNum];
	
	// �����ڴ�	
	m_strCode = new CString[m_nColorNum];
			
	for (i = 0; i < m_nColorNum; i ++)
	{			
		
		// ��ʼ��ΪFALSE
		bFinished[i] = FALSE;

		// �����ʸ�ֵdTemp����
		dTemp[i] = m_dProba[i];
		
		// ���Ҷ�ֵ��С˳������
		l4Turn[i] = i;
		
	}
	
	// ��ð�ݷ��Խ��лҶ�ֵ���ֵĸ�������
	// ͬʱ�ı�Ҷ�ֵλ�õ�ӳ���ϵ
	for (j = 0; j < m_nColorNum - 1; j ++)
		for (i = 0; i < m_nColorNum - j - 1; i ++)
			if (dTemp[i] > dTemp[i + 1])
			{
				// ����
				dT = dTemp[i];
				dTemp[i] = dTemp[i + 1];
				dTemp[i + 1] = dT;
				
				// ��i��i+1�Ҷȵ�λ��ֵ����
				lTemp = l4Turn[i];
				l4Turn[i] = l4Turn[i+1];
				l4Turn[i+1] = lTemp;
				
			}
		
	/*******************************************************
	������ũ����ŵ�����
	*******************************************************/
	
	// �Ӹ��ʴ���0����ʼ����
	for (lBegin = 0; lBegin < m_nColorNum - 1; lBegin ++)
		if (dTemp[lBegin] > 0)
			break;

	// ��ʼ����
	while(lCount < m_nColorNum)
	{
		// �Ӹ��ʴ�����ĻҶ�ֵ��ʼ����
		lCount = lBegin;
		
		// ��������зָ��ÿ������ĻҶ�ֵ����
		for (i = lBegin; i < m_nColorNum; i ++)
		{
			// �ж��Ƿ�������
			if (bFinished[i] == FALSE)
			{
				// ���ӵ�ǰ��������ĸ����ۺ�
				dSum += dTemp[i];
				
				// �ж��Ƿ񳬳��ܺ͵�һ��
				if (dSum > dAccum/2.0)
				{
					// ������׷�ӵ��ַ���Ϊ0
					str = "0";
				}
				
				// ׷���ַ�
				m_strCode[l4Turn[i]] = m_strCode[l4Turn[i]] + str;
				
				// �ж��Ƿ������һ��
				if (dSum == dAccum)
				{
					// ���һ���ֱ��룬���¼���dAccum
					
					// ��ʼ��dSumΪ0
					dSum = 0;
					
					// �ж��ǲ��Ƕ����лҶ�ֵ�Ѿ�����һ��
					if (i == m_nColorNum - 1)
						j = lBegin;
					else
						j = i + 1;
					
					// ����jֵ
					lTemp = j;
					str = m_strCode[l4Turn[j]];
					
					// ������һ��������ĸ����ܺ�
					dAccum = 0;
					for (; j < m_nColorNum; j++)
					{
						// �ж��Ƿ���ͬһ�α���
						if ((m_strCode[l4Turn[j]].Right(1) != str.Right(1)) 
							|| (m_strCode[l4Turn[j]].GetLength() != str.GetLength()))
							break;

						// ��ǰ����ĸ����ܺ�����
						dAccum += dTemp[j];
					}
					
					// ��������ֵΪ1
					str = "1";
					
					// �жϸöα����Ѿ����
					if (lTemp + 1 == j)
						bFinished[lTemp] = TRUE;
				}
			}
			else
			{
				//  ��ʼ��һ�ֱ���
				lCount ++;
				
				// ���¸�2dSumΪ0
				dSum = 0;
				
				// �ж��ǲ��Ƕ����лҶ�ֵ�Ѿ�����һ��
				if (i == m_nColorNum - 1)
					j = lBegin;
				else
					j = i + 1;
				
				// ����jֵ
				lTemp = j;
				str = m_strCode[l4Turn[j]];
				
				// ������һ��������ĸ����ܺ�
				dAccum = 0;
				for (; j < m_nColorNum; j++)
				{
					// �ж��Ƿ���ͬһ�α���
					if ((m_strCode[l4Turn[j]].Right(1) != str.Right(1)) 
						|| (m_strCode[l4Turn[j]].GetLength() != str.GetLength()))
					{
						// �˳�ѭ��
						break;
					}
					
					// �ۼ�
					dAccum += dTemp[j];
				}
				
				str = "1";
				
				// �жϸöα����Ѿ����
				if (lTemp + 1 == j)
					bFinished[lTemp] = TRUE;
			}
		}
	}
	
	// ����ͼ����
	for (i = 0; i < m_nColorNum; i ++)
	{
		// �жϸ����Ƿ����0
		if (m_dProba[i] > 0)
		{
			// ����ͼ����
			m_dEntropy -= m_dProba[i] * log(m_dProba[i]) / log(2.0);
		}
	}
	
	// ����ƽ�����ֳ���
	for (i = 0; i < m_nColorNum; i ++)
	{
		// �ۼ�
		m_dAvgCodeLen += m_dProba[i] * m_strCode[i].GetLength();
	}
	
	// �������Ч��
	m_dEfficiency = m_dEntropy / m_dAvgCodeLen;
	
	// ����䶯
	UpdateData(FALSE);
	
	/*************************************************
	���������
	*************************************************/	
	
	// ����List�ؼ���ʽ
	m_lstTable.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
	
	// ��List�ؼ����Header
	m_lstTable.InsertColumn(0, "�Ҷ�ֵ", LVCFMT_LEFT, 60, 0);
	m_lstTable.InsertColumn(1, "�Ҷ�ֵ����", LVCFMT_LEFT, 78, 0);
	m_lstTable.InsertColumn(2, "��ũ����ŵ����", LVCFMT_LEFT, 110, 1);
	m_lstTable.InsertColumn(3, "���ֳ���", LVCFMT_LEFT, 78, 2);
	
	// ������ʽΪ�ı�
	lvItem.mask = LVIF_TEXT;
	
	// �����ʾ
	for (i = 0; i < m_nColorNum; i ++)
	{
		// ��һ����ʾ
		lvItem.iItem = m_lstTable.GetItemCount();
		str.Format("%u",i);
		lvItem.iSubItem = 0;
		lvItem.pszText= (LPTSTR)(LPCTSTR)str;
		nItem2View = m_lstTable.InsertItem(&lvItem);
		
		// ��������ʾ
		lvItem.iItem = nItem2View;
		
		// ��ӻҶ�ֵ���ֵ�Ƶ��
		lvItem.iSubItem = 1;
		str.Format("%f",m_dProba[i]);
		lvItem.pszText = (LPTSTR)(LPCTSTR)str;
		m_lstTable.SetItem(&lvItem);
		
		// �����ũ����ŵ����
		lvItem.iSubItem = 2;
		lvItem.pszText = (LPTSTR)(LPCTSTR)m_strCode[i];
		m_lstTable.SetItem(&lvItem);
		
		// ������ֳ���
		lvItem.iSubItem = 3;
		str.Format("%u",m_strCode[i].GetLength());
		lvItem.pszText = (LPTSTR)(LPCTSTR)str;
		m_lstTable.SetItem(&lvItem);
	}
	
	//  �ڴ��ͷ�
	delete l4Turn;
	delete dTemp;
	delete bFinished;
	
	return TRUE;
}
