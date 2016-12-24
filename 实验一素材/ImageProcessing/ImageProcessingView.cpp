// ImageProcessingView.cpp : implementation of the CImageProcessingView class
//

#include "stdafx.h"
#include "ImageProcessing.h"

#include "ImageProcessingDoc.h"
#include "ImageProcessingView.h"
#include  "GlobalApi.h"
#include  "DlgCoding.h"
#include <complex>
using namespace std;

#include "DlgHistShow1.h"
#include "DlgSmooth.h"
#include "DlgMedian.h"
#include "DlgEnhColor.h"
#include "DlgEhnLinTrans.h"
#include "DlgReg.h"
#include "DlgRecMatch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingView

IMPLEMENT_DYNCREATE(CImageProcessingView, CScrollView)

BEGIN_MESSAGE_MAP(CImageProcessingView, CScrollView)
	//{{AFX_MSG_MAP(CImageProcessingView)
	ON_COMMAND(ID_FFT_2D, OnFft2d)
	ON_COMMAND(ID_DFT_2D, OnDft2d)
	ON_COMMAND(ID_VIEW_HISTOGRAM, OnViewHistogram)
	ON_COMMAND(ID_ENHANCE_SMOOTH, OnEnhanceSmooth)
	ON_COMMAND(ID_ENHANCE_MEDIAN, OnEnhanceMedian)
	ON_COMMAND(ID_ENHANCE_PSEUDCOLOR, OnEnhancePseudcolor)
	ON_COMMAND(ID_TRANS_DWT, OnTransDwt)
	ON_COMMAND(ID_TRANS_IDWT, OnTransIdwt)
	ON_COMMAND(IDC_ENHANCE_LINTRANS, OnEnhanceLintrans)
	ON_COMMAND(IDC_ENHANCE_HISTEQU, OnEnhanceHistequ)
	ON_COMMAND(ID_REG_REG, OnRegReg)
	ON_COMMAND(IDC_ENHANCE_SHARP, OnEnhanceSharp)
	ON_COMMAND(ID_ENHANCE_SMOOTH_FR, OnEnhanceSmoothFr)
	ON_COMMAND(IDC_ENHANCE_BUTT_LOW, OnEnhanceButtLow)
	ON_COMMAND(IDC_ENHANCE_SHARP_FREQ, OnEnhanceSharpFreq)
	ON_COMMAND(IDC_ENHANCE_BUTT_HIGHT, OnEnhanceButtHight)
	ON_COMMAND(ID_REGIONSEG_FIX, OnRegionsegFix)
	ON_COMMAND(ID_ADA_REGION_SEG, OnAdaRegionSeg)
	ON_COMMAND(ID_EDGE_ROBERTS, OnEdgeRoberts)
	ON_COMMAND(ID_EDGE_SOBEL, OnEdgeSobel)
	ON_COMMAND(ID_EDGE_PREWITT, OnEdgePrewitt)
	ON_COMMAND(ID_EDGE_LAPLACE, OnEdgeLaplace)
	ON_COMMAND(ID_EDGE_CANNY, OnEdgeCanny)
	ON_COMMAND(ID_EDGE_TRACK, OnEdgeTrack)
	ON_COMMAND(ID_REGION_GROW, OnRegionGrow)
	ON_COMMAND(ID_MOTION_BACKGROUND, OnMotionBackground)
	ON_COMMAND(ID_RECOG_MATCH, OnRecogMatch)
	ON_COMMAND(ID_CODING_SHANFINO, OnCodingShanfino)
	ON_COMMAND(ID_DEGENERATION_INVERSE, OnDegenerationInverse)
	ON_COMMAND(ID_DEGENERATION_MOTION, OnDegenerationMotion)
	ON_COMMAND(ID_DEGENERATION_Winner, OnDEGENERATIONWinner)
	ON_COMMAND(ID_RESTORE_INVERSE, OnRestoreInverse)
	ON_COMMAND(ID_RESTORE_MOTION, OnRestoreMotion)
	ON_COMMAND(ID_RESTORE_WINNER, OnRestoreWinner)
	ON_COMMAND(ID_STREET_FRAMEWORK, OnStreetFramework)
	ON_COMMAND(ID_STREET_TRANSFORM, OnStreetTransform)
	ON_COMMAND(ID_TRACE, OnTrace)
	ON_COMMAND(ID_VIEW_BAYER, OnViewBayer)
	ON_COMMAND(ID_VIEW_FloydSteinberg, OnVIEWFloydSteinberg)
	ON_COMMAND(ID_OUTLINE, OnOutline)
	ON_COMMAND(ID_FRAME_RESTORE, OnFrameRestore)
	ON_COMMAND(ID_MOMENT, OnMoment)
	ON_COMMAND(ID_BARYCENTERMOMENT, OnBarycentermoment)
	ON_COMMAND(ID_ANALYSIS_HOLENUM, OnAnalysisHolenum)
	ON_COMMAND(ID_FREQ_DCT, OnFreqDct)
	ON_COMMAND(ID_FREQ_HOTELLING, OnFreqHotelling)
	ON_COMMAND(ID_FREQ_WALSH, OnFreqWalsh)
	ON_COMMAND(ID_CODING_ARITH, OnCodingArith)
	ON_COMMAND(ID_CODING_BITPLANE, OnCodingBitplane)
	ON_COMMAND(ID_CODING_HUFFMAN, OnCodingHuffman)
	ON_COMMAND(ID_CODING_LOADIMG, OnCodingLoadimg)
	ON_COMMAND(ID_CODING_WRITEIMG, OnCodingWriteimg)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingView construction/destruction

CImageProcessingView::CImageProcessingView()
{
	// ΪС���任���õĲ���
	// ��ʱ���С���任ϵ���ڴ�
	m_pDbImage = NULL;	
	
	// ���õ�ǰ����
	m_nDWTCurDepth = 0;

	// ����С������֧������
	m_nSupp = 1;
}

CImageProcessingView::~CImageProcessingView()
{
	// �ͷ��ѷ����ڴ�
	if(m_pDbImage){
		delete[]m_pDbImage;
		m_pDbImage = NULL;
	}
}

BOOL CImageProcessingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingView drawing

void CImageProcessingView::OnDraw(CDC* pDC)
{
	CImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	CSize sizeDibDisplay;		
	

	
	if(!pDoc->m_pDibInit->IsEmpty()){	
		sizeDibDisplay = pDoc->m_pDibInit->GetDimensions();
		pDoc->m_pDibInit->Draw(pDC,CPoint(0,0),sizeDibDisplay);	
	}	

}

void CImageProcessingView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	CSize sizeTotal = pDoc->m_pDibInit->GetDimensions();
	SetScrollSizes(MM_TEXT, sizeTotal);

	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingView printing

BOOL CImageProcessingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CImageProcessingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CImageProcessingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingView diagnostics

#ifdef _DEBUG
void CImageProcessingView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImageProcessingView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CImageProcessingDoc* CImageProcessingView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageProcessingDoc)));
	return (CImageProcessingDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingView message handlers









/*************************************************************************
 *
 * \�������ƣ�
 *   OnFft2d()
 *
 * \�������:
 *   ��
 * 
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   ���ж�ά���ٸ���Ҷ�任
 *
 *************************************************************************
 */
void CImageProcessingView::OnFft2d() 
{
	//ͼ��FFT�任

	// ���Ĺ����״
	BeginWaitCursor();
	
	// ѭ�����Ʊ���
	int y;
	int x;
	
	// ���Doc���ָ��
	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();

	CDib * pDib = pDoc->m_pDibInit;

	// ���ͼ���ͷ�ļ���Ϣ
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ����ɢ����Ҷ�任��
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ����ɢ����Ҷ�任��", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
						
		// ����
		return;
	}

	// ͼ��Ŀ�
	CSize sizeImage ;
	int nWidth ;
	int nHeight;

	// ���ͼ��Ŀ�
	sizeImage = pDib->GetDimensions() ;
		
	nWidth = sizeImage.cx;
	nHeight= sizeImage.cy;

	// ��ʱ����
	double	dTmpOne;
	double  dTmpTwo;
	
	// ����Ҷ�任��ֱ�������
	int nTransHeight ;

	// ����Ҷ�任ˮƽ�������
	int nTransWidth  ;	
	
	// ������и���Ҷ�任�ĵ���	��2���������ݣ�
	dTmpOne = log(nWidth)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransWidth = (int) dTmpTwo	   ;
	
	// ������и���Ҷ�任�ĵ��� ��2���������ݣ�
	dTmpOne = log(nHeight)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransHeight = (int) dTmpTwo   ;

	// ����ͼ�����ݴ洢ÿ����Ҫ���ֽ���
	// BMP�ļ���ÿ�����ݴ洢��DWORD�����
	int		nSaveWidth;
	nSaveWidth = ( (nWidth << 3) + 31)/32 * 4 ;

	// ָ��ͼ�����ݵ�ָ��
	LPBYTE lpImage ;
	lpImage = pDib->m_lpImage ;

	// ͼ������ֵ
	unsigned char unchValue;

	
	// ָ��ʱ�����ݵ�ָ��
	complex<double> * pCTData ;

	// ָ��Ƶ�����ݵ�ָ��
	complex<double> * pCFData ;

	// �����ڴ�
	pCTData=new complex<double>[nTransWidth * nTransHeight];
	pCFData=new complex<double>[nTransWidth * nTransHeight];

	// ��ʼ��
	// ͼ�����ݵĿ�͸߲�һ����2���������ݣ�����pCTData
	// ��һ����������Ҫ��0
	for(y=0; y<nTransHeight; y++)
	{
		for(x=0; x<nTransWidth; x++)
		{
			pCTData[y*nTransWidth + x]=complex<double>(0,0);
		}
	}

	// ��ͼ�����ݴ���pCTData
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			unchValue = lpImage[y*nSaveWidth +x];
			pCTData[y*nTransWidth + x]=complex<double>(unchValue,0);
		}
	}

	// ����Ҷ���任
	DIBFFT_2D(pCTData, nWidth, nHeight, pCFData) ;
	
	// ��ʱ����
	double dTmp;

	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			dTmp = pCFData[y * nTransWidth + x].real() 
				   * pCFData[y * nTransWidth + x].real()
				 + pCFData[y * nTransWidth + x].imag() 
				   * pCFData[y * nTransWidth + x].imag();
			
			dTmp = sqrt(dTmp) ;

			// Ϊ����ʾ����Ҫ�Է��ȵĴ�С��������
			dTmp /= 100        ;

			// ����ͼ�����ݵĴ�С
			dTmp = min(dTmp, 255) ;

			lpImage[y*nSaveWidth +x] = (unsigned char)(int)dTmp;
		}
	}

	// Ϊ������Ļ����ʾ�����ǰѷ���ֵ��Ĳ����ú�ɫ��ʾ
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			lpImage[y*nSaveWidth +x] = 255 - lpImage[y*nSaveWidth +x];
		}
	}
	
	// ˢ����Ļ
	Invalidate();

	// �ͷ��ڴ�
	delete pCTData;
	delete pCFData;
	pCTData = NULL;
	pCFData = NULL;

	// ��������
	pDoc->SetModifiedFlag(TRUE);
		
	// ������ͼ
	pDoc->UpdateAllViews(NULL);

	//  �ָ������״
	EndWaitCursor();
}


