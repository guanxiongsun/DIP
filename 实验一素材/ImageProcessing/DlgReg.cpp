// DlgReg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessing.h"
#include "ImageProcessingDoc.h"
#include "GlobalApi.h"
#include "DlgReg.h"
#include "DlgAftReg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgReg dialog


CDlgReg::CDlgReg(CWnd* pParent /*=NULL*/, CImageProcessingDoc* pDoc)
	: CDialog(CDlgReg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgReg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// ��ȡ�ĵ���ָ��
	m_pDoc = pDoc;

	// ���ü���ͼ��λ�ñ�־λλFALSE
	m_bCalImgLoc = FALSE;

	// ���û�׼ͼ��Ϊԭʼ�򿪵�ͼ��
	m_pDibInit = pDoc->m_pDibInit;

	// ���ô���׼ͼ��
	m_pDibSamp = new CDib;

	// ����ѡȡ���������Ŀ��ʼֵ
	m_nChsFeatureNum = 0;

	// ����ѡȡ������ı�־λΪFALSE
	m_bChoseFeature = FALSE;
}


void CDlgReg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgReg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgReg, CDialog)
	//{{AFX_MSG_MAP(CDlgReg)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_REG_OPEN, OnRegOpen)
	ON_BN_CLICKED(IDC_REG_REG, OnRegReg)
	ON_BN_CLICKED(IDC_REG_CHOSE_FEATURE, OnRegChoseFeature)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgReg message handlers

void CDlgReg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// �����û�м���ͼ���λ�ã�����м���
	if(!m_bCalImgLoc){
		CalImageLocation();
	}

	CSize sizeDisplay;
	CPoint pointDisplay;
	
	// ��ʾ��׼ͼ��
	if(!m_pDibInit->IsEmpty()){
		sizeDisplay.cx=m_pDibInit->m_lpBMIH->biWidth;
		sizeDisplay.cy=m_pDibInit->m_lpBMIH->biHeight;
		pointDisplay.x = m_rectInitImage.left;
		pointDisplay.y = m_rectInitImage.top;
		m_pDibInit->Draw(&dc,pointDisplay,sizeDisplay);		
	}
	
	// ��ʾ����׼ͼ��
	if(!m_pDibSamp->IsEmpty()){
		sizeDisplay.cx=m_pDibSamp->m_lpBMIH->biWidth;
		sizeDisplay.cy=m_pDibSamp->m_lpBMIH->biHeight;
		pointDisplay.x = m_rectResltImage.left;
		pointDisplay.y = m_rectResltImage.top;
		m_pDibSamp->Draw(&dc,pointDisplay,sizeDisplay);		
	}

	// ��ʾ����������׼��������
	DrawFeature(&dc);	
}

/*************************************************************************
 *
 * \�������ƣ�
 *   CalImageLocation()
 *
 * \�������:
 *   ��
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   �ú������öԻ����еĿؼ�λ�úʹ�С����������ʾͼ���λ�á�Ĭ�ϵ�ͼ���СΪ352��288�����ͼ��С��
 *�˴�С����ؼ���С����Ϊ352��288������ͼ������ڿؼ��м䡣
 *
 *************************************************************************
 */
