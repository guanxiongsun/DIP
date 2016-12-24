
#include "stdafx.h"
#include "cdib.h"
#include "math.h"
#include "GlobalApi.h"

/*************************************************************************
 *
 * \�������ƣ�
 *   TemplateMatch()
 *
 * \�������:
 *   CDib*	pDibSrc		- ָ��CDib���ָ�룬���д�ƥ��ͼ����Ϣ 
 *   CDib*	pDibTemplate	- ָ��CDib���ָ�룬����ģ��ͼ����Ϣ 
 *
 * \����ֵ:
 *   BOOL			- �ɹ��򷵻�TRUE�����򷵻�FALSE
 *
 * \˵��:
 *   �ú�������ͼ�����ģ��ƥ���������Ҫע����ǣ��˳���ֻ����256�Ҷȼ���
 *ͼ��
 *
 *************************************************************************
 */
BOOL TemplateMatch(CDib* pDibSrc, CDib* pDibTemplate)
{	
	// ָ��Դͼ���ָ��
	LPBYTE	lpSrc,lpTemplateSrc;
	
	// ָ�򻺴�ͼ���ָ��
	LPBYTE	lpDst;

	//ѭ������
	long i;
	long j;
	long m;
	long n;

	//�м���
	double dSigmaST;
	double dSigmaS;
	double dSigmaT;

	//�����Բ��
	double R;

	//��������Բ��
	double  dbMaxR;

	//��������Գ���λ��
	int nMaxWidth;
	int nMaxHeight;

	//����ֵ
	unsigned char unchPixel;
	unsigned char unchTemplatePixel;

	// ���ͼ�����ݴ洢�ĸ߶ȺͿ��
	CSize sizeSaveImage;
	sizeSaveImage = pDibSrc->GetDibSaveDim();

	// ���ģ��ͼ�����ݴ洢�ĸ߶ȺͿ��
	CSize sizeSaveTemplate;
	sizeSaveTemplate = pDibTemplate->GetDibSaveDim();


	// ��ʱ�����ڴ棬�Ա�����ͼ��
	CDib* pDibNew;
	pDibNew = new CDib;

	// ��������ڴ�ʧ�ܣ����Ƴ�
	if(!CopyDIB(pDibSrc,pDibNew)){
		// �ͷ��ѷ����ڴ�
		pDibNew->Empty();

		// ����
		return FALSE;
	}
	

	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
	lpDst = (LPBYTE)pDibNew->m_lpImage; 
	memset(lpDst, (BYTE)255, pDibNew->GetSizeImage());

	// ͼ��ĸ߶�
	int nImageHeight ;
	nImageHeight = pDibSrc->m_lpBMIH->biHeight;

	// ͼ��Ŀ��
	int nImageWidth;
	nImageWidth = pDibSrc->m_lpBMIH->biWidth;

	// ģ��ͼ��ĸ߶�
	int nTemplateHeight;
	nTemplateHeight = pDibTemplate->m_lpBMIH->biHeight;

	// ģ��ͼ��Ŀ��
	int nTemplateWidth;
	nTemplateWidth = pDibTemplate->m_lpBMIH->biWidth;

	//����dSigmaT
	dSigmaT = 0;
	for (n = 0;n < nTemplateHeight ;n++)
	{
		for(m = 0;m < nTemplateWidth ;m++)
		{
			// ָ��ģ��ͼ������j�У���i�����ص�ָ��			
			lpTemplateSrc = (LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * n + m;
			unchTemplatePixel = (unsigned char)*lpTemplateSrc;
			dSigmaT += (double)unchTemplatePixel*unchTemplatePixel;
		}
	}

	//�ҵ�ͼ������������Եĳ���λ��
	 dbMaxR = 0.0;
	for (j = 0;j < nImageHeight - nTemplateHeight +1 ;j++)
	{
		for(i = 0;i < nImageWidth - nTemplateWidth + 1;i++)
		{
			dSigmaST = 0;
			dSigmaS = 0;
	
			for (n = 0;n < nTemplateHeight ;n++)
			{
				for(m = 0;m < nTemplateWidth ;m++)
				{
					// ָ��Դͼ������j+n�У���i+m�����ص�ָ��			
					lpSrc  = (LPBYTE)pDibSrc->m_lpImage + sizeSaveImage.cx * (j+n) + (i+m);
			
					// ָ��ģ��ͼ������n�У���m�����ص�ָ��			
					lpTemplateSrc  = (LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * n + m;

					unchPixel = (unsigned char)*lpSrc;
					unchTemplatePixel = (unsigned char)*lpTemplateSrc;

					dSigmaS += (double)unchPixel*unchPixel;
					dSigmaST += (double)unchPixel*unchTemplatePixel;
				}
			}
			//����������
			R = dSigmaST / ( sqrt(dSigmaS)*sqrt(dSigmaT));
			//����������ԱȽ�
			if (R >  dbMaxR)
			{
				 dbMaxR = R;
				nMaxWidth = i;
				nMaxHeight = j;
			}
		}
	}

	//����������Գ������򲿷ָ��Ƶ�Ŀ��ͼ��
	for (n = 0;n < nTemplateHeight ;n++)
	{
		for(m = 0;m < nTemplateWidth ;m++)
		{
			lpTemplateSrc = (LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * n + m;
			lpDst = (LPBYTE)pDibNew->m_lpImage + sizeSaveImage.cx * (n+nMaxHeight) + (m+nMaxWidth);
			*lpDst = *lpTemplateSrc;
		}
	}
	
	// ����ͼ��
	memcpy(pDibSrc->m_lpImage, pDibNew->m_lpImage, nImageWidth * nImageHeight);

	// �ͷ��ڴ�
	pDibNew->Empty();
	
	// ����
	return TRUE;
}

