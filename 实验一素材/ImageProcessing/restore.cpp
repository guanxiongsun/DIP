// ************************************************************************
//  �ļ�����restore.cpp
//
//  ͼ��ԭAPI�����⣺
//
//  DIBNoRestriction()			- ͼ��ģ��
//  DIBInverseFilter()			- ͼ�����˲���ԭ
//  DIBNoiseDegeneration()		- ͼ��ģ������
//  DIBWinnerFilter()			- ͼ��ά���˲�
//	DIBMotionDegeneration()		- ͼ���˶�ģ��
//	DIBMotionRestore()			- ͼ���˶�ģ����ԭ
//
// *************************************************************************


#include "stdafx.h"
#include "GlobalApi.h"
#include "Cdib.h"

#include <math.h>
#include <direct.h>
#include <complex>
using namespace std;

#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr

/*************************************************************************
 *
 * �������ƣ�
 *   DIBNoRestriction()
 *
 * ����:
 *   CDib  *pDib       - ָ��CDib���ָ��
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú���������DIBͼ�����ģ��������
 *
 ************************************************************************/

BOOL WINAPI DIBNoRestriction(CDib *pDib)
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
	
	//ѭ������
	long i;
	long j;

	//��ʱ����
	double temp;

	// ʵ�ʽ��и���Ҷ�任�Ŀ�Ⱥ͸߶�
	LONG	lW = 1;
	LONG	lH = 1;
	
	int		wp = 0;
	int		hp = 0;

	// ��֤��ɢ����Ҷ�任�Ŀ�Ⱥ͸߶�Ϊ2�������η�
	while(lW * 2 <= lLineBytes)
	{
		lW = lW * 2;
		wp++;
	}
	
	while(lH * 2 <= lHeight)
	{
		lH = lH * 2;
		hp++;
	}

	//�����洢Դͼ��ͱ任�˵�ʱ������
	complex<double> *pCTSrc,*pCTH;

	//�����洢Դͼ��ͱ任�˵�Ƶ������
	complex<double>  *pCFSrc,*pCFH;
	
	//ͼ���һ������
	double MaxNum;
	
	//����ͼ��ĳ��Ϳ����Ϊ2��������
	if(lW != (int) lLineBytes)
	{
		return false;
	}

	if(lH != (int) lHeight)
	{
		return false;
	}

	// Ϊʱ���Ƶ����������ռ�
	pCTSrc = new complex<double> [lHeight*lLineBytes];
	pCTH   = new complex<double> [lHeight*lLineBytes];
	
	pCFSrc = new complex<double> [lHeight*lLineBytes];
	pCFH   = new complex<double> [lHeight*lLineBytes];

	// �����ݴ���ʱ������
	for (j = 0; j < lHeight; j++)
	{
		for(i = 0; i < lLineBytes; i++)
		{
			// ָ��Դͼ������j�У���i�����ص�ָ��			
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
	
			pCTSrc[ lLineBytes*j + i ] = complex<double>((double)*lpSrc , 0);
			pCFSrc[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
	
			if(i < 5 && j < 5)
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.04 , 0.0);
			}
			else
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
			}
			pCFH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
		}
	}

	//��Դͼ�����FFT
	::DIBFFT_2D(pCTSrc, lLineBytes, lHeight, pCFSrc);
	
	//�Ա任��ͼ�����FFT
	::DIBFFT_2D(pCTH, lLineBytes, lHeight, pCFH);

	//Ƶ�����
	for (i = 0;i <lHeight*lLineBytes;i++)
	{
		pCFSrc[i] = pCFSrc[i]*pCFH[i];
	}

	//�Խ��ͼ����з�FFT
	IFFT_2D(pCFSrc, pCTSrc, lLineBytes, lHeight);

	//ȷ����һ������
	MaxNum = 0.0;
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			temp = sqrt(pCTSrc[ lLineBytes*j + i ].real() * pCTSrc[ lLineBytes*j + i ].real()
						+pCTSrc[lLineBytes*j + i ].imag() * pCTSrc[ lLineBytes*j +i].imag());
			
			//ѡ���һ������
			if( MaxNum < temp)
				MaxNum = temp;
		}
	}
	
	//ת��Ϊͼ��
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// ָ��Դͼ������j�У���i�����ص�ָ��			
 			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
	
			*lpSrc = (unsigned char) (pCTSrc[(lLineBytes)*j + i].real()*255.0/MaxNum);
		}
	}
	
	//�ͷŴ洢�ռ�
	delete pCTSrc;
	delete pCTH;

	delete pCFSrc;
	delete pCFH;

	// ����
	return true;
}

