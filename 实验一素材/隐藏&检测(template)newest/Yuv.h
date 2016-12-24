// Yuv.h: interface for the CYuv class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YUV_H__97D862B2_86C0_47DA_B97F_3D071C286295__INCLUDED_)
#define AFX_YUV_H__97D862B2_86C0_47DA_B97F_3D071C286295__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CLIP(x) (x<0?0:(x>255?255:x))
typedef double yuvData;

#include "Bmp.h"
struct WaterMarkPara{double T2,T3,W2,W3;};
class CYuv  
{
#define MAX_INDEX 100
	float m_threshold[MAX_INDEX];
public:
	BOOL AbstractWaterMark(CBmp *pMarkBmp,WaterMarkPara *pPara);
	BOOL AddWaterMark(CBmp *pMarkBmp,WaterMarkPara *pPara);

	
	BOOL CreateGray(CBmp *p_bitmap);
	BOOL Create(int w,int h,yuvData *y,yuvData *u=NULL,yuvData*v=NULL);
	CSize GetSize();
	void OnDraw(CDC *pDC);
	int m_width,m_height;
	BOOL Create(CBmp *p_bitmap=NULL);
	yuvData *y,*u,*v;
	CYuv();
	virtual ~CYuv();

};

#endif // !defined(AFX_YUV_H__97D862B2_86C0_47DA_B97F_3D071C286295__INCLUDED_)