/*************************************************************************
 *
 * \�������ƣ�
 *   OnDft2d()
 *
 * \�������:
 *   ��
 * 
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   ���ж�ά����Ҷ�任
 *
 *************************************************************************
 */
void CImageProcessingView::OnDft2d() 
{
	//ͼ����ɢ����Ҷ�任
	
	//��ʾ�û���ֱ�ӽ�����ɢ����Ҷ�任��ʱ��ܳ�
	MessageBox("û��ʹ��FFT��ʱ����ܺܳ���", "������ʾ" ,
			MB_ICONINFORMATION | MB_OK);
	
	//���Ĺ����״
	BeginWaitCursor(); 

	// ѭ�����Ʊ���
	int y;
	int x;
	
	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
	CDib * pDib = pDoc->m_pDibInit;

	// ���ͼ���ͷ�ļ���Ϣ
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ����ɢ����Ҷ�任��
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ����ɢ����Ҷ�任��", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
						
		// ����
		return;
	}

	//ͼ��ĳ����С
	CSize sizeImage		= pDib->GetDimensions();
	int nWidth			= sizeImage.cx		;
	int nHeight			= sizeImage.cy		;

	// ����ͼ�����ݴ洢ÿ����Ҫ���ֽ���
	// BMP�ļ���ÿ�����ݴ洢��DWORD�����
	int		nSaveWidth;
	nSaveWidth = ( (nWidth << 3) + 31)/32 * 4 ;

	// ָ��ͼ�����ݵ�ָ��
	LPBYTE lpImage ;
	lpImage = pDib->m_lpImage ;

	double * pTrRstRpart  = new double [nWidth*nHeight];
	double * pTrRstIpart  = new double [nWidth*nHeight];	
	
	::DIBDFT_2D(pDib, pTrRstRpart,pTrRstIpart);

	// ��ʱ����
	double dTmp;

	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			dTmp = pTrRstRpart[y*nWidth + x] * pTrRstRpart[y*nWidth + x]
				 + pTrRstIpart[y*nWidth + x] * pTrRstIpart[y*nWidth + x];
			
			dTmp = sqrt(dTmp) ;

			// Ϊ����ʾ����Ҫ�Է��ȵĴ�С��������
			dTmp /= 100        ;

			// ����ͼ�����ݵĴ�С
			dTmp = min(dTmp, 255) ;

			lpImage[y*nSaveWidth +x] = (unsigned char)(int)dTmp;
		}
	}

	// Ϊ������Ļ����ʾ�����ǰѷ���ֵ��Ĳ����ú�ɫ��ʾ
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			lpImage[y*nSaveWidth +x] = 255 - lpImage[y*nSaveWidth +x];
		}
	}

	// �ͷ��ڴ�
	delete pTrRstRpart;
	pTrRstRpart=NULL  ;
	
	delete pTrRstIpart;
	pTrRstIpart=NULL  ;

	// ��������
	pDoc->SetModifiedFlag(TRUE);
		
	// ������ͼ
	pDoc->UpdateAllViews(NULL);

	// �ָ������״
	EndWaitCursor(); 
	
	// ˢ����Ļ
	Invalidate();
}

void CImageProcessingView::OnFreqDct() 
{
	// ͼ�����ɢ���ұ任
		
	// ���Ĺ����״
	BeginWaitCursor();

	// ��ȡ�ĵ�
	CImageProcessingDoc* pDoc = GetDocument();

	//  ���ͼ��CDib���ָ��
	CDib * pDib = pDoc->m_pDibInit;

	// ���ͼ���ͷ�ļ���Ϣ
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ����ɢ���ұ任��
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ����ɢ���ұ任��", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
						
		// ����
		return;
	}
	
	::DIBDct(pDib);
	
	// ��������
	pDoc->SetModifiedFlag(TRUE);
		
	// ������ͼ
	pDoc->UpdateAllViews(NULL);

    // �ָ����
	EndWaitCursor();
	
}

void CImageProcessingView::OnFreqHotelling() 
{
	// ͼ������ֱ任
	
	// ���Ĺ����״
	BeginWaitCursor();

	// ��ȡ�ĵ�
	CImageProcessingDoc* pDoc = GetDocument();

	//  ���ͼ��CDib���ָ��
	CDib * pDib = pDoc->m_pDibInit;

	// ���ͼ���ͷ�ļ���Ϣ
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ����ɢ�����ֱ任��
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ����ɢ�����ֱ任��", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
						
		// ����
		return;
	}

	//  ͼ��Ļ����ֱ任
	DIBHOTELLING(pDib);
	
	// ��������
	pDoc->SetModifiedFlag(TRUE);
		
	// ������ͼ
	pDoc->UpdateAllViews(NULL);

    // �ָ����
	EndWaitCursor();
	
}

void CImageProcessingView::OnFreqWalsh() 
{
	// ͼ����ֶ�ʲ��������任
	
	// ���Ĺ����״
	BeginWaitCursor();

	// ��ȡ�ĵ�
	CImageProcessingDoc* pDoc = GetDocument();

	//  ���ͼ��CDib���ָ��
	CDib * pDib = pDoc->m_pDibInit;

	// ���ͼ���ͷ�ļ���Ϣ
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ���ֶ�ʲ��������任��
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ����ɢ�ֶ�ʲ�任��", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
						
		// ����
		return;
	}

	//  �����ֶ�ʲ��������任
	DIBWalsh(pDib);
	
	// ��������
	pDoc->SetModifiedFlag(TRUE);
		
	// ������ͼ
	pDoc->UpdateAllViews(NULL);

    // �ָ����
	EndWaitCursor();
	
}

/*************************************************************************
 *
 * \�������ƣ�
 *   OnViewHistogram()
 *
 * \�������:
 *   ��
 * 
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   �鿴ֱ��ͼ������ֱ��ͼ��ʾ����
 *
 *************************************************************************
 */
