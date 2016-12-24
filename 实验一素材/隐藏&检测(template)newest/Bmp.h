//

// 文件描述：定义类CBmp，此类是用于读取BMP文件，涉及读取、

// 建立及一系列常用的操作。

// 文件名： BitmapFile.h 

// 时间： 1999-2-11 

// 作者： 贾暾 

// 

#ifndef _CBmp_H_
#define _CBmp_H_

class CBmp : public CGdiObject

{

DECLARE_DYNAMIC(CBmp)

public:

static CBmp* PASCAL FromHandle(HBITMAP hBitmap);

// Constructors

CBmp();

BOOL LoadBitmap(LPCTSTR lpszFileName);

BOOL CreateBitmap(int nWidth, int nHeight, UINT nBitCount, const void* lpBits);

BOOL CreateBitmapIndirect(LPBITMAPINFO lpBitmapInfo, const void* lpBits);

// Attributes

operator HBITMAP() const;

int GetBitmap(BITMAP* pBitMap);

protected:

// Attributes

int GetColorNumber(WORD wBitCount);

HANDLE	CreateDib(DWORD dwcompression, CPalette* ppal );
BOOL	WriteDib(LPCTSTR lpszFileName, HANDLE hDib);

public:

// Operations

DWORD SetBitmapBits(DWORD dwCount, const void* lpBits);

DWORD GetBitmapBits(DWORD dwCount, LPVOID lpBits);

// Implementation

public:
	BOOL CreateDuplicate(CBmp *pSrc);
	BOOL SaveBitmap(LPCTSTR lpszFileName);
	void OnDraw(CDC *pDC,CRect *pRect);
	LPBITMAPINFO lpinfo;

	CSize GetSize();
	void OnDraw(CDC *pDC);
	BOOL DeleteObject();

virtual ~CBmp();

};

#endif