/*************************************************************************
 *
 * �������ƣ�
 *   DIBInverseFilter()
 *
 * ����:
 *   CDib  *pDib       - ָ��CDib���ָ��
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE
 *
 * ˵��:
 *   �ú���������DIBNoRestriction()���ɵ�DIBͼ����и�ԭ������
 *
 ************************************************************************/

BOOL WINAPI DIBInverseFilter (CDib *pDib)
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
	
	//ѭ������
	long i;
	long j;

	//��ʱ����
	double tempre, tempim, a, b, c, d;

	// ʵ�ʽ��и���Ҷ�任�Ŀ�Ⱥ͸߶�
	LONG	lW = 1;
	LONG	lH = 1;
	
	int		wp = 0;
	int		hp = 0;

	// ��֤��ɢ����Ҷ�任�Ŀ�Ⱥ͸߶�Ϊ2�������η�
	while(lW * 2 <= lLineBytes)
	{
		lW = lW * 2;
		wp++;
	}
	
	while(lH * 2 <= lHeight)
	{
		lH = lH * 2;
		hp++;
	}

	//�����洢Դͼ��ͱ任�˵�ʱ������
	complex<double> *pCTSrc,*pCTH;

	//�����洢Դͼ��ͱ任�˵�Ƶ������
	complex<double>  *pCFSrc,*pCFH;
	
	//ͼ���һ������
	double MaxNum;
	
	//�����˻�ͼ��ĳ��Ϳ����Ϊ2��������
	if(lW != (int) lLineBytes)
	{
		return false;
	}

	if(lH != (int) lHeight)
	{
		return false;
	}

	// Ϊʱ���Ƶ����������ռ�
	pCTSrc = new complex<double> [lHeight*lLineBytes];
	pCTH   = new complex<double> [lHeight*lLineBytes];
	
	pCFSrc = new complex<double> [lHeight*lLineBytes];
	pCFH   = new complex<double> [lHeight*lLineBytes];

	// ���˻�ͼ�����ݴ���ʱ������
	for (j = 0; j < lHeight; j++)
	{
		for(i = 0; i < lLineBytes; i++)
		{
			// ָ���˻�ͼ������j�У���i�����ص�ָ��			
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
	
			pCTSrc[ lLineBytes*j + i ] = complex<double>((double)*lpSrc , 0);
			pCFSrc[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
	
			if(i < 5 && j < 5)
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.04 , 0.0);
			}
			else
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
			}
			pCFH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
		}
	}

	//���˻�ͼ�����FFT
	::DIBFFT_2D(pCTSrc, lLineBytes, lHeight, pCFSrc);
	
	//�Ա任��ͼ�����FFT
	::DIBFFT_2D(pCTH, lLineBytes, lHeight, pCFH);
	
	//Ƶ�����
	for (i = 0;i <lHeight*lLineBytes;i++)
	{
		a = pCFSrc[i].real();
		b = pCFSrc[i].imag();
		c = pCFH[i].real();
		d = pCFH[i].imag();
		
		//���Ƶ��ֵ̫С�����迼��
		if (c*c + d*d > 1e-3)
		{
			tempre = ( a*c + b*d ) / ( c*c + d*d );
			tempim = ( b*c - a*d ) / ( c*c + d*d );
		}

		pCFSrc[i]= complex<double>(tempre , tempim);
	}

	//�Ը�ԭͼ����з�FFT
	IFFT_2D(pCFSrc, pCTSrc, lLineBytes, lHeight);

	//ȷ����һ������
	MaxNum=300;

	//ת��Ϊ��ԭͼ��
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// ָ��ԭͼ������j�У���i�����ص�ָ��			
 			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
	
			*lpSrc = (unsigned char) (pCTSrc[(lLineBytes)*j + i].real()*255.0/MaxNum);
		}
	}
	
	//�ͷŴ洢�ռ�
	delete pCTSrc;
	delete pCTH;

	delete pCFSrc;
	delete pCFH;

	// ����
	return true;
}