void CDlgReg::CalImageLocation()
{
	// ��ÿؼ�IDC_REG_INIT_IMAGE�ľ��������ÿؼ��ĳ�ʼλ����Ϣ
	CWnd* pWnd=GetDlgItem(IDC_REG_INIT_IMAGE);
	WINDOWPLACEMENT *winPlacement;
	winPlacement=new WINDOWPLACEMENT;
	pWnd->GetWindowPlacement(winPlacement);
	
	// ͼ����
	int nImageWidth;
	nImageWidth = m_pDibInit->m_lpBMIH->biWidth;

	// ͼ��߶�
	int nImageHeight;
	nImageHeight = m_pDibInit->m_lpBMIH->biHeight;

	
	// �����ؼ�IDC_REG_INIT_IMAGE�Ĵ�Сλ�ã���ͬʱ������ʾ��׼ͼ���λ��
	if(nImageHeight > 352){
		winPlacement->rcNormalPosition.bottom = winPlacement->rcNormalPosition.top + nImageHeight;	
		m_rectInitImage.bottom = winPlacement->rcNormalPosition.bottom;
		m_rectInitImage.top    = winPlacement->rcNormalPosition.top;		
	}
	else{
		winPlacement->rcNormalPosition.bottom = winPlacement->rcNormalPosition.top + 352;
		m_rectInitImage.bottom = winPlacement->rcNormalPosition.top + 176 + nImageHeight/2;
		m_rectInitImage.top    = winPlacement->rcNormalPosition.top + 176 - nImageHeight/2;
	}
	if(nImageWidth > 288){
		winPlacement->rcNormalPosition.right = winPlacement->rcNormalPosition.left + nImageWidth;
		m_rectInitImage.right = winPlacement->rcNormalPosition.right;
		m_rectInitImage.left  = winPlacement->rcNormalPosition.left;			
	}
	else{
		winPlacement->rcNormalPosition.right = winPlacement->rcNormalPosition.left + 288;
		m_rectInitImage.right = winPlacement->rcNormalPosition.left + 144 + nImageWidth/2;
		m_rectInitImage.left  = winPlacement->rcNormalPosition.left + 144 - nImageWidth/2;
	}

	// ����IDC_REG_INIT_IMAGE�ؼ��Ĵ�Сλ��
	pWnd->SetWindowPlacement(winPlacement);	
			
	// �����ʾ��׼ͼ��ؼ����ұ�λ�ã��Ա�ȷ����ʾ����׼ͼ��ؼ���λ��
	int nIniImgRight;
	nIniImgRight = winPlacement->rcNormalPosition.right;
	
	int  nIniImgLeft;
	nIniImgLeft   = winPlacement->rcNormalPosition.left;

	// ���IDC_REG_INIT_IMAGE�ؼ����±�λ�ã��Ա���������ؼ���λ��
	int nIniImgBottom;
	nIniImgBottom = winPlacement->rcNormalPosition.bottom;

	// ��ÿؼ�IDC_REG_RESLT_IMAGE�ľ��������ó�ʼλ����Ϣ
	pWnd=GetDlgItem(IDC_REG_RESLT_IMAGE);
	pWnd->GetWindowPlacement(winPlacement);
	
	// �����δ�򿪴���׼ͼ�������ô���׼ͼ���С�ͻ�׼ͼ���С���
	if(!m_pDibSamp->IsEmpty()){
		nImageWidth  = m_pDibSamp->m_lpBMIH->biWidth;
		nImageHeight = m_pDibSamp->m_lpBMIH->biHeight;
	}
	
	// �����ؼ�IDC_REG_RESLT_IMAGE�Ĵ�Сλ�ã���ͬʱ������ʾ����׼ͼ���λ��

	// �ȵ����ؼ������λ�ã���IDC_REG_INIT_IMAGE�ؼ����15������
	winPlacement->rcNormalPosition.left = nIniImgRight + 15;

	if(nImageHeight > 352){		
		winPlacement->rcNormalPosition.bottom = winPlacement->rcNormalPosition.top + nImageHeight;	
		m_rectResltImage.bottom = winPlacement->rcNormalPosition.bottom;
		m_rectResltImage.top    = winPlacement->rcNormalPosition.top;		
	}
	else{
		winPlacement->rcNormalPosition.bottom = winPlacement->rcNormalPosition.top + 352;
		m_rectResltImage.bottom = winPlacement->rcNormalPosition.top + 176 + nImageHeight/2;
		m_rectResltImage.top    = winPlacement->rcNormalPosition.top + 176 - nImageHeight/2;
	}
	if(nImageWidth > 288){
		winPlacement->rcNormalPosition.right = winPlacement->rcNormalPosition.left + nImageWidth;
		m_rectResltImage.right = winPlacement->rcNormalPosition.right;
		m_rectResltImage.left  = winPlacement->rcNormalPosition.left;			
	}
	else{
		winPlacement->rcNormalPosition.right = winPlacement->rcNormalPosition.left + 288;
		m_rectResltImage.right = winPlacement->rcNormalPosition.left + 144 + nImageWidth/2;
		m_rectResltImage.left  = winPlacement->rcNormalPosition.left + 144 - nImageWidth/2;
	}

	// ����IDC_REG_RESLT_IMAGE�ؼ��Ĵ�Сλ��
	pWnd->SetWindowPlacement(winPlacement);
	
	if(nIniImgBottom < winPlacement->rcNormalPosition.bottom)
		nIniImgBottom = winPlacement->rcNormalPosition.bottom;


	nIniImgBottom = winPlacement->rcNormalPosition.bottom;
	
	nIniImgRight  = winPlacement->rcNormalPosition.right;
	
	// ���ÿؼ�IDOK��λ�ô�С
	pWnd=GetDlgItem(IDOK);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom +15;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 60;
	pWnd->SetWindowPlacement(winPlacement);
	
	// ���ÿؼ�IDCANCEL��λ�ô�С
	pWnd=GetDlgItem(IDCANCEL);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom +15;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 60;
	pWnd->SetWindowPlacement(winPlacement);

	// ���ÿؼ�IDC_REG_OPEN��λ�ô�С
	pWnd=GetDlgItem(IDC_REG_OPEN);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom +15;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 60;
	pWnd->SetWindowPlacement(winPlacement);
	
	// ���ÿؼ�IDC_REG_REG��λ�ô�С
	pWnd=GetDlgItem(IDC_REG_REG);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom +15;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 60;
	pWnd->SetWindowPlacement(winPlacement);
	
	
	// ���ÿؼ�IDC_REG_CHOSE_FEATUR��λ�ô�С
	pWnd=GetDlgItem(IDC_REG_CHOSE_FEATURE);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom +15;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 60;
	pWnd->SetWindowPlacement(winPlacement);

	// �����˶Ի���Ĵ�С
	//pWnd = GetDlgItem(IDD_DLG_REG);
	this->GetWindowPlacement(winPlacement);
	//winPlacement->rcNormalPosition.top = nIniImgBottom +15;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 300;
	winPlacement->rcNormalPosition.left   = nIniImgLeft   - 20;
	winPlacement->rcNormalPosition.right  = nIniImgRight  + 20;
	this->SetWindowPlacement(winPlacement);

	// �ͷ��ѷ����ڴ�
	delete winPlacement;

	// ���ü���ͼ��ؼ�λ�ñ�־λΪTRUE
	m_bCalImgLoc = TRUE;
}

/*************************************************************************
 *
 * \�������ƣ�
 *   DrawFeature()
 *
 * \�������:
 *   ��
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   �ú���������ĳ�Ա����ȷ�����������Ŀ��λ�ã�����ͼ���н�����ʾ��
 *
 *************************************************************************
 */
void CDlgReg::DrawFeature(CDC* pDC)
{
	// ѭ������
	int i;

	// ��ʱ����
	CPoint pointTemp;

	// �뾶
	int nRadius;
	nRadius = 5;

	// ���û�ͼ����
	pDC->SelectStockObject(HOLLOW_BRUSH);

	// ��������
	CPen penWhite(PS_SOLID,1,RGB(255,255,255));
	CPen *pOldPen;

	// ������ѡ�룬��������ǰ�Ļ���
	pOldPen = pDC->SelectObject(&penWhite);

	for(i=0; i<m_nChsFeatureNum; i++){
		// ������ʾ������
		
		// ȷ���˵����ʾλ��
		pointTemp.x = m_pPointSampl[i].x + m_rectResltImage.left;
		pointTemp.y = m_pPointSampl[i].y + m_rectResltImage.top ;		

		// �����������㣬����԰�İ뾶ΪnRadius
		CRect rectSamp(pointTemp.x-nRadius , pointTemp.y-nRadius , 
			pointTemp.x+nRadius , pointTemp.y+nRadius);
		pDC->Ellipse(rectSamp);

		// ����ʾ��׼������
		// ȷ���˵����ʾλ��
		pointTemp.x = m_pPointBase[i].x + m_rectInitImage.left;
		pointTemp.y = m_pPointBase[i].y + m_rectInitImage.top ;		

		// �����������㣬����԰�İ뾶ΪnRadius
		CRect rectBase(pointTemp.x-nRadius , pointTemp.y-nRadius , 
			pointTemp.x+nRadius , pointTemp.y+nRadius);
		pDC->Ellipse(rectBase);
	}

	// �ظ���ǰ�Ļ���
	pDC->SelectObject(pOldPen);
	penWhite.DeleteObject();
}

/*************************************************************************
 *
 * \�������ƣ�
 *   OnRegOpen()
 *
 * \�������:
 *   ��
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   �ú����򿪴���׼ͼ�󣬲���ͼ������m_pDibSamp�С�
 *
 *************************************************************************
 */