void CImageProcessingView::OnViewHistogram() 
{
	// ��ȡ�ĵ�
	CImageProcessingDoc* pDoc = GetDocument();

	// DIB����ɫ��Ŀ
	int nColorTableEntries;
	nColorTableEntries = pDoc->m_pDibInit->m_nColorTableEntries;
		
	// �ж��Ƿ���8bppλͼ������ֻ����8bppλͼ��
	if ( nColorTableEntries != 256)
	{
		// ��ʾ�û������ٽ��д���
		MessageBox("Ŀǰֻ֧�ֲ鿴256ɫλͼ�Ҷ�ֱ��ͼ��", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
		
		// ����
		return;
	}
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	// �����Ի���
	CDlgHistShow dlgHistShow;
	
	// ��ʼ������ֵ
	dlgHistShow.m_pDib = pDoc->m_pDibInit;
		
	// ��ʾ�Ի���
	if (dlgHistShow.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	// �ָ����
	EndWaitCursor();	
}

/*************************************************************************
 *
 * \�������ƣ�
 *   OnEnhanceSmooth()
 *
 * \�������:
 *   ��
 * 
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   ��ͼ�����ƽ������������ƽ��ģ�����öԻ���
 *
 *************************************************************************
 */
void CImageProcessingView::OnEnhanceSmooth() 
{
	// TODO: Add your command handler code here
		// ͼ��ƽ��
	
	// ��ȡ�ĵ�
	CImageProcessingDoc* pDoc = GetDocument();
	
	// ģ��߶�
	int		nTempHeight;
	
	// ģ����
	int		nTempWidth;
	
	// ģ��ϵ��
	double		dbTempCoef;
	
	// ģ������Ԫ��X����
	int		nTempCenX;
	
	// ģ������Ԫ��Y����
	int		nTempCenY;
	
	// ģ��Ԫ������
	double	pdbTemp[25];
	
	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��ƽ���������Ŀ������ƣ�
	if(pDoc->m_pDibInit->m_nColorTableEntries != 256)
	//if (::DIBNumColors(lpDIB) != 256)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ƽ����", "ϵͳ��ʾ" , 
			MB_ICONINFORMATION | MB_OK);
		
		// ����
		return;
	}
	
	// �����Ի���
	CDlgSmooth dlgSmth;
	
	// ��ģ�����鸳��ֵ��Ϊƽ��ģ�壩
	pdbTemp[0] = 1.0;
	pdbTemp[1] = 1.0;
	pdbTemp[2] = 1.0;
	pdbTemp[3] = 0.0;
	pdbTemp[4] = 0.0;
	pdbTemp[5] = 1.0;
	pdbTemp[6] = 1.0;
	pdbTemp[7] = 1.0;
	pdbTemp[8] = 0.0;
	pdbTemp[9] = 0.0;
	pdbTemp[10] = 1.0;
	pdbTemp[11] = 1.0;
	pdbTemp[12] = 1.0;
	pdbTemp[13] = 0.0;
	pdbTemp[14] = 0.0;
	pdbTemp[15] = 0.0;
	pdbTemp[16] = 0.0;
	pdbTemp[17] = 0.0;
	pdbTemp[18] = 0.0;
	pdbTemp[19] = 0.0;
	pdbTemp[20] = 0.0;
	pdbTemp[21] = 0.0;
	pdbTemp[22] = 0.0;
	pdbTemp[23] = 0.0;
	pdbTemp[24] = 0.0;
	
	// ��ʼ���Ի������ֵ
	dlgSmth.m_nTemType  = 0;
	dlgSmth.m_nSmthTemHeight  = 3;
	dlgSmth.m_nSmthTemWidth  = 3;
	dlgSmth.m_nSmthTemCenX = 1;
	dlgSmth.m_nSmthTemCenY = 1;
	dlgSmth.m_dbSmthTemCoef  = (double) (1.0 / 9.0);
	dlgSmth.m_pdbTemp = pdbTemp;
	
	// ��ʾ�Ի�����ʾ�û��趨ƽ����
	if (dlgSmth.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	// ��ȡ�û��趨��ƽ����
	nTempHeight   = dlgSmth.m_nSmthTemHeight;
	nTempWidth  = dlgSmth.m_nSmthTemWidth;
	nTempCenX = dlgSmth.m_nSmthTemCenX;
	nTempCenY = dlgSmth.m_nSmthTemCenY;
	dbTempCoef  = dlgSmth.m_dbSmthTemCoef;
	
	// ɾ���Ի���
	delete dlgSmth;	
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	// ����Template()����ƽ��DIB
	if (GeneralTemplate(pDoc->m_pDibInit, nTempWidth, nTempHeight, 
		nTempCenX, nTempCenY, pdbTemp, dbTempCoef))
	{		
		// ��������
		pDoc->SetModifiedFlag(TRUE);

		// ������ͼ
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// ��ʾ�û�
		MessageBox("�����ڴ�ʧ�ܣ�", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
	}
	
	// �ָ����
	EndWaitCursor();	
	
}

/*************************************************************************
 *
 * \�������ƣ�
 *   OnEnhanceMedian()
 *
 * \�������:
 *   ��
 * 
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   ��ͼ�������ֵ�˲���������ƽ��ģ�����öԻ���
 *
 *************************************************************************
 */
void CImageProcessingView::OnEnhanceMedian() 
{
	// ��ֵ�˲�
	
	// ��ȡ�ĵ�
	CImageProcessingDoc* pDoc = GetDocument();
	
	// �˲����ĸ߶�
	int nFilterHeight;
	
	// �˲����Ŀ��
	int nFilterWidth;
	
	// ����Ԫ�ص�X����
	int nFilterCenX;
	
	// ����Ԫ�ص�Y����
	int nFilterCenY;
	
	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��ƽ���������Ŀ������ƣ�
	if(pDoc->m_pDibInit->m_nColorTableEntries != 256)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ƽ����", "ϵͳ��ʾ" , 
			MB_ICONINFORMATION | MB_OK);
		
		// ����
		return;
	}
	
	
	// �����Ի���
	CDlgMedian dlgMedian;
	
	// ��ʼ������ֵ
	dlgMedian.m_nFilterType = 0;
	dlgMedian.m_nFilterHeight = 3;
	dlgMedian.m_nFilterWidth = 1;
	dlgMedian.m_nFilterCenX = 0;
	dlgMedian.m_nFilterCenY = 1;
	
	// ��ʾ�Ի�����ʾ�û��趨ƽ����
	if (dlgMedian.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	// ��ȡ�û����趨
	nFilterHeight = dlgMedian.m_nFilterHeight;
	nFilterWidth = dlgMedian.m_nFilterWidth;
	nFilterCenX = dlgMedian.m_nFilterCenX;
	nFilterCenY = dlgMedian.m_nFilterCenY;
	
	// ɾ���Ի���
	delete dlgMedian;	
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	// ����MedianFilter()������ֵ�˲�
	if (MedianFilter(pDoc->m_pDibInit, nFilterWidth,
		nFilterHeight, nFilterCenX, nFilterCenY ))
	
	{		
		// ��������
		pDoc->SetModifiedFlag(TRUE);

		// ������ͼ
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// ��ʾ�û�
		MessageBox("�����ڴ�ʧ�ܣ�", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
	}
	
	// �ָ����
	EndWaitCursor();

	// ��������
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
	
}


void CImageProcessingView::OnEnhancePseudcolor() 
{
	// α��ɫ����
	
	// ��ȡ�ĵ�
	CImageProcessingDoc* pDoc = GetDocument();
	
	// �����û�ѡ���α��ɫ���������
	int		nColor;
	
	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��ƽ���������Ŀ������ƣ�
	if(pDoc->m_pDibInit->m_nColorTableEntries != 256)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��α��ɫ�任��", "ϵͳ��ʾ" , 
			MB_ICONINFORMATION | MB_OK);

		// ����
		return;
	}
	
	// �����Ի���
	CDlgEnhColor dlgPara;
	
	// ��ʼ������ֵ
	if (pDoc->m_nColorIndex >= 0)
	{
		// ��ʼѡ�е�ǰ��α��ɫ
		dlgPara.m_nColor = pDoc->m_nColorIndex;
	}
	else
	{
		// ��ʼѡ�лҶ�α��ɫ�����
		dlgPara.m_nColor = 0;
	}
	
	// ָ�����������ָ��
	dlgPara.m_lpColorName = (LPSTR) ColorScaleName;
	
	// α��ɫ������Ŀ
	dlgPara.m_nColorCount = COLOR_SCALE_COUNT;
	
	// �����ַ�������
	dlgPara.m_nNameLen = sizeof(ColorScaleName) / COLOR_SCALE_COUNT;
	
	// ��ʾ�Ի�����ʾ�û��趨ƽ����
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	// ��ȡ�û����趨
	nColor = dlgPara.m_nColor;
	
	// ɾ���Ի���
	delete dlgPara;	
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	// �ж�α��ɫ�����Ƿ�Ķ�
	if (pDoc->m_nColorIndex != nColor)
	{
		// ����ReplaceColorPal()�����任��ɫ��
		ReplaceDIBColorTable(pDoc->m_pDibInit, (LPBYTE)ColorsTable[nColor]);
		
		// ���µ�ɫ��
		pDoc->m_pDibInit->MakePalette();
		
		// �������Ա����
		pDoc->m_nColorIndex = nColor;
		
		// ��������
		pDoc->SetModifiedFlag(TRUE);
				
		// ������ͼ
		pDoc->UpdateAllViews(NULL);
	}	
	

	// �ָ����
	EndWaitCursor();
}

void CImageProcessingView::OnTransDwt() 
{
	// ����ĵ���ָ��
	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();

	// ָ��ͼ���ָ��
	CDib * pDib = pDoc->m_pDibInit;

	// ���Ĺ����״
	BeginWaitCursor();

	// ����С���任
	int rsl = DIBDWTStep(pDib,0);

	// �ָ������״
	EndWaitCursor();

	// ���С���任���ɹ�����ֱ�ӷ���
	if (!rsl)			
		return;

	// �������־
	pDoc->SetModifiedFlag(TRUE);

	// ������ʾ
	pDoc->UpdateAllViews(FALSE);
}

void CImageProcessingView::OnTransIdwt() 
{
	// ����ĵ���ָ��	
	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();

	// ָ��ͼ���ָ��
	CDib * pDib = pDoc->m_pDibInit;

	// ���Ĺ����״
	BeginWaitCursor();

	// ����С�����任
	int rsl = DIBDWTStep(pDib,1);

	// �ָ������״
	EndWaitCursor();

	// ���С���任���ɹ�����ֱ�ӷ���
	if (!rsl)			
		return;
	pDoc->UpdateAllViews(FALSE);

	// ��������
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
}

BOOL CImageProcessingView::DIBDWTStep(CDib* pDib, int nInv)
{
	// ѭ������
	int i, j;

	// ��ȡͼ��ĳ��ȺͿ��
	int nWidth  = pDib->m_lpBMIH->biWidth;
	int nHeight = pDib->m_lpBMIH->biHeight;
		
	// ��ȡ�任��������
	int nMaxWLevel = Log2(nWidth);
	int nMaxHLevel = Log2(nHeight);
	int nMaxLevel;
	if (nWidth == 1<<nMaxWLevel && nHeight == 1<<nMaxHLevel)
		nMaxLevel = min(nMaxWLevel, nMaxHLevel);

	// ��ȡͼ��Ĵ洢�ߴ�
	CSize sizeImageSave = pDib->GetDibSaveDim();

	// ��ʱ����
	double	*pDbTemp;
	BYTE	*pBits;

	// ���С���任�Ĵ洢�ڴ滹û�з��䣬�������ڴ�
	if(!m_pDbImage){			
		m_pDbImage = new double[nWidth*nHeight];
		if (!m_pDbImage)	return FALSE;

		// ��ͼ�����ݷ���m_pDbImage�� 
		for (j=0; j<nHeight; j++)
		{
			pDbTemp = m_pDbImage + j*sizeImageSave.cx;
			pBits = pDib->m_lpImage + (nHeight-1-j)*sizeImageSave.cx;		
			for (i=0; i<nWidth; i++)
				pDbTemp[i] = pBits[i];
		}
	}
	
	// ����С���任���򷴱任��
	if (!DWTStep_2D(m_pDbImage, nMaxWLevel-m_nDWTCurDepth, nMaxHLevel-m_nDWTCurDepth,
						nMaxWLevel, nMaxHLevel, nInv, 1, m_nSupp))
		return FALSE;

	// ����Ƿ��任����ǰ������1
	if (nInv)
		m_nDWTCurDepth --;
	// �����1
	else
		m_nDWTCurDepth ++;

	// Ȼ�󣬽����ݿ�����ԭCDib�У���������Ӧ������ת��
	int lfw = nWidth>>m_nDWTCurDepth, lfh = nHeight>>m_nDWTCurDepth;
	for (j=0; j<nHeight; j++)
	{
		pDbTemp = m_pDbImage + j*sizeImageSave.cx;
		pBits = pDib->m_lpImage + (nHeight-1-j)*sizeImageSave.cx;
		for (i=0; i<nWidth; i++)
		{
			if (j<lfh && i<lfw)
				pBits[i] = FloatToByte(pDbTemp[i]);
			else
				pBits[i] = BYTE(FloatToChar(pDbTemp[i]) ^ 0x80);			
		}
	}

	// ����
	return TRUE;
}
void CImageProcessingView::OnEnhanceLintrans() 
{
	// ��ȡ�ĵ�
	CImageProcessingDoc* pDoc = GetDocument();

	// �����Ի���
	CDlgEhnLinTrans dlgPara;
	
	// ��1����
	int	nX1;
	int	nY1;
	
	// ��2����
	int	nX2;
	int	nY2;

	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ�ĻҶ����죬�����Ŀ������ƣ�
	if(pDoc->m_pDibInit->m_nColorTableEntries != 256)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��α��ɫ�任��", "ϵͳ��ʾ" , 
			MB_ICONINFORMATION | MB_OK);

		// ����
		return;
	}
	
	// ��ʼ������ֵ
	dlgPara.m_nX1 = 50;
	dlgPara.m_nY1 = 30;
	dlgPara.m_nX2 = 200;
	dlgPara.m_nY2 = 220;
	
	// ��ʾ�Ի�����ʾ�û��趨����λ��
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	// ��ȡ�û����趨
	nX1 = dlgPara.m_nX1;
	nY1 = dlgPara.m_nY1;
	nX2 = dlgPara.m_nX2;
	nY2 = dlgPara.m_nY2;
	
	// ɾ���Ի���
	delete dlgPara;	
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	// ����GrayStretch()�������лҶ�����
	GraySegLinTrans(pDoc->m_pDibInit, nX1, nY1, nX2, nY2);
	
	// ��������
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
	
	// �ָ����
	EndWaitCursor();
}