/*************************************************************************
 *
 * �������ƣ�
 *   DIBNoiseDegeneration()
 *
 * ����:
 *   CDib  *pDib       - ָ��CDib���ָ��
 *
 * ����ֵ:
 *   BOOL               - ģ����������ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú���������DIBͼ�����ģ�����������
 *
 ************************************************************************/

BOOL WINAPI DIBNoiseDegeneration (CDib *pDib)
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
	
	//ѭ������
	long i;
	long j;

	//ת��Ϊͼ�񣬼���
	unsigned char NoisePoint;
	
	//��ʱ����
	double temp;

	//ͼ���һ������
	double MaxNum;

	// ʵ�ʽ��и���Ҷ�任�Ŀ�Ⱥ͸߶�
	LONG	lW = 1;
	LONG	lH = 1;
	
	int		wp = 0;
	int		hp = 0;

	// ��֤��ɢ����Ҷ�任�Ŀ�Ⱥ͸߶�Ϊ2�������η�
	while(lW * 2 <= lLineBytes)
	{
		lW = lW * 2;
		wp++;
	}
	
	while(lH * 2 <= lHeight)
	{
		lH = lH * 2;
		hp++;
	}

	//�����洢Դͼ��ͱ任�˵�ʱ������
	complex<double> *pCTSrc,*pCTH;

	//�����洢Դͼ��ͱ任�˵�Ƶ������
	complex<double>  *pCFSrc,*pCFH;
		
	// Ϊʱ���Ƶ����������ռ�
	pCTSrc = new complex<double> [lHeight*lLineBytes];
	pCTH   = new complex<double> [lHeight*lLineBytes];
	
	pCFSrc = new complex<double> [lHeight*lLineBytes];
	pCFH   = new complex<double> [lHeight*lLineBytes];
	
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// ָ��Դͼ������j�У���i�����ص�ָ��				
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
	
			// ������ֵ�洢��ʱ��������
			pCTSrc[ lLineBytes*j + i ] = complex<double>((double)*lpSrc , 0);
			
			// Ƶ����ֵ
			pCFSrc[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
	
			// ������ͼ�����˻���ϵͳ
			if(i < 5 && j <5 )
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.04 , 0.0);
			}
			else
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
			}
			
			// Ƶ����ֵ
			pCFH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
		}
	}

	//��Դͼ�����FFT
	::DIBFFT_2D(pCTSrc, lLineBytes, lHeight, pCFSrc);
	
	//�Ա任��ͼ�����FFT
	::DIBFFT_2D(pCTH, lLineBytes, lHeight, pCFH);

	//Ƶ�����
	for (i = 0;i <lHeight*lLineBytes;i++)
	{
		pCFSrc[i] = pCFSrc[i]*pCFH[i];
	}

	//�Խ��ͼ����з�FFT
	IFFT_2D(pCFSrc, pCTSrc, lLineBytes, lHeight);

	//ȷ����һ������
	MaxNum = 0.0;
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			temp = sqrt(pCTSrc[ lLineBytes*j + i ].real() * pCTSrc[ lLineBytes*j + i ].real()
						+pCTSrc[lLineBytes*j + i ].imag() * pCTSrc[ lLineBytes*j +i].imag());
			
			//ѡ���һ������
			if( MaxNum < temp)
				MaxNum = temp;
		}
	}
	
	//����α���������
	srand((unsigned)time(NULL));

	//ת��Ϊͼ�񣬲�����α�������
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// ����������
			NoisePoint = rand()/2048-8;
			
			// ָ��Դͼ������j�У���i�����ص�ָ��			
 			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
	
			// ʱ����룬�洢����ֵ
			*lpSrc = (unsigned char) (pCTSrc[(lLineBytes)*j + i].real()*255.0/MaxNum + NoisePoint);
			
			//�������ֵ����ֱ�Ӹ�ֵ255
			if(*lpSrc > 255)
				*lpSrc = 255 ;
		}
	}	
	
	//�ͷŴ洢�ռ�
	delete pCTSrc;
	delete pCTH;

	delete pCFSrc;
	delete pCFH;

	// ����
	return true;
}

