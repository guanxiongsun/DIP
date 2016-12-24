// StegBmp.cpp: implementation of the CStegBmp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WaterMark.h"
#include "StegBmp.h"
#include "yuv.h"
#include "wavelete.h"
#include "fft.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStegBmp::CStegBmp()
{
	
}

CStegBmp::~CStegBmp()
{
	
}

BOOL CStegBmp::HideData(BYTE *pData, int Count)
/*������Ϣ�ĺ���
pData: Ҫ���ص���Ϣ�Ļ�����ָ��
Count: Ҫ���ص��ֽ���
bmp������ֵ������ָ��Ϊlpbits(=info.bmBits)
����ֵ������lpbits[i*info.bmWidthBytes+j*3+c]��lpbits[i*info.bmWidthBytes+j]���ʡ�
����cΪ��ɫͨ��(0:B 1:G 2:R)��iΪ������jΪ������
������Ϣ����info��
�ɹ�����TRUE�����򷵻�FALSE*/
{
	BITMAP info;
	
	int i,j;
	int k=0;
	
	if(!GetBitmap(&info))return FALSE;
	BYTE *lpbits=(BYTE *)info.bmBits;
	int w=info.bmWidth;
	int h=info.bmHeight;
	switch(info.bmBitsPixel)//�ж�24λ����8λ
	{
	case 24:
		{
			int c;//��ɫ0:B 1:G 2:R
			
			//����ɫ������ѭ������ȡϵ�����޸�
			//��ȡϵ�����޸�Ҳ���Էֿ�����
			for(c=0;c<3;c++)
			{
				for(i=0;i<h;i++)
					for(j=0;j<w;j++)
					{
						//����pData�е���Ϣ
						//���޸�����ֵlpbits[i*info.bmWidthBytes+j*3+c];
						
					}
			}
		}
		break;
	case 8:
		{
			for(i=0;i<h;i++)
				for(j=0;j<w;j++)
				{
					//���޸�����ֵlpbits[i*info.bmWidthBytes+j];
					//����pData�е���Ϣ
				}
		}
		break;
	default:return FALSE;
	}
	this->SetBitmapBits(info.bmWidthBytes*info.bmHeight,lpbits);
	return TRUE;
}



BOOL CStegBmp::GetData(BYTE *pData, int Count)
/*��ȡ��Ϣ�ĺ���
pData: ׼������������Ϣ�Ļ�����ָ��
Count: ׼����ȡ���ֽ���
�밴��Countָ�����ֽ�����ȡǰCount�ֽڱ�����pData�У�ע�ⲻҪԽ��
bmp����ֵ�Ļ�ȡ��HideData����

�ɹ�����TRUE�����򷵻�FALSE*/
{
	BITMAP info;
	
	int i,j;
	
	if(!GetBitmap(&info))return FALSE;
	BYTE *lpbits=(BYTE *)info.bmBits;
	int w=info.bmWidth;
	int h=info.bmHeight;
	switch(info.bmBitsPixel)
	{
	case 24:
		{
			
			
			int c;
			for(c=0;c<3;c++)//��ɫ0:B 1:G 2:R
			{
				for(i=0;i<h;i++)
					for(j=0;j<w;j++)
					{
						
						//����ֵlpbits[i*info.bmWidthBytes+j*3+c];
						//��ȡ��Ϣ��pData��
					}
			}
		}
		break;
	case 8:
		{
			for(i=0;i<h;i++)
				for(j=0;j<w;j++)
				{
					//����ֵlpbits[i*info.bmWidthBytes+j];
					//��ȡ��Ϣ��pData��
				}
		}
		break;
	default:return FALSE;
	}
	return TRUE;
}
/*����Ƿ���������Ϣ�ĺ���
bmp����ֵ�Ļ�ȡ��HideData���ƣ���ο�HideData
�뷵��һ��0~1�ĸ�����
��ʾ��������Ϣ�Ŀ�����*/
float CStegBmp::Detect()
{
	BITMAP info;
	
	int i,j;
	
	if(!GetBitmap(&info))return FALSE;
	BYTE *lpbits=(BYTE *)info.bmBits;
	int w=info.bmWidth;
	int h=info.bmHeight;
	switch(info.bmBitsPixel)
	{
	case 24:
		{
			int c;
			for(c=0;c<3;c++)//��ɫ0:B 1:G 2:R
			{
				for(i=0;i<h;i++)
					for(j=0;j<w;j++)
					{
						
						//����ֵlpbits[i*info.bmWidthBytes+j*3+c];
						//
					}
			}
		}
		break;
	case 8:
		{
			for(i=0;i<h;i++)
				for(j=0;j<w;j++)
				{
					//����ֵlpbits[i*info.bmWidthBytes+j];
					//
				}
		}
		break;
	default:return 0.0;
	}
	return 0.0;
}
