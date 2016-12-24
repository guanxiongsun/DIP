// JpegStegBase.h: interface for the CJpegStegBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JPEGSTEGBASE_H__2522FB88_8306_46F8_8529_242013CB9CDA__INCLUDED_)
#define AFX_JPEGSTEGBASE_H__2522FB88_8306_46F8_8529_242013CB9CDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Jpeg.h"

class CJpegStegBase : public CJpegR  
{
public:
	virtual float Detect();
	BOOL GetDataPack(BYTE* pData,int Count);
	int GetDataCount();
	BOOL HideDataPack(BYTE *pData,int Count);
	CJpegStegBase();
	virtual ~CJpegStegBase();
	virtual BOOL GetData(BYTE *pData,int Count){return FALSE;};
	virtual BOOL HideData(BYTE *pData,int Count){return FALSE;};

};

#endif // !defined(AFX_JPEGSTEGBASE_H__2522FB88_8306_46F8_8529_242013CB9CDA__INCLUDED_)
