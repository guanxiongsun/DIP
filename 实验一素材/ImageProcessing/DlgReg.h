#if !defined(AFX_DlgReg_H__096C39D7_72FA_4ABD_90AC_688669D1692C__INCLUDED_)
#define AFX_DlgReg_H__096C39D7_72FA_4ABD_90AC_688669D1692C__INCLUDED_

#include "ImageProcessingDoc.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgReg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgReg dialog

class CDlgReg : public CDialog
{
// Construction
public:
	CImageProcessingDoc* m_pDoc;
	CDlgReg(CWnd* pParent = NULL, CImageProcessingDoc* pDoc = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgReg)
	enum { IDD = IDD_DLG_REG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgReg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// ��׼ͼ������ָ�룬����ͼ��ƴ��
	LPBYTE m_lpBaseImg;		

// ����׼ͼ������ָ�룬����ͼ��ƴ��
	LPBYTE m_lpSampImg;		

	// ����ͼ��λ�õı�־λ��FALSE��ʾ��û�м���ͼ��λ��
	BOOL m_bCalImgLoc;		

// ѡȡ�������־λ��FALSE��ʾ��û��ѡȡ
	BOOL m_bChoseFeature;	

// ����ͼ��ȿؼ���λ�ô�С	
	void CalImageLocation();	

	// ��׼ͼ��
	CDib* m_pDibInit;		 

	// ����׼ͼ��
	CDib* m_pDibSamp;		

	// ��׼���ͼ��
	CDib* m_pDibResult;		

	// ��׼ͼ����ʾ����
	CRect m_rectInitImage;		

	// ����׼ͼ����ʾ����
	CRect m_rectResltImage;	

	// ����׼������λ��	
	CPoint m_pPointSampl[3];	

	// ��׼��������λ��
	CPoint m_pPointBase[3];		

	// Ѱ����׼��
	CPoint FindMatchPoint(CDib* pDibBase, CDib* pDibSamp, CPoint pointSamp);			

	// �������ƶ�
	double CalCorrelation(unsigned char* pBase, unsigned char* pSamp, 	int nBlockLen);				
	// ����������
	void   DrawFeature(CDC* pDC);			

	// ��÷���任ϵ��
	void   GetAffinePara(CPoint* pPointBase, CPoint* pPointSampl, double* pDbBs2SpAffPara);		
	// ���������������
	void   CalMatProduct(double* pDbSrc1, double *pDbSrc2, double *pDbDest, int nX, int nY, int nZ);			
	// ����������
	BOOL   CalInvMatrix(double* pDbSrc, int nLen);	
	
	// ��ô���׼ͼ�����任�������
	CRect  GetAftAffDim(double* pDbAffPara);	

	// �������任��Ĵ���׼ͼ��
	LPBYTE   SetSampImgAftAff(double* pDbAffPara, CRect rectNewImg);	

	// �������β�ֵ
	unsigned char CalSpline(unsigned char *pUnchCorr, double x, double y);
	 
	// �������任��Ļ�׼ͼ��
	LPBYTE   SetBaseImgAftAff(CRect rectNewImg);			
	
	// Generated message map functions
	//{{AFX_MSG(CDlgReg)
	afx_msg void OnPaint();
	afx_msg void OnRegOpen();
	afx_msg void OnRegReg();
	afx_msg void OnRegChoseFeature();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int m_nChsFeatureNum;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DlgReg_H__096C39D7_72FA_4ABD_90AC_688669D1692C__INCLUDED_)
