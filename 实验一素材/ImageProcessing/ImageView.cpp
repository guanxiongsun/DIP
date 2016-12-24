// ************************************************************************
//  �ļ�����ImageView.cpp
//
//  ͼ��ԭAPI�����⣺
//
//  LimbPatternBayer()				- ��BAYER������ʾͼ��
//  DitherFloydSteinberg()			- ��Floyd��Steinberg�㷨��������ͼ��
//
// *************************************************************************


#include "stdafx.h"
#include "GlobalApi.h"
#include "Cdib.h"

#include <math.h>
#include <direct.h>
#include <complex>
using namespace std;

/*************************************************************************
 *
 * �������ƣ�
 *   LimbPatternBayer()
 *
 * ����:
 *   CDib  *pDib       - ָ��CDib���ָ��
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú�������BAYER������ʾͼ��
 *
 ************************************************************************/


BOOL LimbPatternBayer(CDib *pDib)
{
	// Bayer��Ķ���
	BYTE BayerPattern[8][8]={ 0, 32,  8, 40,  2, 34, 10, 42,
							 48, 16, 56, 24, 50, 18, 58, 26,
							 12, 44,  4, 36, 14, 46,  6, 38,
							 60, 28, 52, 20, 62, 30, 54, 22,
							  3, 35, 11, 43,  1, 33,  9, 41,
							 51, 19, 59, 27, 49, 17, 57, 25,
							 15, 47,  7, 39, 13, 45,  5, 37,
							 63, 31, 55, 23, 61, 29, 53, 21};

	// ָ��Դͼ���ָ��
	BYTE *	lpSrc;

	//ͼ��Ŀ�Ⱥ͸߶�
	LONG    lWidth;
	LONG    lHeight;

	//�õ�ʵ�ʵ�Dibͼ��洢��С
	CSize   SizeRealDim;
	SizeRealDim = pDib->GetDibSaveDim();

	// ͼ��ÿ�е��ֽ���
	LONG	lLineBytes;
	
	//�õ�ͼ��Ŀ�Ⱥ͸߶�
	CSize   SizeDim;
	SizeDim = pDib->GetDimensions();
	lWidth  = SizeDim.cx;
	lHeight = SizeDim.cy;;

	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = SizeRealDim.cx;
	
	//ͼ�����ݵ�ָ��
	LPBYTE  lpDIBBits = pDib->m_lpImage;

	// ѭ������
	int i, j;

	// ���ص�ֵ
	int nPixelValue;

	// ��ͼ���ֵ��������BAYER������ʾͼ��
	for (j = 0; j < lHeight ; j++)
	{
		for(i = 0; i < lLineBytes ; i++)
		{
			// ָ��Դͼ������j�У���i�����ص�ָ��			
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
			
			nPixelValue = (*lpSrc);

			nPixelValue =nPixelValue;
			
			// ������λ�����Ƚ�
			if ( (nPixelValue>>2) > BayerPattern[j&7][i&7]) 
                //��׵�    
				*(lpSrc)=(unsigned char)255; 
						
			else 
				//��ڵ�
				*(lpSrc)=(unsigned char)0; 
		}		
	}

	return true;
}


/*************************************************************************
 *
 * �������ƣ�
 *   DitherFloydSteinberg()
 *
 * ����:
 *   CDib  *pDib       - ָ��CDib���ָ��
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú���������Floyd��Steinberg�㷨��������ͼ��
 *
 ************************************************************************/
BOOL DitherFloydSteinberg(CDib *pDib)
{	
	// ָ��Դͼ���ָ��
	BYTE *	lpSrc;
	
	//ͼ��Ŀ�Ⱥ͸߶�
	LONG    lWidth;
	LONG    lHeight;
	
	// ͼ��ÿ�е��ֽ���
	LONG	lLineBytes;
	
	//�õ�ͼ��Ŀ�Ⱥ͸߶�
	CSize   SizeDim;
	SizeDim = pDib->GetDimensions();
	lWidth  = SizeDim.cx;
	lHeight = SizeDim.cy;
	
	//�õ�ʵ�ʵ�Dibͼ��洢��С
	CSize   SizeRealDim;
	SizeRealDim = pDib->GetDibSaveDim();
		
	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = SizeRealDim.cx;
	
	//ͼ�����ݵ�ָ��
	LPBYTE  lpDIBBits = pDib->m_lpImage;
	
	// ѭ������
	int i, j;

	// ����ϵ��
	double temp, error;
	
	// ����ֵ
	int nPixelValue;
	
	// ��ͼ���ֵ��������Floyd��Steinberg�㷨��������ͼ��
	for (j = 0; j < lHeight; j++)
	{
		for(i = 0; i < lLineBytes; i++)
		{
			// ָ��Դͼ������j�У���i�����ص�ָ��			
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
			
			nPixelValue = *lpSrc;
			
			//128����ֵ
			if ( nPixelValue > 128 )
			{ 				
				//��׵�
				*lpSrc=255; 
				
				//�������
				error = (double)(nPixelValue-255.0); 							
			}
			
			else
			{
				//��ڵ�
				*lpSrc=0; 
				
				//�������
				error = (double)nPixelValue; 
			}
			
			// ������Ǳ߽�
			if(i < lLineBytes-1)
			{ 
				//���Ҵ���
				temp = (float)*(lpSrc+1);
				
				temp = temp + error * (1.5/8.0);
				
				if(temp > 255.0)
					temp = 255.0;

				*(lpSrc+1)=(int)temp; 
				
			}
			
			// ������Ǳ߽�			
			if(j < lHeight - 1)
			{ 				
				// ���´���
				temp = (float)*(lpSrc + lLineBytes);
				
				temp = temp + error * (1.5/8.0);				
				
				*(lpSrc+lLineBytes) = (int)temp;
				
				if(i < lLineBytes-1)
				{
					// �����´���
					temp = (float)*(lpSrc + lLineBytes + 1);
					
					temp = temp + error * (2.0/16.0);
				
					*(lpSrc + lLineBytes + 1) = (int)temp;				
				}
			}
			
		}
		
	}
	
	return true;	
}