void CDlgReg::OnRegOpen() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE,"bmp","*.bmp");
	if(dlg.DoModal() == IDOK)
	{
 
	 	CFile file;
	 
	 	CString strPathName;
 
		strPathName = dlg.GetPathName();
 
		// ���ļ�
		if( !file.Open(strPathName, CFile::modeRead | CFile::shareDenyWrite))
		{
			// ����	
			return ;
		}

		// ����ģ��ͼ��
		if(!m_pDibSamp->Read(&file)){
			// �ָ������״
			EndWaitCursor();

			// ����ѷ����ڴ�
			m_pDibSamp->Empty();

			// ����
 			return;
		}
	}
	
	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��ģ����׼�������Ŀ������ƣ�
	if(m_pDibSamp->m_nColorTableEntries != 256)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);

		// ����ѷ����ڴ�
		m_pDibSamp->Empty();

		// ����
		return;
	}

	// ������µĴ���׼�ļ�����ͼ��λ�����ñ�־λ��ΪFALSE���Ա��ٴε���λ��
	m_bCalImgLoc = FALSE;

	// ������ʾ
	this->UpdateData();
	this->Invalidate();
}



/*************************************************************************
 *
 * \�������ƣ�
 *   OnRegChoseFeatureOnRegReg()
 *
 * \�������:
 *   ��
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   �ú�������ѡȡ�������־λ��Ȼ����ú����ڴ���׼ͼ����ѡȡ�����㣬��
 *��׼��Щ�����㡣���������Ŀ����Ӧ��ѡȡ������
 *
 *************************************************************************
 */
void CDlgReg::OnRegChoseFeature() 
{
	// �������׼ͼ����δ�򿪣����ܽ���������ѡȡ����
	if((m_pDibSamp->IsEmpty())){
		AfxMessageBox("��δ�򿪴���׼ͼ���ļ�����򿪴���׼ͼ��");
		return;
	}
	// ����ѡȡ�������־λ
	m_bChoseFeature = TRUE;	
	
	AfxMessageBox("���ڴ���׼ͼ����ѡȡ������");
}

/*************************************************************************
 *
 * \�������ƣ�
 *   OnLButtonUp()
 *
 * \�������:
 *   ��
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   �ú�������������궨��λ�����������㡣Ȼ�����������׼������׼�������㡣
 *���������Ŀ����Ӧ��ѡȡ������
 *
 *************************************************************************
 */
void CDlgReg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// ѭ������
	int i,j;

	// �������ѡȡ��־λΪTRUE��������������ѡȡ����׼�������˳�
	if(!m_bChoseFeature){
		return;
	}

	// ����׼ͼ�������ѡȡ����������ѡ���������ѡ������Ҫ��ͼ�����
	// ��СһȦ
	CRect rectChoose;
	rectChoose.bottom = m_rectResltImage.bottom - 5;
	rectChoose.top    = m_rectResltImage.top + 5;
	rectChoose.left   = m_rectResltImage.left + 5;
	rectChoose.right  = m_rectResltImage.right - 5;

	// �����������
	CRect rectFeature;

	// ��־λ����ʾ�˵��Ƿ����Ѿ�ѡ���������
	BOOL bFlag = FALSE;

	// �жϴ˵��Ƿ�Ϸ������жϴ˵��Ƿ��Ѿ�ѡ������ǣ���ȥ���˵�
	if(rectChoose.PtInRect(point))
	{
		// �����ѡ�������������ǰ�������㣬��ȥ���˵�
		for( i = 0; i<m_nChsFeatureNum; i++){

			// ѡ�����������ʾ�����Ա�����������ȡ��
			rectFeature.bottom = m_pPointSampl[i].y + m_rectResltImage.top + 5;
			rectFeature.top    = m_pPointSampl[i].y + m_rectResltImage.top - 5;
			rectFeature.left   = m_pPointSampl[i].x + m_rectResltImage.left- 5;
			rectFeature.right  = m_pPointSampl[i].x + m_rectResltImage.left+ 5;

			// �ж���ѡ����������Ƿ�Ϊԭ��ѡ���������
			// ����ǣ���ȥ����������
			if(rectFeature.PtInRect(point)){
				// ���������������ǰ�ƶ�һλ��ȥ����ѡ��Ĵ�������
				for(j=i; j<m_nChsFeatureNum-1; j++){
					m_pPointSampl[j] = m_pPointSampl[j+1];
					m_pPointBase[j]  = m_pPointBase[j+1];						
				}

				// ��������ļ�����һ
				m_nChsFeatureNum--;
				
				// ������ʾ
				Invalidate();
				
				// ���ñ�־λ
				bFlag = TRUE;

				// �˳�
				return;
			}
		}
		
		// ���ж��������Ƿ��Ѿ�ѡȡ����
		if(m_nChsFeatureNum == 3){
			AfxMessageBox("���Ѿ�ѡȡ��3�������㣬���Ҫ����ѡȡ�������ȥ����׼����ȷ���������ڽ���ѡȡ");
			return;
		}

		// ����˵�����Ҫѡȡ�ģ��������ز���
		if(!bFlag){
			// ���˴���׼������ѡȡ��ע�����������������ͼ������Ͻ�Ϊԭ��ȷ����
			m_pPointSampl[m_nChsFeatureNum].x = point.x - m_rectResltImage.left;
			m_pPointSampl[m_nChsFeatureNum].y = point.y - m_rectResltImage.top;
			
			// ��׼��������
			m_pPointBase[m_nChsFeatureNum] = FindMatchPoint(m_pDibInit, m_pDibSamp, 
				m_pPointSampl[m_nChsFeatureNum]);

			// �������������һ
			m_nChsFeatureNum++;
		}
			
		
	}	

	// ������ʾ
	Invalidate();

	CDialog::OnLButtonUp(nFlags, point);
}

/*************************************************************************
 *
 * \�������ƣ�
 *   OnLButtonMove()
 *
 * \�������:
 *   ��
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   ���������ѡȡ��־λΪTRUE����ú���������׼������������Ϊʮ����״��
 *�Ա��ܸ���ȷ�Ķ�λ�����㡣
 *
 *************************************************************************
 */
void CDlgReg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// ���������������ѡȡ״̬���򲻽�����صĲ���
	if(m_bChoseFeature){
		// �������ڴ���׼ͼ�������У�����������״
		if(m_rectResltImage.PtInRect(point))
		{
			::SetCursor(LoadCursor(NULL,IDC_CROSS));
		}
		else
		{
			::SetCursor(LoadCursor(NULL,IDC_ARROW));
		}
	}
	CDialog::OnMouseMove(nFlags, point);
}