/*************************************************************************
 *
 * �������ƣ�
 *   DIBWinnerFilter()
 *
 * ����:
 *   CDib  *pDib       - ָ��CDib���ָ��
 *
 * ����ֵ:
 *   BOOL               - ά���˲���ԭ�����ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú���������DIBͼ�����ά���˲���ԭ������
 *
 ************************************************************************/

BOOL WINAPI DIBWinnerFilter (CDib *pDib)
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
	
	//ѭ������
	long i;
	long j;

	//��ʱ����
	double temp, tempre, tempim, 
			a, b, c, d, norm2;
	
	// ʵ�ʽ��и���Ҷ�任�Ŀ�Ⱥ͸߶�
	LONG	lW = 1;
	LONG	lH = 1;
	
	int		wp = 0;
	int		hp = 0;

	// ��֤��ɢ����Ҷ�任�Ŀ�Ⱥ͸߶�Ϊ2�������η�
	while(lW * 2 <= lLineBytes)
	{
		lW = lW * 2;
		wp++;
	}
	
	while(lH * 2 <= lHeight)
	{
		lH = lH * 2;
		hp++;
	}

	//�����洢Դͼ��ͱ任�˵�ʱ������
	complex<double> *pCTSrc,*pCTH;

	//�����洢Դͼ��ͱ任�˵�Ƶ������
	complex<double>  *pCFSrc,*pCFH;
	
	//�����˻�ͼ��ĳ��Ϳ����Ϊ2��������
	if(lW != (int) lLineBytes)
	{
		return false;
	}

	if(lH != (int) lHeight)
	{
		return false;
	}

	// Ϊʱ���Ƶ����������ռ�
	pCTSrc		= new complex<double> [lHeight*lLineBytes];
	pCTH		= new complex<double> [lHeight*lLineBytes];
	
	pCFSrc		= new complex<double> [lHeight*lLineBytes];
	pCFH		= new complex<double> [lHeight*lLineBytes];

	// �˲�����Ȩϵ��
	double *pCFFilter   = new double [lHeight*lLineBytes];

	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// ָ���˻�ͼ������j�У���i�����ص�ָ��			
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
	
			// ������ֵ�洢��ʱ��������
			pCTSrc[ lLineBytes*j + i ] = complex<double>((double)*lpSrc , 0);
			
			// Ƶ����ֵ
			pCFSrc[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
	
			// �˻�ϵͳʱ��ά���˲���Ȩϵ����ֵ
			if(i < 5 && j <5)
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.04 , 0.0);
				pCFFilter[ lLineBytes*j + i ] = 0.5;
			}
			else
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);	
				pCFFilter[ lLineBytes*j + i ] = 0.05;
			}

			// Ƶ����ֵ
			pCFH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
		}
	}
	
	//���˻�ͼ�����FFT
	::DIBFFT_2D(pCTSrc, lLineBytes, lHeight, pCFSrc);
	
	//�Ա任��ͼ�����FFT
	::DIBFFT_2D(pCTH, lLineBytes, lHeight, pCFH);
	
	// ����M
	for (i = 0; i < lHeight * lLineBytes; i++)
	{
			// ��ֵ
			a = pCFSrc[i].real();
			b = pCFSrc[i].imag();
			c = pCFH[i].real();
			d = pCFH[i].imag();

			// |H(u,v)|*|H(u,v)|
			norm2 = c * c + d * d;

			// |H(u,v)|*|H(u,v)|/(|H(u,v)|*|H(u,v)|+a)
			temp  = (norm2 ) / (norm2 + pCFFilter[i]);

			{				
				tempre = ( a*c + b*d ) / ( c*c + d*d );
				tempim = ( b*c - a*d ) / ( c*c + d*d );

				// ���f(u,v)
				pCFSrc[i]= complex<double>(temp*tempre , temp*tempim);
			}						
	}

	//�Ը�ԭͼ����з�FFT
	IFFT_2D(pCFSrc, pCTSrc, lLineBytes, lHeight);

	//ת��Ϊ��ԭͼ��
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// ָ��ԭͼ������j�У���i�����ص�ָ��			
 			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;

			a = pCTSrc[(lLineBytes)*j + i].real();
			b = pCTSrc[(lLineBytes)*j + i].imag();

			norm2  = a*a + b*b;
			norm2  = sqrt(norm2) + 40;
	
			if(norm2 > 255)
				norm2 = 255.0;
			if(norm2 < 0)
				norm2 = 0;	

			*lpSrc = (unsigned char) (norm2);			
		}
	}

	//�ͷŴ洢�ռ�
	delete pCTSrc;
	delete pCTH;

	delete pCFSrc;
	delete pCFH;
	delete pCFFilter;

	// ����
	return true;
}

