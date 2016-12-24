// JpegSteg.cpp: implementation of the CJpegSteg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WaterMark.h"
#include "JpegSteg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJpegSteg::CJpegSteg()
{
	
}

CJpegSteg::~CJpegSteg()
{
	
}

CJpegSteg::HideData(BYTE *pData, int Count)
/*隐藏信息的函数
pData: 要隐藏的信息的缓冲区指针
Count: 要隐藏的字节数
jpg系数在raw_coefs中，隐藏时先用
raw_coefs.raw_ptrs[c]+i*BufWidth[c]+j读出系数，
修改后再保存回raw_coefs.raw_ptrs[c]+i*BufWidth[c]+j中
其中c为颜色通道(0:Y 1:U 2:V)，i为行数，j为列数。

系数的排列方式如下，A为直流系数, "."为交流系数
每一个8*8的块中,左上角为直流系数，从左上到右下是从低频到高频的系数

A . . . . . . . A . . . . . . . A . . . . . . . A . . . . . . . 
. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
A . . . . . . . A . . . . . . . A . . . . . . . A . . . . . . .
. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
*/
{
	AdjustCoef();//调整量化表，因为写系数时只支持一种量化表
				//所以隐藏信息前，需要把量化表和系数调整好
				//只有嵌入时需要调用，提取和检测时不需要
	int i,j,c,qt;
	int k,t;
	//把量化表读入buf中
	for(qt=0;qt<this->maxquantindex;qt++)
		for(i=0,j=0,k=0;k<64;k++)
		{	
			buf[qt][i][j]=this->quantizer[qt][k];
			if(i==0)
			{t=i,i=j,j=t;
			if(i==7)j++;
			else i++;
			}
			else
				if(j==7)
				{t=i,i=j,j=t;
				j++;
				}
				else i--,j++;
		}


	//对颜色、行列循环，读取系数并修改
	//读取系数和修改也可以分开进行
	for(c = 0; c < jcprops.JPGChannels; c++)
	{
		//得到该颜色通道的宽和高
		this->BufWidth[c]=jcprops.jprops.numxMCUs*8*jcprops.jprops.jframe.comps[c].hsampling;
		this->BufHeight[c]=jcprops.jprops.numyMCUs*8*jcprops.jprops.jframe.comps[c].vsampling;
		
		//得到量化表编号
		int sel=this->jcprops.jprops.jframe.comps[c].quant_sel;	
		
		//对行列循环
		for(i=0;i<BufHeight[c];i++)
			for(j=0;j<BufWidth[c];j++)
			{
				short coef;	
				coef=*(this->raw_coefs.raw_ptrs[c]+i*BufWidth[c]+j);//量化后的系数
				
				//请在这里修改coef

				//或先读出系数到一个缓冲区，然后修改后，
				//再仿照这个循环写回到*(this->raw_coefs.raw_ptrs[c]+i*BufWidth[c]+j)中
				
				*(this->raw_coefs.raw_ptrs[c]+i*BufWidth[c]+j)=coef;
				// 量化表里的系数=buf[sel][i%8][j%8];
			}
	}
	return TRUE;//成功嵌入返回TRUE否则返回FALSE;	
}

CJpegSteg::GetData(BYTE *pData, int Count)
/*提取信息的函数
pData: 准备保存隐藏信息的缓冲区指针
Count: 准备提取的字节数
请按照Count指定的字节数提取前Count字节保存在pData中，注意不要越界
jpg系数的获取与HideData类似*/
{
	int i,j,c,qt;
	int k,t;
	for(qt=0;qt<this->maxquantindex;qt++)
		for(i=0,j=0,k=0;k<64;k++)
		{	
			buf[qt][i][j]=this->quantizer[qt][k];
			if(i==0)
			{t=i,i=j,j=t;
			if(i==7)j++;
			else i++;
			}
			else
				if(j==7)
				{t=i,i=j,j=t;
				j++;
				}
				else i--,j++;
		}
		
		for(c = 0; c < jcprops.JPGChannels; c++)
		{
			this->BufWidth[c]=jcprops.jprops.numxMCUs*8*jcprops.jprops.jframe.comps[c].hsampling;
			this->BufHeight[c]=jcprops.jprops.numyMCUs*8*jcprops.jprops.jframe.comps[c].vsampling;
			
			int sel=this->jcprops.jprops.jframe.comps[c].quant_sel;	
			for(i=0;i<BufHeight[c];i++)
				for(j=0;j<BufWidth[c];j++)
				{
					short coef;	
					coef=*(this->raw_coefs.raw_ptrs[c]+i*BufWidth[c]+j);//量化后的系数
					//coef
					// 量化步长=buf[sel][i%8][j%8];
				}
		}
		
		return TRUE;//成功提取返回TRUE否则返回FALSE;	
}
/*检测是否有隐藏信息的函数
jpg系数的获取与HideData类似，请参考HideData
请返回一个0~1的浮点数
表示有隐藏信息的可能性
*/
float CJpegSteg::Detect()
{
	int i,j,c,qt;
	int k,t;
	for(qt=0;qt<this->maxquantindex;qt++)
		for(i=0,j=0,k=0;k<64;k++)
		{	
			buf[qt][i][j]=this->quantizer[qt][k];
			if(i==0)
			{t=i,i=j,j=t;
			if(i==7)j++;
			else i++;
			}
			else
				if(j==7)
				{t=i,i=j,j=t;
				j++;
				}
				else i--,j++;
		}
		
	for(c = 0; c < jcprops.JPGChannels; c++)
	{
		this->BufWidth[c]=jcprops.jprops.numxMCUs*8*jcprops.jprops.jframe.comps[c].hsampling;
		this->BufHeight[c]=jcprops.jprops.numyMCUs*8*jcprops.jprops.jframe.comps[c].vsampling;
		
		int sel=this->jcprops.jprops.jframe.comps[c].quant_sel;	
		for(i=0;i<BufHeight[c];i++)
			for(j=0;j<BufWidth[c];j++)
			{
				short coef;	
				coef=*(this->raw_coefs.raw_ptrs[c]+i*BufWidth[c]+j);//量化后的系数
				//coef
				// 量化步长=buf[sel][i%8][j%8];
			}
	}
	
	return 0;
}