/*************************************************************************
 *
 * \�������ƣ�
 *   FindMatchPoint()
 *
 * \�������:
 *   CDib*	pDibBase	- ��׼ͼ��ָ��
 *   CDib*	pDibSamp	- ����׼ͼ��ָ��
 *   CPoint	pointSamp	- ����׼�������λ��
 *
 * \����ֵ:
 *   CPoint			- �����ڻ�׼ͼ������׼���������λ��
 *
 * \˵��:
 *   �ú������ݴ�����׼ͼ���е�������λ���ڻ�׼ͼ����Ѱ����׼�����㣬����
 *��׼��������λ�÷��ء�����׼�Ĺ����У���ȡ���ǿ���׼�ķ���������׼��������
 *ѡȡ��Ĵ�СΪ7*7�������ķ���Ϊȫ��������
 *
 *************************************************************************
 */
CPoint CDlgReg::FindMatchPoint(CDib* pDibBase, CDib* pDibSamp, 
			       CPoint pointSamp)
{
	// ѭ������
	int i,j,m,n;

	// ��ʱ����
	int nX, nY;

	// ��׼������λ��
	CPoint pointBase;

	// ��׼���ݿ�ĳߴ�
	int nBlockLen = 7;
	int nBlockHalfLen =3;

	// ��׼ͼ������ָ��	
	unsigned char* pBase;
	pBase = (unsigned char *)pDibBase->m_lpImage;
	
	// ����׼ͼ������ָ��
	unsigned char* pSamp;
	pSamp = (unsigned char *)pDibSamp->m_lpImage;		

	// ������λ�õ�������׼��
	unsigned char* pUnchSampBlock;
	pUnchSampBlock = new unsigned char[nBlockLen*nBlockLen];

	// ��ʱ�����ڴ棬���ڴ����׼���ݿ�
	unsigned char* pUnchBaseBlock;
	pUnchBaseBlock = new unsigned char[nBlockLen*nBlockLen];

	// ���ƶ�
	double dbCor;

	// ������ƶ�
	double dbMaxCor = 0;

	// ��׼ͼ��Ĵ洢��С
	CSize sizeBaseImg;
	sizeBaseImg = pDibBase->GetDibSaveDim();

	// ����׼ͼ��Ĵ洢��С
	CSize sizeSampImg;
	sizeSampImg = pDibSamp->GetDibSaveDim();

	// �Ӵ���׼ͼ������ȡ��������Ϊ���ĵ�nBlockLen*nBlockLen0�����ݿ�
	for(i=-nBlockHalfLen; i<=nBlockHalfLen; i++){
		for(j=-nBlockHalfLen; j<=nBlockHalfLen; j++){
			// ����˵���ͼ���е�λ��
			nX = pointSamp.x + i;
			nY = sizeSampImg.cy - pointSamp.y + j +1;

			// ��ȡͼ������
			pUnchSampBlock[(j+nBlockHalfLen)*nBlockLen + (i+nBlockHalfLen)] = 
				pSamp[nY*sizeSampImg.cx + nX];				
		}
	}

	// ��׼ͼ��ĸ߶ȺͿ��
	int nBaseImgHeight, nBaseImgWidth;
	nBaseImgHeight = pDibBase->m_lpBMIH->biHeight;
	nBaseImgWidth  = pDibBase->m_lpBMIH->biWidth;

	// �ڻ�׼ͼ����Ѱ����׼�����㣬��ȡ����������Ϊȫ������
	for(m = nBlockHalfLen; m< nBaseImgHeight-nBlockHalfLen; m++){
		for(n=nBlockHalfLen; n<nBaseImgWidth-nBlockHalfLen; n++){
			// ��ȡ�Դ˵�Ϊ���ģ���СΪnBlockLen*nBlockLen�����ݿ�
			for(i=-nBlockHalfLen; i<=nBlockHalfLen; i++){
				for(j=-nBlockHalfLen; j<=nBlockHalfLen; j++){
					// ����˵���ͼ���д洢��λ��
					nX = n + i;
					nY = sizeBaseImg.cy - m + j - 1;

					// ��ȡͼ������
					pUnchBaseBlock[(j+nBlockHalfLen)*nBlockLen + (i+nBlockHalfLen)] = 
						pBase[nY*sizeBaseImg.cx + nX];				
				}
			}

			// �����������ݿ������׼���������ƶ�
			dbCor = CalCorrelation(pUnchBaseBlock, pUnchSampBlock, nBlockLen);

			// �ж��Ƿ�Ϊ������ƶȣ�����ǣ����¼�����ƶȺ���׼������λ��
			if(dbCor > dbMaxCor){
				dbMaxCor  = dbCor;
				pointBase.x = n;
				pointBase.y = m;
			}

		}
	}
	
	return pointBase;
}

/*************************************************************************
 *
 * \�������ƣ�
 *   CalCorrelation()
 *
 * \�������:
 *   unsigned char*	pBase		- ��׼ͼ������ָ��
 *   unsigned char*	pSamp		- ����׼ͼ������ָ��
 *   int		nBlockLen	- ��׼���ݿ�ĳ߶ȴ�С
 *
 * \����ֵ:
 *   double			- �����������ݿ���׼�����ƶ�
 *
 * \˵��:
 *   �ú����Ը�����������СΪnBlockLen*nBlockLen�����ݿ飬��������֮���
 *����׼���ƶȡ����У�ȥ����ֵ���������ȱ任��Ӱ�졣
 *
 *************************************************************************
 */
double CDlgReg::CalCorrelation(unsigned char* pBase, unsigned char* pSamp, int nBlockLen)
{
	// ��ʱ����
	double dbSelfBase=0,dbSelfSamp=0;

	// ���ƶ�
	double dbCor=0;

	// ���ֵ
	double dbMeanBase=0,dbMeanSamp=0;

	// �����������ƽ��ֵ
	for(int i=0;i<nBlockLen;i++)
		for(int j=0;j<nBlockLen;j++)
		{
			dbMeanBase += pBase[j*nBlockLen + i];
			dbMeanSamp += pSamp[j*nBlockLen + i];
		}
	dbMeanBase = dbMeanBase/(nBlockLen*nBlockLen);
	dbMeanSamp = dbMeanSamp/(nBlockLen*nBlockLen);

	// ��ȡ���ƶ�
	for(i=0;i<nBlockLen;i++)
		for(int j=0;j<nBlockLen;j++)
		{
			dbSelfBase += (pBase[j*nBlockLen + i] - dbMeanBase)*
				(pBase[j*nBlockLen + i]  - dbMeanBase);
			dbSelfSamp += (pSamp[j*nBlockLen + i] - dbMeanSamp)*
				(pSamp[j*nBlockLen + i]  - dbMeanSamp);
			dbCor += (pBase[j*nBlockLen + i] - dbMeanBase)*
				(pSamp[j*nBlockLen + i]  - dbMeanSamp);
		}
	dbCor = dbCor / sqrt(dbSelfBase * dbSelfSamp);

	// �������ƶ�
	return dbCor;
}