/*************************************************************************
 *
 * �������ƣ�
 *   DIBMotionDegeneration()
 *
 * ����:
 *   CDib  *pDib       - ָ��CDib���ָ��
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú���������DIBͼ��ģ��������ֱ���˶���ɵ�ģ��
 *
 ***********************************************************************
 */

BOOL WINAPI DIBMotionDegeneration(CDib *pDib)
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
	
	//ѭ������
	long iColumn;
	long jRow;

	//��ʱ����
	int temp,m;

	// ��ʱ����
	double p,q;
	
	int nTotTime, nTotLen, nTime;

	//�ܵ��˶�ʱ��10s
	nTotTime = 10;

	// �ܵ��˶�����10�����ص�
	nTotLen = 10;
	
	// ������ı���ϵ��
	double B;

	B = 0.1;

	//�����洢Դͼ��ͱ任�˵�ʱ������
	int *nImageDegener;

	// Ϊʱ���Ƶ����������ռ�
	nImageDegener = new int [lHeight*lLineBytes];
	
	// �����ݴ���ʱ������
	for (jRow = 0; jRow < lHeight; jRow++)
	{
		for(iColumn = 0; iColumn < lLineBytes; iColumn++)
		{
			temp=0;
			
			// ָ��Դͼ������jRow�У���iColumn�����ص�ָ��			
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * jRow + iColumn;

			// ���ص������ֵ����
			for ( nTime = 0; nTime < nTotTime; nTime++ )
			{
				p = (float)iColumn - (float)(nTotLen)*nTime/nTotTime;
				
				if (p > 0)
				{
					q = p - floor((double)p);
					
					if(q >= 0.5)
						m = (int)ceil((double)p);
					else
						m = (int)floor((double)p);

					// �ۼ�
					lpSrc = (unsigned char *)lpDIBBits + lLineBytes * jRow + m;
					temp = temp + *lpSrc;
				}	
			}
			
			// ����������ı���ϵ��
			temp = B * temp;

			temp=(int)ceil((double)temp);
			
			// ʹ��temp��ȡֵ����ȡֵ��Χ
			if(temp<0)
				temp=0;

			if(temp>255)
				temp=255;

			nImageDegener[lLineBytes*jRow + iColumn] = temp;
		}
	}	
	
	//ת��Ϊͼ��
	for (jRow = 0;jRow < lHeight ;jRow++)
	{
		for(iColumn = 0;iColumn < lLineBytes ;iColumn++)
		{
			// ָ��Դͼ������jRow�У���iColumn�����ص�ָ��			
 			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * jRow + iColumn;
	
			*lpSrc = nImageDegener[lLineBytes*jRow + iColumn];
		}
	}
	
	//�ͷŴ洢�ռ�
	delete nImageDegener;
	
	// ����
	return true;
}

