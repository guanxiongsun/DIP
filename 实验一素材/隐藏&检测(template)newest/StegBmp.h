// StegBmp.h: interface for the CStegBmp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STEGBMP_H__FC05F0A7_20F0_4F8F_8719_643BCE6D3E84__INCLUDED_)
#define AFX_STEGBMP_H__FC05F0A7_20F0_4F8F_8719_643BCE6D3E84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StegBmpBase.h"

class CStegBmp : public CStegBmpBase  
{
public:
	float Detect();
	BOOL GetData(BYTE *pData,int Count);
	BOOL HideData(BYTE *pData,int Count);
	CStegBmp();
	virtual ~CStegBmp();
};

#endif // !defined(AFX_STEGBMP_H__FC05F0A7_20F0_4F8F_8719_643BCE6D3E84__INCLUDED_)