/*************************************************************************
 *
 * \�������ƣ�
 *   OnRegReg()
 *
 * \�������:
 *   ��
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   �ú���������ͼ��m_pDibInit��m_pDibSamp������׼�����У���Ҫ��ȷ�������㣬
 *������������Ҫѡȡ3�����ϣ���һ������Ҫ�ڡ�ѡȡ�����㡱�����н��С�����ѡȡ
 *�������㣬����õ������������󣬽���׼������ͼ��ƴ��������ʾ��
 *
 *************************************************************************
 */
void CDlgReg::OnRegReg() 
{
	// ������׼֮ǰ���ж��Ƿ��Ѿ�ѡȡ������
	if(!m_bChoseFeature){
		AfxMessageBox("��û��ѡȡ�����㣬����ѡȡ������");
		return;
	}

	// ���ѡȡ����������Ŀ������Ҳ���ܽ��д���
	if(m_nChsFeatureNum < 3){
		AfxMessageBox("��ѡ�����������㣬�ٽ���ͼ����׼");
		return;
	}

	// ����任ϵ��������任��ϵ��Ϊ6������ϵ��Ϊ�ӻ�׼ͼ�󵽴���׼ͼ��ı任ϵ��
	double *pDbBs2SpAffPara;
	pDbBs2SpAffPara = new double[2*3];

	// ����任ϵ��������任��ϵ��Ϊ6������ϵ��Ϊ�Ӵ���׼ͼ�󵽻�׼ͼ��ı任ϵ��
	double *pDbSp2BsAffPara;
	pDbSp2BsAffPara = new double[2*3];

	// ����Ѿ�ѡȡ�������㣬���õ�����׼�������㣬����Լ������任ϵ����
	GetAffinePara(m_pPointBase, m_pPointSampl, pDbSp2BsAffPara);
	
	// ����Ӵ���׼ͼ�󵽻�׼ͼ��ķ���任ϵ��
	GetAffinePara(m_pPointSampl, m_pPointBase, pDbBs2SpAffPara);

	// ����ͼ�󾭹�����任��ĳߴ��С
	CRect rectAftAff;
	rectAftAff = GetAftAffDim(pDbSp2BsAffPara);
	
	// ����ͼ��ĳߴ��С�����µ�ͼ��
	//CreateDIB();
	int nNewImgWidth, nNewImgHeight;
	nNewImgWidth  = rectAftAff.right - rectAftAff.left;
	nNewImgHeight = rectAftAff.bottom- rectAftAff.top;

	
	// ����׼ͼ������µ�ͼ����
	LPBYTE lpBaseImg;
	lpBaseImg = SetBaseImgAftAff(rectAftAff);

	// ������׼ͼ������µ�ͼ����
	LPBYTE lpSampImg;
	lpSampImg = SetSampImgAftAff(pDbBs2SpAffPara, rectAftAff);

	// ����ͼ����CDib���װ
	m_pDibResult = new CDib(CSize(nNewImgWidth,nNewImgHeight), 8);

	// ������ͼ��Ĵ洢��С�ߴ�
	CSize sizeSaveResult;
	sizeSaveResult = m_pDibResult->GetDibSaveDim();

	// ������ɫ��
	memcpy(m_pDibResult->m_lpvColorTable, m_pDibInit->m_lpvColorTable, m_pDibResult->m_nColorTableEntries*sizeof(RGBQUAD));

	// Ӧ�õ�ɫ��
	m_pDibResult->MakePalette();
	
	// �����ڴ���ϲ����ͼ��
	LPBYTE lpImgResult;
	lpImgResult = (LPBYTE)new unsigned char[sizeSaveResult.cx * sizeSaveResult.cy];

	// ��ͼ����и�ֵ
	for( int i=0; i<nNewImgWidth; i++)
		for( int j=0; j<nNewImgHeight; j++){
			int nX = i;
			int nY = sizeSaveResult.cy - j - 1;
			lpImgResult[nY*sizeSaveResult.cx + nX] = (lpBaseImg[j*nNewImgWidth + i] +lpSampImg[j*nNewImgWidth + i])/2;
		}

	// ��ָ�븳ֵ��CDib�������
	m_pDibResult->m_lpImage = lpImgResult;

	// ��ʾ�ϲ����ͼ��
	CDlgAftReg* pDlg;
	pDlg = new CDlgAftReg(NULL, m_pDibResult);
	pDlg->DoModal();
	
	// ɾ������
	delete pDlg;

	// �ͷ��ѷ����ڴ�
	delete[]lpBaseImg;
	delete[]lpSampImg;
	delete[]pDbBs2SpAffPara;
	delete[]pDbSp2BsAffPara;
}

/*************************************************************************
 *
 * \�������ƣ�
 *   GetAffinePara()
 *
 * \�������:
 *   double  *pDbBs2SpAffPara	- ���ڴ�Ż�׼ͼ�󵽴���׼ͼ��ķ���任ϵ��
 *   double  *pDbSp2BsAffPara	- ���ڴ�Ŵ���׼ͼ�󵽻�׼ͼ��ķ���任ϵ��
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   �ú������ݵõ���������׼�������㣬�������任ϵ�����õ��ķ���任ϵ��
 *������������������ָ����ڴ��С�
 *
 *************************************************************************
 */