/*************************************************************************
 *
 * �������ƣ�
 *   DIBMotionRestore()
 *
 * ����:
 *   CDib  *pDib       - ָ��CDib���ָ��
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú�����������������ֱ���˶���ɵ�ģ��ͼ����и�ԭ
 *
 ***********************************************************************
 */

BOOL WINAPI DIBMotionRestore(CDib *pDib)
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
	
	//ѭ������
	long iColumn;
	long jRow;

	int i,n,m;

	//��ʱ����
	int temp1,temp2,
		totalq,q1,q2,z;

	double p,q;

	// ����A��ֵ
	int A = 80;
	
	//����B��ֵ
	int B = 10;
	
	//�ܵ��ƶ�����
	int nTotLen=10;

	// ͼ���Ȱ������ٸ�ntotlen
	int K=lLineBytes/nTotLen;
	
	int error[10];

	//�����洢Դͼ��ʱ������
	int *nImageDegener;

	// Ϊʱ���������ռ�
	nImageDegener = new int [lHeight*lLineBytes];

	// �����ش���������
	for (jRow = 0; jRow < lHeight; jRow++)
	{
		for(iColumn = 0; iColumn < lLineBytes; iColumn++)
		{
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * jRow + iColumn;
	
			nImageDegener[lLineBytes*jRow + iColumn] = (*lpSrc);
		}
	}	
	
	for (jRow = 0; jRow < lHeight; jRow++)
	{		
		// ����error[i]
		for(i = 0; i < 10; i++)
		{
			
			error[i] = 0;
			
			for(n = 0; n < K; n++)
				for(m = 0; m <= n; m++)
				{
					// �����Ƿ�Ϊһ�еĿ�ʼ��
					if(i == 0 && m == 0)
					{
						temp1=temp2=0;
					}
					
					// ���в������
					else
					{
						lpSrc = (unsigned char *)lpDIBBits + lLineBytes * jRow + m*10+i;
						temp1=*lpSrc;
						
						lpSrc = (unsigned char *)lpDIBBits + lLineBytes * jRow + m*10+i-1;
						temp2 = *lpSrc;
					}
					
					error[i] = error[i] + temp1 - temp2;
					
				}				
			
				error[i] = B * error[i] / K;
		}
		
		for(iColumn = 0; iColumn < lLineBytes; iColumn++)
		{			
			// ����m���Լ�z
			m = iColumn / nTotLen;
			z = iColumn - m*nTotLen;			
			
			// ��ʼ��
			totalq = 0;
			q = 0;
			
			for(n = 0; n <= m; n++)
			{
				q1 = iColumn - nTotLen*n;
				
				if(q1 == 0)
					q = 0;
				
				// ���в������
				else
				{
					q2 = q1 - 1;
					
					lpSrc = (unsigned char *)lpDIBBits + lLineBytes * jRow + q1;
					temp1 = *lpSrc;
					
					lpSrc = (unsigned char *)lpDIBBits + lLineBytes * jRow + q2;
					temp2 = *lpSrc;
					
					q = (temp1 - temp2) * B;				
				}
				
				totalq = totalq + q;
			}
			
			p = error[z];

			// �õ�f(x,y)��ֵ
			temp1 = totalq + A - p;
			
			// ʹ�����ص�ȡֵ����ȡֵ��Χ
			if(temp1 < 0)
				temp1 = 0;
			
			if(temp1 > 255)
				temp1 = 255;
						
			nImageDegener[lLineBytes*jRow + iColumn] = temp1;
		}
	}

	//ת��Ϊͼ��
	for (jRow = 0;jRow < lHeight ;jRow++)
	{
		for(iColumn = 0;iColumn < lLineBytes ;iColumn++)
		{
			// ָ��Դͼ������jRow�У���iColumn�����ص�ָ��			
 			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * jRow + iColumn;
	
			// �洢����ֵ
			*lpSrc = nImageDegener[lLineBytes*jRow + iColumn];
		}
	}	
	
	//�ͷŴ洢�ռ�
	delete nImageDegener;
	
	// ����
	return true;
}
#undef SWAP