void CImageProcessingView::OnEnhanceHistequ() 
{
	// ֱ��ͼ����

	// ��ȡ�ĵ�
	CImageProcessingDoc* pDoc = GetDocument();
		
	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��ֱ��ͼ���⣬�����Ŀ������ƣ�
	if(pDoc->m_pDibInit->m_nColorTableEntries != 256)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��α��ɫ�任��", "ϵͳ��ʾ" , 
			MB_ICONINFORMATION | MB_OK);

		// ����
		return;
	}
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	// ����HistogramEqualize()��������ֱ��ͼ����
	HistogramEqualize(pDoc->m_pDibInit);
	
	// ��������
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
		
	// �ָ����
	EndWaitCursor();	
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
 *   �ú���ʵ��ͼ�����׼
 *
 *************************************************************************
 */
void CImageProcessingView::OnRegReg() 
{	
	// ����ĵ�����
	CImageProcessingDoc* pDoc;
	pDoc = GetDocument();

	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��ˮƽ���������Ŀ������ƣ�
	if(pDoc->m_pDibInit->m_nColorTableEntries != 256)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ����׼��", "ϵͳ��ʾ" , 
			MB_ICONINFORMATION | MB_OK);
		
		// ����
		return;
	}
	
	// ��ͼ����׼�Ի���
	CDlgReg* pDlg=new CDlgReg(NULL,pDoc);

	pDlg->DoModal();

	delete pDlg;
}

/*************************************************************************
 *
 * \�������ƣ�
 *   OnEnhanceSharp()
 *
 * \�������:
 *   ��
 * 
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   �ú�������Laplacian����ʵ��ͼ�����
 *
 *************************************************************************
 */
void CImageProcessingView::OnEnhanceSharp() 
{
	//���Ĺ����״
	BeginWaitCursor(); 
	
	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
	CDib * pDib = pDoc->m_pDibInit;
	
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
	
	// �ж��Ƿ���8-bppλͼ
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ��ָ", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
		
		// ����
		return;
	}
	
	// ѭ�����Ʊ���
	int y;
	int x;
	
	CSize sizeImage = pDib->GetDimensions();
	int nWidth = sizeImage.cx ;
	int nHeight= sizeImage.cy ;
	
	int nSaveWidth = pDib->GetDibSaveDim().cx;
	
	// �����ڴ棬�洢ͼ������,�����ݵĴ洢����DWORD�����
	unsigned char * pUnchImage = new unsigned char[nWidth*nHeight];
	
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			pUnchImage[y*nWidth+x] = pDib->m_lpImage[y*nSaveWidth+x];
		}
	}
	
	// ����LinearSharpen��������ͼ������ǿ
	LinearSharpen(pUnchImage, nWidth, nHeight) ;
	
	// ��ǿ�Ժ��ͼ�󿽱���pDib�У�������ʾ
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			pDib->m_lpImage[y*nSaveWidth+x] = pUnchImage[y*nWidth+x];
		}
	}
	
	// �ͷ��ڴ�
	delete []pUnchImage;
	pUnchImage = NULL  ;
	
	// �ָ������״
	EndWaitCursor(); 
	
	// ��������
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
	
}

/*************************************************************************
*
* \�������ƣ�
*   OnEnhanceSmoothFr()
*
* \�������:
*   ��
*
* \����ֵ:
*   ��
*
* \˵��:
*   �ú������õ�ͨ�˲�ʵ��ͼ��ƽ��
*
************************************************************************
*/
void CImageProcessingView::OnEnhanceSmoothFr() 
{
	// TODO: Add your command handler code here
	//���Ĺ����״
	BeginWaitCursor(); 
	
	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
	CDib * pDib = pDoc->m_pDibInit;
	
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
	
	// �ж��Ƿ���8-bppλͼ
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ��ָ", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
		
		// ����
		return;
	}

	// ѭ�����Ʊ���
	int y;
	int x;
	
	CSize sizeImage = pDib->GetDimensions();
	int nWidth = sizeImage.cx ;
	int nHeight= sizeImage.cy ;

	int nSaveWidth = pDib->GetDibSaveDim().cx;
	
	// �����ڴ棬�洢ͼ������,�����ݵĴ洢����DWORD�����
	unsigned char * pUnchImage = new unsigned char[nWidth*nHeight];
	
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			pUnchImage[y*nWidth+x] = pDib->m_lpImage[y*nSaveWidth+x];
		}
	}

	// ���õ�ͨ�˲���������ͼ����ǿ
	LowPassFilterEnhance(pUnchImage, nWidth, nHeight, nWidth/16) ;
	
	// ��ǿ�Ժ��ͼ�󿽱���pDib�У�������ʾ
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			pDib->m_lpImage[y*nSaveWidth+x] = pUnchImage[y*nWidth+x];
		}
	}
	
	// �ͷ��ڴ�
	delete []pUnchImage;
	pUnchImage = NULL  ;
	
	// �ָ������״
	EndWaitCursor(); 
	
	// ��������
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
}

/*************************************************************************
*
* \�������ƣ�
*   OnEnhanceButtLow()
*
* \�������:
*   ��
*
* \����ֵ:
*   ��
*
* \˵��:
*   �ú�������Butterworth��ͨ�˲�ʵ��ͼ��ƽ��
*
************************************************************************
*/
void CImageProcessingView::OnEnhanceButtLow() 
{
	// TODO: Add your command handler code here
	//���Ĺ����״
	BeginWaitCursor(); 
	
	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
	CDib * pDib = pDoc->m_pDibInit;
	
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
	
	// �ж��Ƿ���8-bppλͼ
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ��ָ", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
		
		// ����
		return;
	}
	
	// ѭ�����Ʊ���
	int y;
	int x;
	
	CSize sizeImage = pDib->GetDimensions();
	int nWidth = sizeImage.cx ;
	int nHeight= sizeImage.cy ;
	
	int nSaveWidth = pDib->GetDibSaveDim().cx;
	
	// �����ڴ棬�洢ͼ������,�����ݵĴ洢����DWORD�����
	unsigned char * pUnchImage = new unsigned char[nWidth*nHeight];
	
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			pUnchImage[y*nWidth+x] = pDib->m_lpImage[y*nSaveWidth+x];
		}
	}
	
	// ����ButterWorth��ͨ�˲���������ͼ����ǿ
	ButterWorthLowPass(pUnchImage, nWidth, nHeight, nWidth/2) ;
	
	// ��ǿ�Ժ��ͼ�󿽱���pDib�У�������ʾ
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			pDib->m_lpImage[y*nSaveWidth+x] = pUnchImage[y*nWidth+x];
		}
	}
	
	// �ͷ��ڴ�
	delete []pUnchImage;
	pUnchImage = NULL  ;
	
	// �ָ������״
	EndWaitCursor(); 
	
	// ��������
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
}

/*************************************************************************
*
* \�������ƣ�
*   OnEnhanceSharpFreq()
*
* \�������:
*   ��
*
* \����ֵ:
*   ��
*
* \˵��:
*   �ú������ø�ͨ�˲�ʵ��ͼ����ǿ
*
************************************************************************
*/
void CImageProcessingView::OnEnhanceSharpFreq() 
{
	// TODO: Add your command handler code here
	//���Ĺ����״
	BeginWaitCursor(); 
	
	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
	CDib * pDib = pDoc->m_pDibInit;
	
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
	
	// �ж��Ƿ���8-bppλͼ
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ��ָ", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
		
		// ����
		return;
	}
	
	// ѭ�����Ʊ���
	int y;
	int x;
	
	CSize sizeImage = pDib->GetDimensions();
	int nWidth = sizeImage.cx ;
	int nHeight= sizeImage.cy ;
	
	int nSaveWidth = pDib->GetDibSaveDim().cx;
	
	// �����ڴ棬�洢ͼ������,�����ݵĴ洢����DWORD�����
	unsigned char * pUnchImage = new unsigned char[nWidth*nHeight];
	
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			pUnchImage[y*nWidth+x] = pDib->m_lpImage[y*nSaveWidth+x];
		}
	}
	
	// ���ø�ͨ�˲���������ͼ����ǿ
	HighPassFilterEnhance(pUnchImage, nWidth, nHeight, 50) ;
	
	// ��ǿ�Ժ��ͼ�󿽱���pDib�У�������ʾ
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			pDib->m_lpImage[y*nSaveWidth+x] = pUnchImage[y*nWidth+x];
		}
	}
	
	// �ͷ��ڴ�
	delete []pUnchImage;
	pUnchImage = NULL  ;
	
	// �ָ������״
	EndWaitCursor(); 
	
	// ��������
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
}

/*************************************************************************
*
* \�������ƣ�
*   OnEnhanceButtHight()
*
* \�������:
*   ��
*
* \����ֵ:
*   ��
*
* \˵��:
*   �ú�������Butterworth��ͨ�˲�ʵ��ͼ��ƽ��
*
************************************************************************
*/
void CImageProcessingView::OnEnhanceButtHight() 
{	
	//���Ĺ����״
	BeginWaitCursor(); 
	
	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
	CDib * pDib = pDoc->m_pDibInit;
	
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
	
	// �ж��Ƿ���8-bppλͼ
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ��ָ", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
		
		// ����
		return;
	}
	
	// ѭ�����Ʊ���
	int y;
	int x;
	
	CSize sizeImage = pDib->GetDimensions();
	int nWidth = sizeImage.cx ;
	int nHeight= sizeImage.cy ;
	
	int nSaveWidth = pDib->GetDibSaveDim().cx;
	
	// �����ڴ棬�洢ͼ������,�����ݵĴ洢����DWORD�����
	unsigned char * pUnchImage = new unsigned char[nWidth*nHeight];
	
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			pUnchImage[y*nWidth+x] = pDib->m_lpImage[y*nSaveWidth+x];
		}
	}
	
	// ����ButterWorth��ͨ�˲���������ͼ����ǿ
	ButterWorthHighPass(pUnchImage, nWidth, nHeight, nWidth/2) ;
	
	// ��ǿ�Ժ��ͼ�󿽱���pDib�У�������ʾ
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			pDib->m_lpImage[y*nSaveWidth+x] = pUnchImage[y*nWidth+x];
		}
	}
	
	// �ͷ��ڴ�
	delete []pUnchImage;
	pUnchImage = NULL  ;
	
	// �ָ������״
	EndWaitCursor(); 
	
	// ��������
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
	
}

/*************************************************************************
*
* \�������ƣ�
*   OnRegionsegFix()
*
* \�������:
*   ��
*
* \����ֵ:
*   ��
*
* \˵��:
*   ʵ�ֲ�������ָ�
*
************************************************************************
*/
void CImageProcessingView::OnRegionsegFix() 
{
	// TODO: Add your command handler code here

	//���Ĺ����״
	BeginWaitCursor(); 

	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
	CDib * pDib = pDoc->m_pDibInit;

	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
	
	// �ж��Ƿ���8-bppλͼ
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ��ָ", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
					
		// ����
		return;
	}

	// ���ú���ʵ�̶ֹ���ֵ�ָ�
	RegionSegFixThreshold(pDib,200);

	// �ָ������״
	EndWaitCursor(); 
	
	// ��������
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
}