void CDlgReg::GetAffinePara(CPoint* pPointBase, CPoint* pPointSampl, double* pDbAffPara)
{
	// pDbBMatrix�д�ŵ��ǻ�׼ͼ��������������꣬
	// ��СΪ2*m_nChsFeatureNum��ǰm_nChsFeatureNumΪX����
	double *pDbBMatrix;
	pDbBMatrix = new double[2*m_nChsFeatureNum];

	// pDbSMatrix�д�ŵ��Ǵ���׼ͼ�������������չ����
	// ��СΪ3*m_nChsFeatureNum������ǰm_nChsFeatureNumΪX����
	// �м�m_nChsFeatureNum��ΪY���꣬����m_nChsFeatureNumΪ1
	double *pDbSMatrix;
	pDbSMatrix = new double[3*m_nChsFeatureNum];

	// pDbSMatrixT�д�ŵ�pDbSMatrix��ת�þ���
	// ��СΪm_nChsFeatureNum*3
	double *pDbSMatrixT;
	pDbSMatrixT = new double[m_nChsFeatureNum*3];

	// pDbInvMatrixΪ��ʱ��������ŵ���pDbSMatrix*pDbSMatrixT����
	// ��СΪ3*3
	double *pDbInvMatrix;
	pDbInvMatrix = new double[3*3];

	// ��ʱ�ڴ�
	double *pDbTemp;
	pDbTemp = new double[2*3];

	// ѭ������
	int count;

	// ������ֵ
	for(count = 0; count<m_nChsFeatureNum; count++)
	{
		pDbBMatrix[0*m_nChsFeatureNum + count] = pPointBase[count].x;
		pDbBMatrix[1*m_nChsFeatureNum + count] = pPointBase[count].y;
		pDbSMatrix[0*m_nChsFeatureNum + count] = pPointSampl[count].x;
		pDbSMatrix[1*m_nChsFeatureNum + count] = pPointSampl[count].y;
		pDbSMatrix[2*m_nChsFeatureNum + count] = 1;
		pDbSMatrixT[count*m_nChsFeatureNum + 0] = pPointSampl[count].x;
		pDbSMatrixT[count*m_nChsFeatureNum + 1] = pPointSampl[count].y;
		pDbSMatrixT[count*m_nChsFeatureNum + 2] = 1;		
	}

	// ����pDbSMatrix*pDbSMatrixT�������������pDbInvMatrix��
	CalMatProduct(pDbSMatrix,pDbSMatrixT,pDbInvMatrix,3,3,m_nChsFeatureNum);

	// ����pDbInvMatrix�������
	CalInvMatrix(pDbInvMatrix, 3);

	// �������任ϵ��
	CalMatProduct(pDbBMatrix, pDbSMatrixT, pDbTemp, 2, 3, m_nChsFeatureNum);
	CalMatProduct(pDbTemp, pDbInvMatrix, pDbAffPara, 2, 3, 3);

	// �ͷ��ڴ�
	delete[]pDbBMatrix;
	delete[]pDbSMatrix;
	delete[]pDbSMatrixT;
	delete[]pDbInvMatrix;
	delete[]pDbTemp;

}

/*************************************************************************
 *
 * \�������ƣ�
 *   CalMatProduct()
 *
 * \�������:
 *   double  *pDbSrc1	- ָ����˾���1���ڴ�
 *   double  *pDbSrc2	- ָ����˾���2���ڴ�
 *   double  *pDbDest   - ��ž���������н�����ڴ�ָ��
 *   int     nX		- ����ĳߴ磬����μ�����˵��
 *   int     nY		- ����ĳߴ磬����μ�����˵��
 *   int     nZ		- ����ĳߴ磬����μ�����˵��
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   �ú������������������ˣ�Ȼ����˵Ľ�������pDbDest�С�����pDbSrc1
 *�Ĵ�СΪnX*nZ��pDbSrc2�Ĵ�СΪnZ*nY��pDbDest�Ĵ�СΪnX*nY
 *
 *************************************************************************
 */
void CDlgReg::CalMatProduct(double* pDbSrc1, double *pDbSrc2, double *pDbDest, int y, int x, int z)
{
	// ѭ������
	//int i,j,m;
/*
	// �������
	for( i=0;i<nX;i++)
		for( j=0;j<nY;j++)
		{
			pDbDest[i*nY + j] = 0;
			for( m=0; m<nZ; m++)
				pDbDest[i*nY + j] += pDbSrc1[i*nZ + m]*pDbSrc2[m *nY + j];
		}
		*/
	for(int i=0;i<y;i++)
		for(int j=0;j<x;j++)
		{
			pDbDest[i*x + j] = 0;
			for(int m=0;m<z;m++)
				pDbDest[i*x + j] += pDbSrc1[i*z + m]*pDbSrc2[m*x + j];
		}
}

/*************************************************************************
 *
 * \�������ƣ�
 *   CalInvMatrix()
 *
 * \�������:
 *   double  *pDbSrc	- ָ������ָ��
 *   int     nLen	- ����ĳߴ� 
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   �ú����������pDbSrc�����������pDbSrc�Ĵ�СΪnLen*nLen
 *
 *************************************************************************
 */
BOOL CDlgReg::CalInvMatrix(double *pDbSrc, int nLen)
{
	int *is,*js,i,j,k;
	double d,p;
	is = new int[nLen];
	js = new int[nLen];
	for(k=0;k<nLen;k++)
	{
		d=0.0;
		for(i=k;i<nLen;i++)
			for(j=k;j<nLen;j++)
			{
				p=fabs(pDbSrc[i*nLen + j]);
				if(p>d)
				{
					d     = p; 
					is[k] = i;
					js[k] = j;
				}
			}
		if(d+1.0==1.0)
		{
			delete is;
			delete js;
			return FALSE;
		}
		if(is[k] != k)
			for(j=0;j<nLen;j++)
			{
				p = pDbSrc[k*nLen + j];
				pDbSrc[k*nLen + j] = pDbSrc[(is[k]*nLen) + j];
				pDbSrc[(is[k])*nLen + j] = p;
			}
		if(js[k] != k)
			for(i=0; i<nLen; i++)
			{
				p = pDbSrc[i*nLen + k];
				pDbSrc[i*nLen + k] = pDbSrc[i*nLen + (js[k])];
				pDbSrc[i*nLen + (js[k])] = p;
			}

		pDbSrc[k*nLen + k]=1.0/pDbSrc[k*nLen + k];
		for(j=0; j<nLen; j++)
			if(j != k)
			{
				pDbSrc[k*nLen + j]*=pDbSrc[k*nLen + k];
			}
		for(i=0; i<nLen; i++)
			if(i != k)
				for(j=0; j<nLen; j++)
					if(j!=k)
					{
						pDbSrc[i*nLen + j] -= pDbSrc[i*nLen + k]*pDbSrc[k*nLen + j];
					}
		for(i=0; i<nLen; i++)
			if(i != k)
			{
				pDbSrc[i*nLen + k] *= -pDbSrc[k*nLen + k];
			}
	}
	for(k=nLen-1; k>=0; k--)
	{
		if(js[k] != k)
			for(j=0; j<nLen; j++)
			{
				p = pDbSrc[k*nLen + j];
				pDbSrc[k*nLen + j] = pDbSrc[(js[k])*nLen + j];
				pDbSrc[(js[k])*nLen + j] = p;
			}
		if(is[k] != k)
			for(i=0; i<nLen; i++)
			{
				p = pDbSrc[i*nLen + k];
				pDbSrc[i*nLen + k] = pDbSrc[i*nLen +(is[k])];
				pDbSrc[i*nLen + (is[k])] = p;
			}
	}
	delete is;

	return TRUE;	
}

