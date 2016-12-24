// StegBmpBase.h: interface for the CStegBmpBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STEGBMPBASE_H__1C3ED5B6_B18F_46FD_80FC_ED189ADD9577__INCLUDED_)
#define AFX_STEGBMPBASE_H__1C3ED5B6_B18F_46FD_80FC_ED189ADD9577__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Bmp.h"

class CStegBmpBase : public CBmp  
{
public:
	virtual float Detect(){return 0;};
	BOOL GetDataPack(BYTE* pData,int Count);
	int GetDataCount();
	BOOL HideDataPack(BYTE *pData,int Count);

	virtual BOOL GetData(BYTE *pData,int Count){return FALSE;};
	virtual BOOL HideData(BYTE *pData,int Count){return FALSE;};
	CStegBmpBase();
	virtual ~CStegBmpBase();

};

#endif // !defined(AFX_STEGBMPBASE_H__1C3ED5B6_B18F_46FD_80FC_ED189ADD9577__INCLUDED_)