/*************************************************************************
*
* \�������ƣ�
*   OnAdaRegionSeg()
*
* \�������:
*   ��
*
* \����ֵ:
*   ��
*
* \˵��:
*   ʵ������Ӧ����ָ��㷨
*
************************************************************************
*/
void CImageProcessingView::OnAdaRegionSeg() 
{
	// TODO: Add your command handler code here
	
	//���Ĺ����״
	BeginWaitCursor(); 
	
	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
	CDib * pDib = pDoc->m_pDibInit;
	
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
	
	// �ж��Ƿ���8-bppλͼ
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ��ָ", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
		
		// ����
		return;
	}
	
	// ����Ӧ����ָ�
	RegionSegAdaptive(pDib);
	
	// �ָ������״
	EndWaitCursor(); 
	
		// ��������
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
	
}
/*************************************************************************
*
* \�������ƣ�
*   OnEdgeRoberts()
*
* \�������:
*   ��
*
* \����ֵ:
*   ��
*
* \˵��:
*   ʵ�ֲ��б߽�ָRoberts����
*
************************************************************************
*/
void CImageProcessingView::OnEdgeRoberts() 
{
	// TODO: Add your command handler code here

	//���Ĺ����״
	BeginWaitCursor(); 

	// ѭ�����Ʊ���
	int y;
	int x;
	
	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
	CDib * pDib = pDoc->m_pDibInit;

	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
	
	// �ж��Ƿ���8-bppλͼ
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ��ָ", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
		
		// ����
		return;
	}
	
	// ͼ��ĳ����С
	CSize sizeImage		= pDib->GetDimensions();
	int nWidth			= sizeImage.cx		;
	int nHeight			= sizeImage.cy		;
	
	// ָ���ݶ����ݵ�ָ��
	double * pdGrad;
	
	// ����ͼ��Ĵ�С�����ڴ�ռ䣬�洢�ݶȼ���Ľ��
	pdGrad=new double[nHeight*nWidth];
	
	//ͼ�����ݵ�ָ��
	LPBYTE  pImageData = pDib->m_lpImage;
	
	// ͼ���ڼ�����ڴ洢�е�ʵ�ʴ�С
	CSize sizeImageSave	= pDib->GetDibSaveDim();
	
	// ͼ�����ڴ���ÿһ������ռ�õ�ʵ�ʿռ�
	int nSaveWidth = sizeImageSave.cx;
	
	// Ӧ��Roberts�������ݶ�
	RobertsOperator(pDib, pdGrad);
	
	
	for(y=0; y<nHeight ; y++ )
		for(x=0 ; x<nWidth ; x++ )
		{
			if(*(pdGrad+y*nWidth+x)>50)
				*(	pImageData+y*nSaveWidth+x	)=0;
			else
				*(	pImageData+y*nSaveWidth+x	)=255;			
		}
		
  //�ͷ��ݶȽ��ʹ�õ��ڴ�ռ�
	delete pdGrad;
	pdGrad=NULL;

	// �ָ������״
	EndWaitCursor(); 
	
	// ��������
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
}


/*************************************************************************
*
* \�������ƣ�
*   OnEdgeSobel()
*
* \�������:
*   ��
*
* \����ֵ:
*   ��
*
* \˵��:
*   ʵ�ֲ��б߽�ָSobel����
*
************************************************************************
*/
void CImageProcessingView::OnEdgeSobel() 
{
	// TODO: Add your command handler code here
	
	//���Ĺ����״
	BeginWaitCursor(); 

	// ѭ�����Ʊ���
	int y;
	int x;
	
	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
	CDib * pDib = pDoc->m_pDibInit;

	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
	
	// �ж��Ƿ���8-bppλͼ
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ��ָ", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
		
		// ����
		return;
	}
	
	// ͼ��ĳ����С
	CSize sizeImage		= pDib->GetDimensions();
	int nWidth			= sizeImage.cx		;
	int nHeight			= sizeImage.cy		;
	
	// ָ���ݶ����ݵ�ָ��
	double * pdGrad;
	
	// ����ͼ��Ĵ�С�����ڴ�ռ䣬�洢�ݶȼ���Ľ��
	pdGrad=new double[nHeight*nWidth];
	
	//ͼ�����ݵ�ָ��
	LPBYTE  lpImage = pDib->m_lpImage;
	
	// ͼ���ڼ�����ڴ洢�е�ʵ�ʴ�С
	CSize sizeImageSave	= pDib->GetDibSaveDim();
	
	// ͼ�����ڴ���ÿһ������ռ�õ�ʵ�ʿռ�
	int nSaveWidth = sizeImageSave.cx;
	
	// Ӧ��Sobel�������ݶ�
	SobelOperator(pDib, pdGrad);
	
	
	for(y=0; y<nHeight ; y++ )
		for(x=0 ; x<nWidth ; x++ )
		{
			if(*(pdGrad+y*nWidth+x)>50)
				*(	lpImage+y*nSaveWidth+x	)=0;
			else
				*(	lpImage+y*nSaveWidth+x	)=255;			
		}
		
  //�ͷ��ڴ�ռ�
	delete []pdGrad;
	pdGrad=NULL;
	
	// �ָ������״
	EndWaitCursor(); 
	
	// ��������
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
}
/*************************************************************************
*
* \�������ƣ�
*   OnEdgePrewitt()
*
* \�������:
*   ��
*
* \����ֵ:
*   ��
*
* \˵��:
*   ʵ�ֲ��б߽�ָPrewitt����
*
************************************************************************
*/
void CImageProcessingView::OnEdgePrewitt() 
{
	// TODO: Add your command handler code here
	
	//���Ĺ����״
	BeginWaitCursor(); 

	// ѭ�����Ʊ���
	int y;
	int x;
	
	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
	CDib * pDib = pDoc->m_pDibInit;

	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
	
	// �ж��Ƿ���8-bppλͼ
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ��ָ", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
		
		// ����
		return;
	}
	
	// ͼ��ĳ����С
	CSize sizeImage		= pDib->GetDimensions();
	int nWidth			= sizeImage.cx		;
	int nHeight			= sizeImage.cy		;
	
	// ָ���ݶ����ݵ�ָ��
	double * pdGrad;
	
	// ����ͼ��Ĵ�С�����ڴ�ռ䣬�洢�ݶȼ���Ľ��
	pdGrad=new double[nHeight*nWidth];
	
	//ͼ�����ݵ�ָ��
	LPBYTE  lpImage = pDib->m_lpImage;
	
	// ͼ���ڼ�����ڴ洢�е�ʵ�ʴ�С
	CSize sizeImageSave	= pDib->GetDibSaveDim();
	
	// ͼ�����ڴ���ÿһ������ռ�õ�ʵ�ʿռ�
	int nSaveWidth = sizeImageSave.cx;
	
	// Ӧ��Prewitt�������ݶ�
	PrewittOperator(pDib, pdGrad);
	
	
	for(y=0; y<nHeight ; y++ )
		for(x=0 ; x<nWidth ; x++ )
		{
			if(*(pdGrad+y*nWidth+x)>50)
				*(	lpImage+y*nSaveWidth+x	)=0;
			else
				*(	lpImage+y*nSaveWidth+x	)=255;			
		}
	
  //�ͷ��ڴ�ռ�
	delete []pdGrad;
	pdGrad=NULL;
	
	// �ָ������״
	EndWaitCursor(); 
	
	// ��������
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
		
}

/*************************************************************************
*
* \�������ƣ�
*   OnEdgeLaplace()
*
* \�������:
*   ��
*
* \����ֵ:
*   ��
*
* \˵��:
*   ʵ�ֲ��б߽�ָ������˹����
*
************************************************************************
*/
void CImageProcessingView::OnEdgeLaplace() 
{
	// TODO: Add your command handler code here
	
	//���Ĺ����״
	BeginWaitCursor(); 
	
	// ѭ�����Ʊ���
	int y;
	int x;
	
	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
	CDib * pDib = pDoc->m_pDibInit;
	
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
	
	// �ж��Ƿ���8-bppλͼ
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ��ָ", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
		
		// ����
		return;
	}
	
	// ͼ��ĳ����С
	CSize sizeImage		= pDib->GetDimensions();
	int nWidth			= sizeImage.cx		;
	int nHeight			= sizeImage.cy		;
	
	// ָ���ݶ����ݵ�ָ��
	double * pdGrad;
	
	// ����ͼ��Ĵ�С�����ڴ�ռ䣬�洢�ݶȼ���Ľ��
	pdGrad=new double[nHeight*nWidth];
	
	//ͼ�����ݵ�ָ��
	LPBYTE  lpImage = pDib->m_lpImage;
	
	// ͼ���ڼ�����ڴ洢�е�ʵ�ʴ�С
	CSize sizeImageSave	= pDib->GetDibSaveDim();
	
	// ͼ�����ڴ���ÿһ������ռ�õ�ʵ�ʿռ�
	int nSaveWidth = sizeImageSave.cx;
	
	// Ӧ��Laplace�������ݶ�
	LaplacianOperator(pDib, pdGrad);
	
	
	for(y=0; y<nHeight ; y++ )
		for(x=0 ; x<nWidth ; x++ )
		{
			if(*(pdGrad+y*nWidth+x)>50)
				*(	lpImage+y*nSaveWidth+x	)=0;
			else
				*(	lpImage+y*nSaveWidth+x	)=255;			
		}
	
	//�ͷ��ڴ�ռ�
	delete []pdGrad;
	pdGrad=NULL;
	
	// �ָ������״
	EndWaitCursor(); 
	
	// ��������
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
	
}

/*************************************************************************
*
* \�������ƣ�
*   OnEdgeCanny()
*
* \�������:
*   ��
*
* \����ֵ:
*   ��
*
* \˵��:
*   ʵ�ֲ��б߽�ָCanny����
*
************************************************************************
*/
void CImageProcessingView::OnEdgeCanny() 
{
	// TODO: Add your command handler code here

	//���Ĺ����״
	BeginWaitCursor(); 

	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
	CDib * pDib = pDoc->m_pDibInit;
	
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
	
	// �ж��Ƿ���8-bppλͼ
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ��ָ", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
		
		// ����
		return;
	}

	// ѭ�����Ʊ���
	int y; 
	int x;
	
	CSize sizeImage = pDib->GetDimensions();
	int nWidth = sizeImage.cx ;
	int nHeight= sizeImage.cy ;

	int nSaveWidth = pDib->GetDibSaveDim().cx;

	// �����ڴ棬�洢ͼ������
	unsigned char * pUnchImage = new unsigned char[nWidth*nHeight];

	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			pUnchImage[y*nWidth+x] = pDib->m_lpImage[y*nSaveWidth+x];
		}
	}
	
	// canny���Ӽ����Ľ��
	unsigned char * pUnchEdge = new unsigned char[nWidth*nHeight];
	
	// ����canny�������б߽���ȡ
	Canny(pUnchImage, nWidth, nHeight, 0.4, 0.4, 0.79, pUnchEdge) ;
	
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			pDib->m_lpImage[y*nWidth+x]=(unsigned char)(255-pUnchEdge[y*nWidth+x]);
		}
	}

	delete []pUnchImage;
	pUnchImage = NULL  ;
	delete []pUnchEdge ;
	pUnchEdge = NULL   ;
	
	// �ָ������״
	EndWaitCursor(); 
	
	// ��������
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
	
}