/*************************************************************************
 *
 * \�������ƣ�
 *   CalInvMatrix()
 *
 * \�������:
 *   double  *pDbAffPara	- ����任ϵ������
 *
 * \����ֵ:
 *   CRect			- ���ش���׼ͼ�󾭷���任�������
 *
 * \˵��:
 *   �ú������ݷ���任ϵ�����������׼ͼ�����任���ͼ��ߴ��С
 *
 *************************************************************************
 */
CRect CDlgReg::GetAftAffDim(double* pDbAffPara)
{
	// ��׼ͼ��Ŀ�Ⱥ͸߶�
	int nBaseImgWidth, nBaseImgHeight;
	nBaseImgWidth = m_pDibInit->m_lpBMIH->biWidth;
	nBaseImgHeight= m_pDibInit->m_lpBMIH->biHeight;

	// ����׼ͼ��Ŀ�Ⱥ͸߶�
	int nSamplImgWidth, nSamplImgHeight;
	nSamplImgWidth = m_pDibSamp->m_lpBMIH->biWidth;
	nSamplImgHeight= m_pDibSamp->m_lpBMIH->biHeight;

	// ��׼ͼ���ԭʼ����
	CRect rectBase(0,0,nBaseImgWidth,nBaseImgHeight);

	// ��ʱ����
	CPoint pointTemp;
	double tx,ty;
	
	// ͼ��Ķ˵�
	pointTemp.x = 0; 
	pointTemp.y = 0;

	// �����pointTemp��������任�������
	tx = pDbAffPara[0*3 +0]*pointTemp.x + 
		pDbAffPara[0*3 + 1]*pointTemp.y + pDbAffPara[0*3 + 2];
	ty = pDbAffPara[1*3 + 0]*pointTemp.x + 
		pDbAffPara[1*3 + 1]*pointTemp.y + pDbAffPara[1*3 + 2];

	// �ж�pointTemp��������任���Ƿ񳬳�ԭ���Ĵ�С
	if(tx<rectBase.left)
		rectBase.left = (int)tx;
	if(tx>rectBase.right)
		rectBase.right = (int)tx+1;
	if(ty<rectBase.top)
		rectBase.top = (int)ty;
	if(ty>rectBase.bottom)
		rectBase.bottom = (int)ty+1;

	// ����˵�(0, nSamplImgHeight)�任�������
	pointTemp.x = 0; pointTemp.y = nSamplImgHeight;
	tx = pDbAffPara[0*3 + 0]*pointTemp.x + 
		pDbAffPara[0*3 + 1]*pointTemp.y + pDbAffPara[0*3 + 2];
	ty = pDbAffPara[1*3 +0]*pointTemp.x +
		pDbAffPara[1*3 + 1]*pointTemp.y + pDbAffPara[1*3 + 2];

	// �ж��Ƿ�Խ��
	if(tx<rectBase.left)
		rectBase.left = (int)tx;
	if(tx>rectBase.right)
		rectBase.right = (int)tx+1;
	if(ty<rectBase.top)
		rectBase.top = (int)ty;
	if(ty>rectBase.bottom)
		rectBase.bottom = (int)ty+1;

	// ����˵�(nSamplImgWidth, nSamplImgHeight)�任�������
	pointTemp.x = nSamplImgWidth; pointTemp.y = nSamplImgHeight;
	tx = pDbAffPara[0*3 + 0]*pointTemp.x +
		pDbAffPara[0*3 + 1]*pointTemp.y + pDbAffPara[0*3 + 2];
	ty = pDbAffPara[1*3 + 0]*pointTemp.x +
		pDbAffPara[1*3 + 1]*pointTemp.y + pDbAffPara[1*3 + 2];

	// �ж��Ƿ�Խ��
	if(tx<rectBase.left)
		rectBase.left = (int)tx;
	if(tx>rectBase.right)
		rectBase.right = (int)tx+1;
	if(ty<rectBase.top)
		rectBase.top = (int)ty;
	if(ty>rectBase.bottom)
		rectBase.bottom = (int)ty+1;

	// ����˵�(nSamplImgWidth, 0)�任�������
	pointTemp.x = nSamplImgWidth; pointTemp.y = 0;
	tx = pDbAffPara[0*3 + 0]*pointTemp.x +
		pDbAffPara[0*3 + 1]*pointTemp.y + pDbAffPara[0*3 + 2];
	ty = pDbAffPara[1*3 + 0]*pointTemp.x +
		pDbAffPara[1*3 + 1]*pointTemp.y + pDbAffPara[1*3 + 2];

	// �ж��Ƿ�Խ��
	if(tx<rectBase.left)
		rectBase.left = (int)tx;
	if(tx>rectBase.right)
		rectBase.right = (int)tx+1;
	if(ty<rectBase.top)
		rectBase.top = (int)ty;
	if(ty>rectBase.bottom)
		rectBase.bottom = (int)ty+1;

	// ���ش���׼ͼ��任��������С
	return(rectBase);

}

/*************************************************************************
 *
 * \�������ƣ�
 *   SetSampImgAftAff()
 *
 * \�������:
 *   double  *pDbAffPara	- ����任ϵ������
 *   CRect   rectNewImg		- �任��ͼ��Ĵ�С�ߴ�
 *
 * \����ֵ:
 *   LPBYTE			- ���ر任���ͼ��
 *
 * \˵��:
 *   �ú������ݷ���任ϵ�����������׼ͼ�����任���ͼ�󡣲����ش�ͼ��ָ��
 *��ͼ��Ĵ�СΪrectNewImg
 *
 *************************************************************************
 */
