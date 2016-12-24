// JpegSteg.h: interface for the CJpegSteg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JPEGSTEG_H__5E32D6A9_D268_4295_9787_5872A8171440__INCLUDED_)
#define AFX_JPEGSTEG_H__5E32D6A9_D268_4295_9787_5872A8171440__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "JpegStegBase.h"

class CJpegSteg : public CJpegStegBase  
{
public:
	float Detect();
	CJpegSteg();
	virtual ~CJpegSteg();
	BOOL GetData(BYTE *pData,int Count);
	BOOL HideData(BYTE *pData,int Count);


};

#endif // !defined(AFX_JPEGSTEG_H__5E32D6A9_D268_4295_9787_5872A8171440__INCLUDED_)
