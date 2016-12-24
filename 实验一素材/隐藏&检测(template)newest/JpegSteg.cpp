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
/*������Ϣ�ĺ���
pData: Ҫ���ص���Ϣ�Ļ�����ָ��
Count: Ҫ���ص��ֽ���
jpgϵ����raw_coefs�У�����ʱ����
raw_coefs.raw_ptrs[c]+i*BufWidth[c]+j����ϵ����
�޸ĺ��ٱ����raw_coefs.raw_ptrs[c]+i*BufWidth[c]+j��
����cΪ��ɫͨ��(0:Y 1:U 2:V)��iΪ������jΪ������

ϵ�������з�ʽ���£�AΪֱ��ϵ��, "."Ϊ����ϵ��
ÿһ��8*8�Ŀ���,���Ͻ�Ϊֱ��ϵ���������ϵ������Ǵӵ�Ƶ����Ƶ��ϵ��

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
	AdjustCoef();//������������Ϊдϵ��ʱֻ֧��һ��������
				//����������Ϣǰ����Ҫ���������ϵ��������
				//ֻ��Ƕ��ʱ��Ҫ���ã���ȡ�ͼ��ʱ����Ҫ
	int i,j,c,qt;
	int k,t;
	//�����������buf��
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


	//����ɫ������ѭ������ȡϵ�����޸�
	//��ȡϵ�����޸�Ҳ���Էֿ�����
	for(c = 0; c < jcprops.JPGChannels; c++)
	{
		//�õ�����ɫͨ���Ŀ�͸�
		this->BufWidth[c]=jcprops.jprops.numxMCUs*8*jcprops.jprops.jframe.comps[c].hsampling;
		this->BufHeight[c]=jcprops.jprops.numyMCUs*8*jcprops.jprops.jframe.comps[c].vsampling;
		
		//�õ���������
		int sel=this->jcprops.jprops.jframe.comps[c].quant_sel;	
		
		//������ѭ��
		for(i=0;i<BufHeight[c];i++)
			for(j=0;j<BufWidth[c];j++)
			{
				short coef;	
				coef=*(this->raw_coefs.raw_ptrs[c]+i*BufWidth[c]+j);//�������ϵ��
				
				//���������޸�coef

				//���ȶ���ϵ����һ����������Ȼ���޸ĺ�
				//�ٷ������ѭ��д�ص�*(this->raw_coefs.raw_ptrs[c]+i*BufWidth[c]+j)��
				
				*(this->raw_coefs.raw_ptrs[c]+i*BufWidth[c]+j)=coef;
				// ���������ϵ��=buf[sel][i%8][j%8];
			}
	}
	return TRUE;//�ɹ�Ƕ�뷵��TRUE���򷵻�FALSE;	
}

CJpegSteg::GetData(BYTE *pData, int Count)
/*��ȡ��Ϣ�ĺ���
pData: ׼������������Ϣ�Ļ�����ָ��
Count: ׼����ȡ���ֽ���
�밴��Countָ�����ֽ�����ȡǰCount�ֽڱ�����pData�У�ע�ⲻҪԽ��
jpgϵ���Ļ�ȡ��HideData����*/
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
					coef=*(this->raw_coefs.raw_ptrs[c]+i*BufWidth[c]+j);//�������ϵ��
					//coef
					// ��������=buf[sel][i%8][j%8];
				}
		}
		
		return TRUE;//�ɹ���ȡ����TRUE���򷵻�FALSE;	
}
/*����Ƿ���������Ϣ�ĺ���
jpgϵ���Ļ�ȡ��HideData���ƣ���ο�HideData
�뷵��һ��0~1�ĸ�����
��ʾ��������Ϣ�Ŀ�����
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
				coef=*(this->raw_coefs.raw_ptrs[c]+i*BufWidth[c]+j);//�������ϵ��
				//coef
				// ��������=buf[sel][i%8][j%8];
			}
	}
	
	return 0;
}