LPBYTE CDlgReg::SetSampImgAftAff(double* pDbAffPara, CRect rectNewImg)
{
	// pUnchSect��4*4��С�ľ�������
	unsigned char *pUnchSect;
	pUnchSect = new unsigned char[4*4];

	// �µ�ͼ���Ⱥ͸߶�
	int nNewImgWidth, nNewImgHeight;
	nNewImgWidth  = rectNewImg.right - rectNewImg.left;
	nNewImgHeight = rectNewImg.bottom- rectNewImg.top;

	// ����׼ͼ��Ŀ�Ⱥ͸߶�
	int nSamplImgWidth, nSamplImgHeight;
	nSamplImgWidth = m_pDibSamp->m_lpBMIH->biWidth;
	nSamplImgHeight= m_pDibSamp->m_lpBMIH->biHeight;

	// ����׼ͼ��Ĵ洢���
	int nSampSaveWidth;
	nSampSaveWidth = m_pDibSamp->GetDibSaveDim().cx;
	
	// pUnchAftAffSamp��һ����СΪrectNewImg��С��ͼ��
	// ����rectNewImg��ʾ�任���ͼ���С
	unsigned char *pUnchAftAffSamp;
	pUnchAftAffSamp = new unsigned char[nNewImgWidth * nNewImgHeight];
	
	double tx,ty;

	// �����ڱ任���ͼ�������
	for(int i=0;i<rectNewImg.bottom-rectNewImg.top;i++)
		for(int j=0;j<rectNewImg.right-rectNewImg.left;j++)
		{
			tx = pDbAffPara[0*3 + 0]*(j+rectNewImg.left) +
				pDbAffPara[0*3 + 1]*(i+rectNewImg.top) + pDbAffPara[0*3 + 2];
			ty = pDbAffPara[1*3 + 0]*(j+rectNewImg.left) + 
				pDbAffPara[1*3 + 1]*(i+rectNewImg.top) + pDbAffPara[1*3 + 2];

			for(int m=(int)ty-1;m<=(int)ty+2;m++)
				for(int n=(int)tx-1;n<=(int)tx+2;n++)
				{
					if(m<0||m>=nSamplImgHeight||n<0||n>=nSamplImgWidth)
						pUnchSect[(m-(int)ty+1)*4 + (n-(int)tx+1)] = 0;
					else
						pUnchSect[(m-(int)ty+1)*4 + (n-(int)tx+1)] = 
							m_pDibSamp->m_lpImage[(nSamplImgHeight-m-1)*nSampSaveWidth + n];
				}

			// ȷ���任������
			ty = ty - (int)ty + 1;
			tx = tx - (int)tx + 1;

			// ȷ���任����������ֵ
			pUnchAftAffSamp[i*nNewImgWidth + j] = CalSpline(pUnchSect,tx,ty);
		}
	
	
	// �Ƿ��ڴ�
	delete[]pUnchSect;

	// ����ָ��
	return (LPBYTE)pUnchAftAffSamp;
	
	
}

/*************************************************************************
 *
 * \�������ƣ�
 *   CalSpline()
 *
 * \�������:
 *   unsigned char	*pUnchCorr	- ��ֵ�ĵ�
 *   double		dX		- X����
 *   double		dY		- Y����
 *
 * \����ֵ:
 *   unsigned char			- ��ֵ���ֵ
 *
 * \˵��:
 *   �ú��������ڽ�λ�õ���ֵ���в�ֵ��
 *��ͼ��Ĵ�СΪrectNewImg
 *
 *************************************************************************
 */
unsigned char CDlgReg::CalSpline(unsigned char *pUnchCorr, double x, double y)
{
	double ret=0, Cx, Cy;
	double Temp;

	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
		{
			Temp = pUnchCorr[i*4 + j];
			if(fabs(y-i)<1)
				Cy = 1-2*fabs(y-i)*fabs(y-i)+fabs(y-i)*fabs(y-i)*fabs(y-i);
			if(fabs(y-i)>=1)
				Cy = 4-8*fabs(y-i)+5*fabs(y-i)*fabs(y-i)-fabs(y-i)*fabs(y-i)*fabs(y-i);
			if(fabs(x-j)<1)
				Cx = 1-2*fabs(x-j)*fabs(x-j)+fabs(x-j)*fabs(x-j)*fabs(x-j);
			if(fabs(x-j)>=1)
				Cx = 4-8*fabs(x-j)+5*fabs(x-j)*fabs(x-j)-fabs(x-j)*fabs(x-j)*fabs(x-j);
			ret += Temp*Cy*Cx;
		}
	if(ret<0)
		ret=0;
	if(ret>255)
		ret=255;

	return (unsigned char)ret;

}

/*************************************************************************
 *
 * \�������ƣ�
 *   SetBaseImgAftAff()
 *
 * \�������:
 *   double  *pDbAffPara	- ����任ϵ������
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   �ú������ݷ���任ϵ���������׼ͼ�����任���ͼ�󣬲����ش�Ŵ�
 *���ݵ�ָ��
 *
 *************************************************************************
 */
LPBYTE CDlgReg::SetBaseImgAftAff(CRect rectNewImg)
{
	// ��ͼ��Ĵ�С
	int nNewImgWidth, nNewImgHeight;
	nNewImgWidth  = rectNewImg.right  - rectNewImg.left;
	nNewImgHeight = rectNewImg.bottom - rectNewImg.top;	

	// �任��ͼ��
	unsigned char *pUnchAftAffBase;
	pUnchAftAffBase = new unsigned char[nNewImgWidth*nNewImgHeight];

	// ��׼ͼ��ĸ߶ȺͿ��
	int nBaseWidth, nBaseHeight;
	nBaseWidth  = m_pDibInit->m_lpBMIH->biWidth;
	nBaseHeight = m_pDibInit->m_lpBMIH->biHeight;

	// ��׼ͼ��Ĵ洢���
	int nBaseSaveWidth;
	nBaseSaveWidth = m_pDibInit->GetDibSaveDim().cx;
	
	for(int i=0;i<rectNewImg.bottom-rectNewImg.top;i++)
		for(int j=0;j<rectNewImg.right-rectNewImg.left;j++)
		{
			if(i<-rectNewImg.top||i>=-rectNewImg.top+nBaseHeight||j<-rectNewImg.left||j>=-rectNewImg.left+nBaseWidth)
				pUnchAftAffBase[i*nNewImgWidth + j] = 0;
			else
				pUnchAftAffBase[i*nNewImgWidth + j] = m_pDibInit->m_lpImage[(nBaseHeight - (i+rectNewImg.top) - 1)*nBaseSaveWidth + (j+rectNewImg.left)];
		}

	// ����
	return (LPBYTE)pUnchAftAffBase;

}
