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
/*隐藏信息的函数
pData: 要隐藏的信息的缓冲区指针
Count: 要隐藏的字节数
bmp的像素值缓冲区指针为lpbits(=info.bmBits)
像素值可以用lpbits[i*info.bmWidthBytes+j*3+c]或lpbits[i*info.bmWidthBytes+j]访问。
其中c为颜色通道(0:B 1:G 2:R)，i为行数，j为列数。
其它信息都在info中
成功返回TRUE，否则返回FALSE*/
{
	BITMAP info;
	
	int i,j;
	int k=0;
	
	if(!GetBitmap(&info))return FALSE;
	BYTE *lpbits=(BYTE *)info.bmBits;
	int w=info.bmWidth;
	int h=info.bmHeight;
	switch(info.bmBitsPixel)//判断24位还是8位
	{
	case 24:
		{
			int c;//颜色0:B 1:G 2:R
			
			//对颜色、行列循环，读取系数并修改
			//读取系数和修改也可以分开进行
			for(c=0;c<3;c++)
			{
				for(i=0;i<h;i++)
					for(j=0;j<w;j++)
					{
						//隐藏pData中的信息
						//请修改像素值lpbits[i*info.bmWidthBytes+j*3+c];
						
					}
			}
		}
		break;
	case 8:
		{
			for(i=0;i<h;i++)
				for(j=0;j<w;j++)
				{
					//请修改像素值lpbits[i*info.bmWidthBytes+j];
					//隐藏pData中的信息
				}
		}
		break;
	default:return FALSE;
	}
	this->SetBitmapBits(info.bmWidthBytes*info.bmHeight,lpbits);
	return TRUE;
}



BOOL CStegBmp::GetData(BYTE *pData, int Count)
/*提取信息的函数
pData: 准备保存隐藏信息的缓冲区指针
Count: 准备提取的字节数
请按照Count指定的字节数提取前Count字节保存在pData中，注意不要越界
bmp像素值的获取与HideData类似

成功返回TRUE，否则返回FALSE*/
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
			for(c=0;c<3;c++)//颜色0:B 1:G 2:R
			{
				for(i=0;i<h;i++)
					for(j=0;j<w;j++)
					{
						
						//像素值lpbits[i*info.bmWidthBytes+j*3+c];
						//提取信息到pData中
					}
			}
		}
		break;
	case 8:
		{
			for(i=0;i<h;i++)
				for(j=0;j<w;j++)
				{
					//像素值lpbits[i*info.bmWidthBytes+j];
					//提取信息到pData中
				}
		}
		break;
	default:return FALSE;
	}
	return TRUE;
}
/*检测是否有隐藏信息的函数
bmp像素值的获取与HideData类似，请参考HideData
请返回一个0~1的浮点数
表示有隐藏信息的可能性*/
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
			for(c=0;c<3;c++)//颜色0:B 1:G 2:R
			{
				for(i=0;i<h;i++)
					for(j=0;j<w;j++)
					{
						
						//像素值lpbits[i*info.bmWidthBytes+j*3+c];
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
					//像素值lpbits[i*info.bmWidthBytes+j];
					//
				}
		}
		break;
	default:return 0.0;
	}
	return 0.0;
}