/*************************************************************************
*
* \�������ƣ�
*   OnEdgeTrack()
*
* \�������:
*   ��
*
* \����ֵ:
*   ��
*
* \˵��:
*   ʵ�ֱ߽�����㷨
*
************************************************************************
*/
void CImageProcessingView::OnEdgeTrack() 
{
	// TODO: Add your command handler code here

	//���Ĺ����״
	BeginWaitCursor(); 

	// ���Doc���ָ
	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
	
	CDib * pDib = pDoc->m_pDibInit;

	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
	
	// �ж��Ƿ���8-bppλͼ
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ��ָ", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
		
		// ����
		return;
	}
	
	
	// ͼ���ڼ�����ڴ洢�е�ʵ�ʴ�С
	CSize sizeImageSave	= pDib->GetDibSaveDim();
	
	// ͼ�����ڴ���ÿһ������ռ�õ�ʵ�ʿռ�
	int nSaveWidth = sizeImageSave.cx;
	
	// ����ͼ���������
	int y;
	
	// ����ͼ��ĺ�����
	int x;
	
	// ͼ��ĳ����С
	CSize sizeImage		= pDib->GetDimensions();
	int nWidth			= sizeImage.cx		;
	int nHeight			= sizeImage.cy		;	
	
	
	// ָ��ͼ�����ݵ�ָ��
	LPBYTE lpImage ;
	lpImage = pDib->m_lpImage ;
	
	// �߽���ٺ�Ľ������
	unsigned char * pUnEdgeTrack ;
	
	pUnEdgeTrack = new unsigned char[nWidth * nHeight] ;
	
	EdgeTrack(pDib, pUnEdgeTrack);
	
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{	
			lpImage[y*nSaveWidth + x] = (unsigned char) (255-pUnEdgeTrack[y*nWidth + x]);
		}
	}
	
	//�ͷ��ڴ�
	delete pUnEdgeTrack;
	pUnEdgeTrack = NULL;
	
	// �ָ������״
	EndWaitCursor(); 
	
	// ��������
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
	
}

/*************************************************************************
*
* \�������ƣ�
*   OnRegionGrow()
*
* \�������:
*   ��
*
* \����ֵ:
*   ��
*
* \˵��:
*   ʵ�����������㷨
*
************************************************************************
*/
void CImageProcessingView::OnRegionGrow() 
{
	// TODO: Add your command handler code here

	//���Ĺ����״
	BeginWaitCursor(); 
	
	// ���Doc���ָ
	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
	
	CDib * pDib = pDoc->m_pDibInit;

	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
	
	// �ж��Ƿ���8-bppλͼ
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ��ָ", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
		
		// ����
		return;
	}
	
	// ����ͼ���������
	int y;
	
	// ����ͼ��ĺ�����
	int x;
	
	// ͼ���ڼ�����ڴ洢�е�ʵ�ʴ�С
	CSize sizeImageSave	= pDib->GetDibSaveDim();
	
	// ͼ�����ڴ���ÿһ������ռ�õ�ʵ�ʿռ�
	int nSaveWidth = sizeImageSave.cx;
	
	// ͼ��ĳ����С
	CSize sizeImage		= pDib->GetDimensions();
	int nWidth			= sizeImage.cx		;
	int nHeight			= sizeImage.cy		;	
	
	// ָ��ͼ�����ݵ�ָ��
	LPBYTE lpImage ;
	lpImage = pDib->m_lpImage ;
	
	unsigned char * pUnchRgRst = new unsigned char[nWidth * nHeight];
	// ��ʼ��
	memset(pUnchRgRst, 0 , sizeof(unsigned char)*nWidth*nHeight );
	
	RegionGrow(pDib, pUnchRgRst);
	
	for(y=0; y<nHeight; y++)
		for(x=0; x<nWidth; x++)
		{
			lpImage[y*nSaveWidth + x] =(unsigned char) (255-pUnchRgRst[y*nWidth+x] );
		}
		
	// �ͷ��ڴ�
	delete []pUnchRgRst;
	pUnchRgRst = NULL  ;
	
	// �ָ������״
	EndWaitCursor(); 
	
	// ��������
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
}

/*************************************************************************
 *
 * \�������ƣ�
 *   OnMotionBackground()
 *
 * \�������:
 *   ��
 * 
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   �ú��������˶�ͼ����ȡ���еľ�ֹ�����������˶�ͼ��Ҫ��Ϊbmp�ļ�������
 *˳�����С�
 *
 *************************************************************************
 */
void CImageProcessingView::OnMotionBackground() 
{
	// ��ȡ�����ɹ���־
	BOOL bFlag;
	
	// ��ȡ�ĵ�ָ��
	CImageProcessingDoc* pDoc;
	pDoc = GetDocument();

	// ��õ�ǰ���ļ����ļ�·����
	CString strPathName;
	strPathName = pDoc->GetPathName();

	// ���е���֡��
	int nTotalFrameNum = 20;

	// ͼ��Ŀ��
	int nImageWidth;
	nImageWidth = pDoc->m_pDibInit->m_lpBMIH->biWidth;	

	// ͼ��ĸ߶�
	int nImageHeight;
	nImageHeight = pDoc->m_pDibInit->m_lpBMIH->biHeight;

	// ͼ��ľ�ֹ����	
	unsigned char* pUnchBackGround;
	pUnchBackGround = new unsigned char[nImageWidth*nImageHeight];
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	// ����GetBackground������ȡ��ֹ����
	bFlag = GetBackground(strPathName, nTotalFrameNum, nImageWidth,nImageHeight, pUnchBackGround);
	if(bFlag == FALSE){
		return;
	}

	// ����������Ϊ��ǰ��ʾͼ��
	LPBYTE lpTemp;
	lpTemp = pDoc->m_pDibInit->m_lpImage;

	// �����ݿ�����ͼ����
	memcpy(lpTemp, (LPBYTE)pUnchBackGround, nImageWidth*nImageHeight);
	
	// �ָ������״
	EndWaitCursor(); 

	// �ͷ��ѷ����ڴ�
	delete[]pUnchBackGround;
	pUnchBackGround = NULL;
	
	// ��������
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
}

/*************************************************************************
 *
 * \�������ƣ�
 *   OnRecogMatch()
 *
 * \�������:
 *   ��
 * 
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   ����ͼ��ģ�壬�ڴ�ƥ���ͼ�����ҵ�ƥ���λ��
 *
 *************************************************************************
 */
void CImageProcessingView::OnRecogMatch() 
{
	CImageProcessingDoc* pDoc;
	pDoc = GetDocument();

	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��ˮƽ���������Ŀ������ƣ�
	if(pDoc->m_pDibInit->m_nColorTableEntries != 256)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ����׼��", "ϵͳ��ʾ" , 
			MB_ICONINFORMATION | MB_OK);
		
		// ����
		return;
	}
	
	// ��ͼ��ʶ��Ի���
	CDlgRecMatch* pDlg = new CDlgRecMatch(NULL, pDoc);
	pDlg->DoModal();
	
	delete pDlg;
}

void CImageProcessingView::OnDegenerationInverse() 
{
	// ͼ���ģ��
		
	// ���Ĺ����״
	BeginWaitCursor();

	// ��ȡ�ĵ�
	CImageProcessingDoc* pDoc = GetDocument();

	//  ���ͼ��CDib���ָ��
	CDib * pDib = pDoc->m_pDibInit;

	// ���ͼ���ͷ�ļ���Ϣ
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��ͼ��ģ����
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ��ģ����", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
						
		// ����
		return;
	}
	
	::DIBNoRestriction(pDib);

	// ��������
	pDoc->SetModifiedFlag(TRUE);
		
	// ������ͼ
	pDoc->UpdateAllViews(NULL);

    // �ָ����
	EndWaitCursor();
	
}

void CImageProcessingView::OnRestoreInverse() 
{
	// ͼ������˲�
		
	// ���Ĺ����״
	BeginWaitCursor();

	// ��ȡ�ĵ�
	CImageProcessingDoc* pDoc = GetDocument();

	//  ���ͼ��CDib���ָ��
	CDib * pDib = pDoc->m_pDibInit;

	// ���ͼ���ͷ�ļ���Ϣ
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ�����˲���
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ�����˲���", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
						
		// ����
		return;
	}
	
	::DIBInverseFilter(pDib);

	// ��������
	pDoc->SetModifiedFlag(TRUE);
		
	// ������ͼ
	pDoc->UpdateAllViews(NULL);

    // �ָ����
	EndWaitCursor();
	
}

void CImageProcessingView::OnDegenerationMotion() 
{
	// ͼ����˶�ģ��
		
	// ���Ĺ����״
	BeginWaitCursor();

	// ��ȡ�ĵ�
	CImageProcessingDoc* pDoc = GetDocument();

	//  ���ͼ��CDib���ָ��
	CDib * pDib = pDoc->m_pDibInit;

	// ���ͼ���ͷ�ļ���Ϣ
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ���˶�ģ����
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ���˶�ģ����", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
						
		// ����
		return;
	}
	
	::DIBMotionDegeneration(pDib);

	// ��������
	pDoc->SetModifiedFlag(TRUE);
		
	// ������ͼ
	pDoc->UpdateAllViews(NULL);

    // �ָ����
	EndWaitCursor();
	
}

void CImageProcessingView::OnRestoreMotion() 
{
	// �˶�ģ��ͼ��ԭ
		
	// ���Ĺ����״
	BeginWaitCursor();

	// ��ȡ�ĵ�
	CImageProcessingDoc* pDoc = GetDocument();

	//  ���ͼ��CDib���ָ��
	CDib * pDib = pDoc->m_pDibInit;

	// ���ͼ���ͷ�ļ���Ϣ
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ���˶�ģ����ԭ��
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ���˶�ģ����ԭ��", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
						
		// ����
		return;
	}
	
	::DIBMotionRestore(pDib);

	// ��������
	pDoc->SetModifiedFlag(TRUE);
		
	// ������ͼ
	pDoc->UpdateAllViews(NULL);

    // �ָ����
	EndWaitCursor();
	
}

void CImageProcessingView::OnDEGENERATIONWinner() 
{	
	// ͼ��ļ���ģ��
		
	// ���Ĺ����״
	BeginWaitCursor();

	// ��ȡ�ĵ�
	CImageProcessingDoc* pDoc = GetDocument();

	//  ���ͼ��CDib���ָ��
	CDib * pDib = pDoc->m_pDibInit;

	// ���ͼ���ͷ�ļ���Ϣ
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ�ļ���ģ����
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ�ļ���ģ����", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
						
		// ����
		return;
	}
	
	::DIBNoiseDegeneration(pDib);

	// ��������
	pDoc->SetModifiedFlag(TRUE);
		
	// ������ͼ
	pDoc->UpdateAllViews(NULL);

    // �ָ����
	EndWaitCursor();
	
}


void CImageProcessingView::OnRestoreWinner() 
{
	// ͼ���ά���˲�
		
	// ���Ĺ����״
	BeginWaitCursor();

	// ��ȡ�ĵ�
	CImageProcessingDoc* pDoc = GetDocument();

	//  ���ͼ��CDib���ָ��
	CDib * pDib = pDoc->m_pDibInit;

	// ���ͼ���ͷ�ļ���Ϣ
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��ά���˲���
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ά���˲���", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
						
		// ����
		return;
	}
	
	::DIBWinnerFilter(pDib);

	// ��������
	pDoc->SetModifiedFlag(TRUE);
		
	// ������ͼ
	pDoc->UpdateAllViews(NULL);

    // �ָ����
	EndWaitCursor();
	
}

void CImageProcessingView::OnMoment() 
{
	// ͼ���pq������

	// ���Ĺ����״
	BeginWaitCursor();

	// ��ȡ�ĵ�
	CImageProcessingDoc* pDoc = GetDocument();

	//  ���ͼ��CDib���ָ��
	CDib * pDib = pDoc->m_pDibInit;

	// ���ͼ���ͷ�ļ���Ϣ
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// �ж��Ƿ���8-bppλͼ
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
						
		// ����
		return;
	}
	
	::DIBMOMENT(pDib);
	
    // �ָ����
	EndWaitCursor();
	
}

void CImageProcessingView::OnBarycentermoment() 
{
	// ͼ������ľ�

	// ���Ĺ����״
	BeginWaitCursor();

	// ��ȡ�ĵ�
	CImageProcessingDoc* pDoc = GetDocument();

	//  ���ͼ��CDib���ָ��
	CDib * pDib = pDoc->m_pDibInit;

	// ���ͼ���ͷ�ļ���Ϣ
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// �ж��Ƿ���8-bppλͼ
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
						
		// ����
		return;
	}
	
	::DIBBARYCENTERMOMENT(pDib);

	// �ָ����
	EndWaitCursor();
	
}

void CImageProcessingView::OnAnalysisHolenum() 
{
	// ��ȥ��ֵͼ����С����ֵ���������
		
	// ���Ĺ����״
	BeginWaitCursor();

	// ��ȡ�ĵ�
	CImageProcessingDoc* pDoc = GetDocument();

	//  ���ͼ��CDib���ָ��
	CDib * pDib = pDoc->m_pDibInit;

	// ���ͼ���ͷ�ļ���Ϣ
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// �ж��Ƿ���8-bppλͼ
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ��", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
						
		// ����
		return;
	}
	
	::DIBHOLENUMBER(pDib);

	// ��������
	pDoc->SetModifiedFlag(TRUE);
		
	// ������ͼ
	pDoc->UpdateAllViews(NULL);

    // �ָ����
	EndWaitCursor();
	
}

void CImageProcessingView::OnStreetFramework() 
{
	// ��������Ǽ���ȡ
		
	// ���Ĺ����״
	BeginWaitCursor();

	// ��ȡ�ĵ�
	CImageProcessingDoc* pDoc = GetDocument();

	//  ���ͼ��CDib���ָ��
	CDib * pDib = pDoc->m_pDibInit;

	// ���ͼ���ͷ�ļ���Ϣ
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// �ж��Ƿ���8-bppλͼ
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
						
		// ����
		return;
	}
	
	::DIBFREAMEWORK(pDib);

	// ��������
	pDoc->SetModifiedFlag(TRUE);
		
	// ������ͼ
	pDoc->UpdateAllViews(NULL);

    // �ָ����
	EndWaitCursor();
	
}

void CImageProcessingView::OnStreetTransform() 
{
	// ��ֵͼ��Ľ�������任
		
	// ���Ĺ����״
	BeginWaitCursor();

	// ��ȡ�ĵ�
	CImageProcessingDoc* pDoc = GetDocument();

	//  ���ͼ��CDib���ָ��
	CDib * pDib = pDoc->m_pDibInit;

	// ���ͼ���ͷ�ļ���Ϣ
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// �ж��Ƿ���8-bppλͼ��
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
						
		// ����
		return;
	}
	
	::DIBSTREETDIS(pDib);

	// ��������
	pDoc->SetModifiedFlag(TRUE);
		
	// ������ͼ
	pDoc->UpdateAllViews(NULL);

    // �ָ����
	EndWaitCursor();
	
}

void CImageProcessingView::OnFrameRestore() 
{
	// ��������Ǽܸ�ԭ
		
	// ���Ĺ����״
	BeginWaitCursor();

	// ��ȡ�ĵ�
	CImageProcessingDoc* pDoc = GetDocument();

	//  ���ͼ��CDib���ָ��
	CDib * pDib = pDoc->m_pDibInit;

	// ���ͼ���ͷ�ļ���Ϣ
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// �ж��Ƿ���8-bppλͼ
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
						
		// ����
		return;
	}
	
	::DIBCHESSBOARDDISRESTORE(pDib);

	// ��������
	pDoc->SetModifiedFlag(TRUE);
		
	// ������ͼ
	pDoc->UpdateAllViews(NULL);

    // �ָ����
	EndWaitCursor();
	
}


void CImageProcessingView::OnTrace() 
{
	// ��ֵͼ��߽����

	// ���Ĺ����״
	BeginWaitCursor();

	// ��ȡ�ĵ�
	CImageProcessingDoc* pDoc = GetDocument();

	//  ���ͼ��CDib���ָ��
	CDib * pDib = pDoc->m_pDibInit;

	// ���ͼ���ͷ�ļ���Ϣ
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// �ж��Ƿ���8-bppλͼ
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
						
		// ����
		return;
	}
	
	::DIBTrace(pDib);

	// ��������
	pDoc->SetModifiedFlag(TRUE);
		
	// ������ͼ
	pDoc->UpdateAllViews(NULL);

    // �ָ����
	EndWaitCursor();
	
	
}

void CImageProcessingView::OnOutline() 
{
	// ��ֵͼ��߽���ȡ

	// ���Ĺ����״
	BeginWaitCursor();

	// ��ȡ�ĵ�
	CImageProcessingDoc* pDoc = GetDocument();

	//  ���ͼ��CDib���ָ��
	CDib * pDib = pDoc->m_pDibInit;

	// ���ͼ���ͷ�ļ���Ϣ
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// �ж��Ƿ���8-bppλͼ
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
						
		// ����
		return;
	}
	
	::DIBOUTLINE(pDib);

	// ��������
	pDoc->SetModifiedFlag(TRUE);
		
	// ������ͼ
	pDoc->UpdateAllViews(NULL);

    // �ָ����
	EndWaitCursor();
	
}

void CImageProcessingView::OnViewBayer() 
{
	// Bayer��������ʾͼ��
		
	// ���Ĺ����״
	BeginWaitCursor();

	// ��ȡ�ĵ�
	CImageProcessingDoc* pDoc = GetDocument();

	//  ���ͼ��CDib���ָ��
	CDib * pDib = pDoc->m_pDibInit;

	// ���ͼ���ͷ�ļ���Ϣ
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// �ж��Ƿ���8-bppλͼ
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
						
		// ����
		return;
	}
	
	::LimbPatternBayer(pDib);

	// ��������
	pDoc->SetModifiedFlag(TRUE);
		
	// ������ͼ
	pDoc->UpdateAllViews(NULL);

    // �ָ����
	EndWaitCursor();
	
}

void CImageProcessingView::OnVIEWFloydSteinberg() 
{
	// Floyd-Steinberg��������ʾͼ��
		
	// ���Ĺ����״
	BeginWaitCursor();

	// ��ȡ�ĵ�
	CImageProcessingDoc* pDoc = GetDocument();

	//  ���ͼ��CDib���ָ��
	CDib * pDib = pDoc->m_pDibInit;

	// ���ͼ���ͷ�ļ���Ϣ
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// �ж��Ƿ���8-bppλͼ
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
						
		// ����
		return;
	}
	
	::DitherFloydSteinberg(pDib);

	// ��������
	pDoc->SetModifiedFlag(TRUE);
		
	// ������ͼ
	pDoc->UpdateAllViews(NULL);

    // �ָ����
	EndWaitCursor();
	
}

void CImageProcessingView::OnCodingHuffman() 
{
	// �����������
	
	// ��ȡ�ĵ�
	CImageProcessingDoc * pDoc = GetDocument();
	
	// ָ��Դͼ�����ص�ָ��
	unsigned char *	lpSrc;
		
	// ͼ��ĸ߶ȺͿ��
	LONG	lHeight;
	LONG	lWidth;
	
	// ͼ��ÿ�е��ֽ���
	LONG	lLineBytes;
	
	// ͼ����������
	LONG	lCountSum;
	
	// ѭ������
	LONG	i;
	LONG	j;
	
	// ����ָ��������������Ҷ�ֵ���ָ���
	double * dProba;
	
	// ��ǰͼ����ɫ��Ŀ
	int		nColorNum;

	//  ���ͼ��CDib���ָ��
	CDib * pDib = pDoc->m_pDibInit;
			
	//ͼ�����ݵ�ָ��
	LPBYTE  lpDIBBits = pDib->m_lpImage;

	// ͷ�ļ���Ϣ
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// �ж��Ƿ���8-bppλͼ��ֻ����8-bppλͼ�Ļ��������룩
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ�Ļ��������룡", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
						
		// ����
		return;
	}
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	/********************************************************************
	   ��ʼ��������Ҷȼ����ֵĸ���	

	   �����Ҫ��ָ�������н��й���������,
	   ֻҪ����һ���ĳɸ������Ҷȼ����ʸ�ֵ����
	**********************************************************************
	*/
	
	//  ��ͷ�ļ���Ϣ�õ�ͼ��ı��������Ӷ��õ���ɫ��Ϣ
	nColorNum = (int)pow(2,lpBMIH->biBitCount);

	// �����ڴ�
	dProba = new double[nColorNum];
	
	//�õ�ͼ��Ŀ�Ⱥ͸߶�
	CSize   SizeDim;
	SizeDim = pDib->GetDimensions();
	lWidth = SizeDim.cx;
	lHeight = SizeDim.cy;

	// ����ͼ����������
	lCountSum = lHeight * lWidth;
	
	//�õ�ʵ�ʵ�Dibͼ��洢��С
	CSize   SizeRealDim;
	SizeRealDim = pDib->GetDibSaveDim();

	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = SizeRealDim.cx;
	
	// ����ֵ
	for (i = 0; i < nColorNum; i ++)
	{
		dProba[i] = 0.0;
	}
	
	// ��������Ҷ�ֵ�ļ���
	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lWidth; j ++)
		{
			// ָ��ͼ��ָ��
			lpSrc = lpDIBBits + lLineBytes * i + j;
			
			// ������1
			dProba[*(lpSrc)] = dProba[*(lpSrc)] + 1;
		}
	}
		
	
	// ��������Ҷ�ֵ���ֵĸ���
	for (i = 0; i < nColorNum; i ++)
	{
		dProba[i] = dProba[i] / (FLOAT)lCountSum;
	}
	
	/***************************************************
	 ������������������
	 ���öԻ�����ʾ���������
	****************************************************/
	
	// �����Ի���
	CDlgHuffman dlgCoding;
	
	// ��ʼ������ֵ
	dlgCoding.dProba = dProba;
	dlgCoding.nColorNum = nColorNum;
	
	// ��ʾ�Ի���
	dlgCoding.DoModal();
		
	// �ָ����
	EndWaitCursor();
	
}

void CImageProcessingView::OnCodingShanfino() 
{
	// ��ũ����ŵ�����
	
	// ��ȡ�ĵ�
	CImageProcessingDoc * pDoc = GetDocument();
	
	// ָ��Դͼ�����ص�ָ��
	unsigned char *	lpSrc;
		
	// ͼ��ĸ߶�
	LONG	lHeight;
	LONG	lWidth;
	
	// ͼ��ÿ�е��ֽ���
	LONG	lLineBytes;
	
	// ��ȡ��ǰDIB��ɫ��Ŀ
	int		nColorNum;
	
	// ͼ����������
	LONG	lCountSum;
	
	// ѭ������
	LONG	i;
	LONG	j;
	
	// ��������Ҷ�ֵ���ָ��ʵ�����ָ��
	double * dProba;	

	//  ���ͼ��CDib���ָ��
	CDib * pDib = pDoc->m_pDibInit;
			
	//ͼ�����ݵ�ָ��
	LPBYTE  lpDIBBits = pDib->m_lpImage;

	// ͷ�ļ���Ϣ
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ����ũ����ŵ���룩
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ����ũ����ŵ���룡", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
						
		// ����
		return;
	}
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	/******************************************************************************
	// ��ʼ��������Ҷȼ����ֵĸ���	
	//
	// �����Ҫ��ָ�������н�����ũ����ŵ����,
	//ֻҪ����һ���ĳɸ������Ҷȼ����ʸ�ֵ����
	*****************************************************************************
	*/
	//  �Ҷ�ֵ�����ļ���
	nColorNum = (int)pow(2,lpBMIH->biBitCount);

	// �����ڴ�
	dProba = new double[nColorNum];
	
	//�õ�ͼ��Ŀ�Ⱥ͸߶�
	CSize   SizeDim;
	SizeDim = pDib->GetDimensions();
	lWidth = SizeDim.cx;
	lHeight = SizeDim.cy;

	// ����ͼ����������
	lCountSum = lHeight * lWidth;
	
	//�õ�ʵ�ʵ�Dibͼ��洢��С
	CSize   SizeRealDim;
	SizeRealDim = pDib->GetDibSaveDim();

	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = SizeRealDim.cx;

	// ����ͼ����������
	lCountSum = lHeight * lWidth;
	
	// ���ü���Ϊ0
	for (i = 0; i < nColorNum; i ++)
	{
		dProba[i] = 0.0;
	}
	
	// ��������Ҷ�ֵ�ļ��������ڷ�256ɫλͼ���˴�������dProba��ֵ��������ͬ��
	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lWidth; j ++)
		{
			// ָ��ͼ��ָ��
			lpSrc = lpDIBBits + lLineBytes * i + j;
			
			// ������1
			dProba[*(lpSrc)] = dProba[*(lpSrc)]+ 1;
		}
	}
	
	
	// ��������Ҷ�ֵ���ֵĸ���
	for (i = 0; i < nColorNum; i ++)
	{
		dProba[i] /= (double)lCountSum;
	}
	
	/***************************************************
	 ������ũ����ŵ��������
	 ���öԻ�����ʾ��ũ����ŵ���
	****************************************************/	
	
	// �����Ի���
	CDlgShannon dlgPara;
	
	// ��ʼ������ֵ
	dlgPara.m_dProba = dProba;
	dlgPara.m_nColorNum = nColorNum;
	
	// ��ʾ�Ի���
	dlgPara.DoModal();

	//�ͷ��ڴ�
	delete dProba;
		
	// �ָ����
	EndWaitCursor();
}

void CImageProcessingView::OnCodingArith() 
{
	CDlgArith dlgCoding;
	
	// ��ʾ�Ի���
	dlgCoding.DoModal();
	
}

void CImageProcessingView::OnCodingBitplane() 
{
	// �����Ի���
	CDlgBitPlane dlgCoding;	
	
	// ��ʾ�Ի���
	dlgCoding.DoModal();	
	
	BYTE bBitNum = dlgCoding.m_BItNumber;

	// ��ȡ�ĵ�
	CImageProcessingDoc * pDoc = GetDocument();
		
	//  ���ͼ��CDib���ָ��
	CDib * pDib = pDoc->m_pDibInit;
	
	// ͷ�ļ���Ϣ
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��λƽ��ֽ⣩
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��λƽ��ֽ⣡", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
						
		// ����
		return;
	}
	DIBBITPLANE(pDib,bBitNum);

	// ��������
	pDoc->SetModifiedFlag(TRUE);
		
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
	
}

void CImageProcessingView::OnCodingWriteimg() 
{
	// �Ե�ǰͼ�����DPCM���루��ΪIMG��ʽ�ļ���
	
	// ��ȡ�ĵ�
	CImageProcessingDoc * pDoc = GetDocument();
		
	//  ���ͼ��CDib���ָ��
	CDib * pDib = pDoc->m_pDibInit;
			
	//ͼ�����ݵ�ָ��
	LPBYTE  lpDIBBits = pDib->m_lpImage;

	// ͷ�ļ���Ϣ
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// �ж��Ƿ���8-bppλͼ������8-bppλͼ��DPCM���룩
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��DPCM���룡", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
						
		// ����
		return;
	}
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	// �ļ�����·��
	CString strFilePath;
	
	// ��ȡԭʼ�ļ���
	strFilePath = pDoc->GetPathName();
	
	// ���ĺ�׺ΪIMG
	if (strFilePath.Right(4).CompareNoCase(".BMP") == 0)
	{	
		strFilePath = strFilePath.Left(strFilePath.GetLength()-3) + "IMG";
	}
	else
	{
		strFilePath += ".IMG";
	}

	// ����SaveAs�Ի���
	CFileDialog dlg(FALSE, "IMG", strFilePath, 
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		            "IMGͼ���ļ� (*.IMG) | *.IMG|�����ļ� (*.*) | *.*||", NULL);
	
	// ��ʾ�û�ѡ�񱣴��·��
	if (dlg.DoModal() != IDOK)
	{
		// �ָ����
		EndWaitCursor();
	
		return;
	}
	
	// ��ȡ�û�ָ�����ļ�·��
	strFilePath = dlg.GetPathName();
	
	// CFile��CFileException����
	CFile file;
	CFileException fe;
	
	// ���Դ���ָ����IMG�ļ�
	if (!file.Open(strFilePath, CFile::modeCreate |
	  CFile::modeReadWrite | CFile::shareExclusive, &fe))
	{
		MessageBox("��ָ��IMG�ļ�ʱʧ�ܣ�", "ϵͳ��ʾ" , 
			MB_ICONINFORMATION | MB_OK);
	
		return;
	}
	
	// ����WRITE2IMG()��������ǰ��DIB����ΪIMG�ļ�
	if (::WRITE2IMG(pDib, file))
	{
		MessageBox("�ɹ�����ΪIMG�ļ���", "ϵͳ��ʾ" , 
			MB_ICONINFORMATION | MB_OK);
	}
	else
	{
		MessageBox("����ΪIMG�ļ�ʧ�ܣ�", "ϵͳ��ʾ" , 
			MB_ICONINFORMATION | MB_OK);
	}	
		
	// �ָ����
	EndWaitCursor();
	
}


void CImageProcessingView::OnCodingLoadimg() 
{
	// ����IMG�ļ�
	
	// ��ȡ�ĵ�
	CImageProcessingDoc * pDoc = GetDocument();
		
	//  ���ͼ��CDib���ָ��
	CDib * pDib = pDoc->m_pDibInit;

	// �ļ�·��
	CString strFilePath;
	
	// ����Open�Ի���
	CFileDialog dlg(TRUE, "PCX", NULL,
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
					"IMGͼ���ļ� (*.PCX) | *.IMG|�����ļ� (*.*) | *.*||", NULL);
	
	// ��ʾ�û�ѡ�񱣴��·��
	if (dlg.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	// ��ȡ�û�ָ�����ļ�·��
	strFilePath = dlg.GetPathName();
	
	// CFile��CFileException����
	CFile file;
	CFileException fe;
	
	// ���Դ�ָ����PCX�ļ�
	if (!file.Open(strFilePath, CFile::modeRead | CFile::shareDenyWrite, &fe))
	{
		// ��ʾ�û�
		MessageBox("��ָ��PCX�ļ�ʱʧ�ܣ�", "ϵͳ��ʾ" , 
			MB_ICONINFORMATION | MB_OK);
		
		// ����
		return;
	}
	
	// ���Ĺ����״
	BeginWaitCursor();

	// ����LOADIMG()������ȡָ����IMG�ļ�
	BOOL Succ = LOADIMG(pDib, file);

	if (Succ == TRUE)
	{
		// ��ʾ�û�
		MessageBox("�ɹ���ȡIMG�ļ���", "ϵͳ��ʾ" , 
			MB_ICONINFORMATION | MB_OK);
		
	}
	else
	{
		// ��ʾ�û�
		MessageBox("��ȡIMG�ļ�ʧ�ܣ�", "ϵͳ��ʾ" , 
			MB_ICONINFORMATION | MB_OK);
	}

	// ������ͼ
	pDoc->UpdateAllViews(NULL);
	
	// �ָ����
	EndWaitCursor();
	
}

