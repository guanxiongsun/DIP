//
//jpg系数处理类，作者： 范晓鹏
// 
//#include "stdafx.h"

#include "Bmp.h"
#include "yuv.h"
#include <math.h>
#include <memory.h> 
#include "dibapi.h"

IMPLEMENT_DYNAMIC(CBmp,CGdiObject);

CBmp* PASCAL CBmp::FromHandle(HBITMAP hBitmap)

{ 
	
	return (CBmp*) CGdiObject::FromHandle(hBitmap); 
	
}

CBmp::CBmp()
{
	lpinfo=NULL;
}

BOOL CBmp::LoadBitmap(LPCTSTR lpszFileName)

{ 
	
	CFile file;
	
	if(!file.Open(lpszFileName,CFile::modeRead|CFile::shareDenyWrite))
		
	{
		
		MessageBox(NULL,"BMP file open error!","warning",MB_OK);
		
		return FALSE;
		
	}
	
	BITMAPFILEHEADER bfhHeader;
	
	file.Read(&bfhHeader,sizeof(BITMAPFILEHEADER));
	
	if(bfhHeader.bfType!=((WORD) ('M'<<8)|'B'))
		
	{
		
		MessageBox(NULL,"The file is not a BMP file!","warning",MB_OK);
		
		return FALSE;
		
	}
	
	if(bfhHeader.bfSize!=file.GetLength())
		
	{
		
		MessageBox(NULL,"The BMP file header error!","warning",MB_OK);
		
		return FALSE;
		
	}
	
	UINT uBmpInfoLen=(UINT) bfhHeader.bfOffBits-sizeof(BITMAPFILEHEADER);
	
	LPBITMAPINFO lpBitmap=(LPBITMAPINFO) new BYTE[uBmpInfoLen];
	
	file.Read((LPVOID) lpBitmap,uBmpInfoLen);
	
	if((* (LPDWORD)(lpBitmap))!=sizeof(BITMAPINFOHEADER))
		
	{
		
		MessageBox(NULL,"The BMP is not Windows 3.0 format!","warning",MB_OK);
		
		return FALSE;
		
	}
	
	DWORD dwBitlen=bfhHeader.bfSize - bfhHeader.bfOffBits;
	
	LPVOID lpBits=new BYTE[dwBitlen];
	
	file.ReadHuge(lpBits,dwBitlen);
	
	file.Close();
	
	BOOL bSuccess=CreateBitmapIndirect(lpBitmap, lpBits);
	if(lpinfo!=NULL) delete lpinfo;
	lpinfo=lpBitmap;
	
	
	delete lpBits;
	
	if(!bSuccess)
		
		return FALSE;
	
	return TRUE;
	
}

BOOL CBmp::CreateBitmap(int nWidth, int nHeight, UINT nBitCount, 
						
						const void* lpSrcBits)
						
{
	
	ASSERT(nBitCount==1||nBitCount==4||nBitCount==8
		
		||nBitCount==16||nBitCount==24||nBitCount==32);
	
	//LPBITMAPINFO &lpBitmap=lp_info;
	LPBITMAPINFO lpBitmap;
	lpBitmap=(BITMAPINFO*) new BYTE[sizeof(BITMAPINFOHEADER) + 
		GetColorNumber(nBitCount) * sizeof(RGBQUAD)];
	
	lpBitmap->bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	
	lpBitmap->bmiHeader.biWidth=nWidth;
	
	lpBitmap->bmiHeader.biHeight=nHeight;
	
	lpBitmap->bmiHeader.biBitCount=nBitCount;
	
	lpBitmap->bmiHeader.biPlanes=1;
	
	lpBitmap->bmiHeader.biCompression=BI_RGB;
	
	lpBitmap->bmiHeader.biSizeImage=0;
	
	lpBitmap->bmiHeader.biClrUsed=0;

	BOOL bSuccess=CreateBitmapIndirect(lpBitmap, lpSrcBits);
	
//	delete lpBitmap;
	if(lpinfo!=NULL)delete lpinfo;
	lpinfo=lpBitmap;	
	if(!bSuccess)
		
		return FALSE;
	
	return TRUE;
	
}

BOOL CBmp::CreateBitmapIndirect(LPBITMAPINFO lpBitmapInfo, const void* lpSrcBits)

{
	
	DeleteObject();
	
	LPVOID lpBits;
	
	CDC *dc=new CDC;
	
	dc->CreateCompatibleDC(NULL);
	
	HBITMAP hBitmap=::CreateDIBSection(dc->m_hDC,lpBitmapInfo,DIB_RGB_COLORS,
		
		&lpBits,NULL,0);
	
	ASSERT(hBitmap!=NULL);
	
	delete dc;
	
	Attach(hBitmap);
	
	BITMAP bmp;
	
	GetBitmap(&bmp);
	
	DWORD dwCount=(DWORD) bmp.bmWidthBytes * bmp.bmHeight;
	
	if(SetBitmapBits(dwCount,lpSrcBits)!=dwCount)
		
	{
		
		MessageBox(NULL,"DIB build error!","warning",MB_OK);
		
		return FALSE;
		
	}
	
	return TRUE;
	
}

CBmp::operator HBITMAP() const

{ 
	
	return (HBITMAP)(this == NULL ? NULL : m_hObject); 
	
}

int CBmp::GetBitmap(BITMAP* pBitMap)

{ 
	
	ASSERT(m_hObject != NULL);
	
	int result=::GetObject(m_hObject, sizeof(BITMAP), pBitMap); 
	if(pBitMap->bmBitsPixel==24)
		while(pBitMap->bmWidthBytes%4!=0)pBitMap->bmWidthBytes++;
		return result;
}

int CBmp::GetColorNumber(WORD wBitCount)

{
	
	ASSERT(wBitCount==1||wBitCount==4||wBitCount==8
		
		||wBitCount==16||wBitCount==24||wBitCount==32);
	
	switch(wBitCount)
		
	{
		
	case 1:
		
		return 2;
		
	case 4:
		
		return 16;
		
	case 8:
		
		return 256;
		
	default:
		
		return 0;
		
	}
	
}

DWORD CBmp::SetBitmapBits(DWORD dwCount, const void* lpBits)

{
	
	if(lpBits!=NULL)
		
	{
		
		BITMAP bmp;
		
		GetBitmap(&bmp);
		
		memcpy(bmp.bmBits,lpBits,dwCount);
		
		return dwCount;
		
	}
	
	else 
		
		return 0;
	
}

DWORD CBmp::GetBitmapBits(DWORD dwCount, LPVOID lpBits)

{
	
	if(lpBits!=NULL)
		
	{
		
		BITMAP bmp;
		
		GetBitmap(&bmp);
		
		memcpy(lpBits,bmp.bmBits,dwCount);
		
		return dwCount;
		
	}
	
	else 
		
		return 0;
	
}

CBmp::~CBmp()

{
	DeleteObject();
}

BOOL CBmp::DeleteObject()
{
	if(lpinfo!=NULL) delete lpinfo;
	lpinfo=NULL;
	return CGdiObject::DeleteObject();
}

void CBmp::OnDraw(CDC *pDC)
{
	CDC dcMemory;
	BITMAP info;
	
	GetBitmap(&info);
    dcMemory.CreateCompatibleDC(pDC);
    dcMemory.SelectObject (this);
	pDC->BitBlt (0,0,info.bmWidth,info.bmHeight,&dcMemory,0,0,SRCCOPY);  
	dcMemory.ReleaseAttribDC();
}

CSize CBmp::GetSize()
{
	BITMAP bmp;
	this->GetBitmap(&bmp);
	return(CSize(bmp.bmWidth,bmp.bmHeight));
}






void CBmp::OnDraw(CDC *pDC, CRect *pRect)
{
	CDC dcMemory;
	BITMAP info;
	
	GetBitmap(&info);
    dcMemory.CreateCompatibleDC(pDC);
    dcMemory.SelectObject (this);
	
	//	pDC->StretchBlt(0,0,pRect->right,pRect->bottom,&dcMemory,0,0,info.bmWidth,info.bmHeight,SRCCOPY); 
	pDC->BitBlt (0,0,info.bmWidth,info.bmHeight,&dcMemory,0,0,SRCCOPY);  
	dcMemory.ReleaseAttribDC();
}

BOOL CBmp::SaveBitmap(LPCTSTR lpszFileName)
{
	if(!GetSafeHandle())return FALSE;
	HANDLE hDib=CreateDib(BI_RGB,NULL);
	
	if(!hDib)return FALSE;
	BOOL res=WriteDib(lpszFileName,hDib);
	::GlobalFree(hDib);
	return res;
}
// writedib		- writes a dib to file
// returns		- true on success
// szfile		- name of file to write to
// hdib			- handle of the dib
BOOL CBmp::WriteDib(LPCTSTR lpszFileName, HANDLE hDib)
{
	BITMAPFILEHEADER	hdr;
	LPBITMAPINFOHEADER	lpbi;
	
	if (!hDib)
		return FALSE;
	
	CFile file;
	if(!file.Open(lpszFileName,CFile::modeWrite|CFile::modeCreate))
		return FALSE;
	
	lpbi = (LPBITMAPINFOHEADER)hDib;
	
	int ncolors = 1 << lpbi->biBitCount;
	if( ncolors> 256 ) 
		ncolors = 0;
	// fill in the fields of the file header 
	hdr.bfType = ((WORD) ('M' << 8) | 'B'); // is always "bm" 
	hdr.bfSize=::GlobalSize(hDib) + sizeof( hdr ); 
	hdr.bfReserved1=0; 
	hdr.bfReserved2=0; 
	hdr.bfOffBits=(DWORD) (sizeof( hdr ) + lpbi->biSize +
		ncolors * sizeof(RGBQUAD));
	
	// write the file header 
	file.Write( &hdr, sizeof(hdr) );
	
	// write the dib header and the bits 
	file.Write( lpbi, GlobalSize(hDib) );
	
	return TRUE;
}


// ddbtodib		- creates a dib from a ddb
// bitmap		- device dependent bitmap
// dwcompression	- type of compression - see bitmapinfoheader
// ppal			- logical palette
HANDLE CBmp::CreateDib(DWORD dwcompression, CPalette* ppal ) 
{
	BITMAP			bm;
	BITMAPINFOHEADER	bi;
	LPBITMAPINFOHEADER 	lpbi;
	DWORD			dwlen;
	HANDLE			hdib;
	HANDLE			handle;
	HDC 			hdc;
	HPALETTE		hpal;
	
	
	ASSERT(GetSafeHandle());
	
	// the function has no arg for bitfields
	if( dwcompression == BI_BITFIELDS )
		return NULL;
	
	// if a palette has not been supplied use defaul palette
	hpal = (HPALETTE) ppal->GetSafeHandle();
	if (hpal==NULL)
		hpal = (HPALETTE) ::GetStockObject(DEFAULT_PALETTE);
	
	// get bitmap information
	GetBitmap(&bm);
	
	// initialize the bitmapinfoheader
	bi.biSize		= sizeof(BITMAPINFOHEADER);
	bi.biWidth		= bm.bmWidth;
	bi.biHeight 		= bm.bmHeight;
	bi.biPlanes 		= 1;
	bi.biBitCount		= bm.bmPlanes * bm.bmBitsPixel;
	bi.biCompression	= dwcompression;
	bi.biSizeImage		= 0;
	bi.biXPelsPerMeter	= 0;
	bi.biYPelsPerMeter	= 0;
	bi.biClrUsed		= 0;
	bi.biClrImportant	= 0;
	
	// compute the size of the  infoheader and the color table
	int ncolors = (1 << bi.biBitCount); 
	if( ncolors> 256 ) 
		ncolors = 0;
	dwlen  = bi.biSize + ncolors * sizeof(RGBQUAD);
	
	// we need a device context to get the dib from
	hdc = ::GetDC(NULL);
	hpal = ::SelectPalette(hdc,hpal,FALSE);
	::RealizePalette(hdc);
	
	// allocate enough memory to hold bitmapinfoheader and color table
	hdib = GlobalAlloc(GMEM_FIXED,dwlen);
	
	if (!hdib){
		SelectPalette(hdc,hpal,FALSE);
		ReleaseDC(NULL,hdc);
		return NULL;
	}
	
	lpbi = (LPBITMAPINFOHEADER)hdib;
	
	*lpbi = bi;
	
	// call getdibits with a null lpbits param, so the device driver 
	// will calculate the bisizeimage field 
	::GetDIBits(hdc, HBITMAP(GetSafeHandle()), 0L, (DWORD)bi.biHeight,
		(LPBYTE)NULL, (LPBITMAPINFO)lpbi, (DWORD)DIB_RGB_COLORS);
	
	bi = *lpbi;
	
	// if the driver did not fill in the bisizeimage field, then compute it
	// each scan line of the image is aligned on a dword (32bit) boundary
	if (bi.biSizeImage == 0){
		bi.biSizeImage = ((((bi.biWidth * bi.biBitCount) + 31) & ~31) / 8) 
			* bi.biHeight;
		
		// if a compression scheme is used the result may infact be larger
		// increase the size to account for this.
		if (dwcompression != BI_RGB)
			bi.biSizeImage = (bi.biSizeImage * 3) / 2;
	}
	
	// realloc the buffer so that it can hold all the bits
	dwlen += bi.biSizeImage;
	if (handle = ::GlobalReAlloc(hdib, dwlen, GMEM_MOVEABLE))
		hdib = handle;
	else{
		::GlobalFree(hdib);
		
		// reselect the original palette
		SelectPalette(hdc,hpal,FALSE);
		ReleaseDC(NULL,hdc);
		return NULL;
	}
	
	// get the bitmap bits
	lpbi = (LPBITMAPINFOHEADER)hdib;
	
	// finally get the dib
	BOOL bgotbits = ::GetDIBits( hdc, HBITMAP(GetSafeHandle()),
		0L,				// start scan line
		(DWORD)bi.biHeight,		// # of scan lines
		(LPBYTE)lpbi 			// address for bitmap bits
		+ (bi.biSize + ncolors * sizeof(RGBQUAD)),
		(LPBITMAPINFO)lpbi,		// address of bitmapinfo
		(DWORD)DIB_RGB_COLORS);		// use rgb for color table
	
	if( !bgotbits )
	{
		GlobalFree(hdib);
		
		SelectPalette(hdc,hpal,FALSE);
		ReleaseDC(NULL,hdc);
		return NULL;
	}
	
	SelectPalette(hdc,hpal,FALSE);
	ReleaseDC(NULL,hdc);
	return hdib;
}

BOOL CBmp::CreateDuplicate(CBmp *pSrc)
{
	BITMAP info;
	if(!pSrc->GetBitmap(&info))return FALSE;
	return CreateBitmapIndirect(pSrc->lpinfo,info.bmBits);
}


BOOL CBmp::CreateFromYuv(CYuv *pYuv)
{
	int i,j;
	int w,h,bmWidthBytes;
	this->DeleteObject();
	w=pYuv->m_width;
	h=pYuv->m_height;
	bmWidthBytes=w*3;
	while(bmWidthBytes%4!=0)
		bmWidthBytes++;
	unsigned char *lpbits=new unsigned char [h*bmWidthBytes];

#define B lpbits[i*bmWidthBytes+j*3]
#define G lpbits[i*bmWidthBytes+j*3+1]
#define R lpbits[i*bmWidthBytes+j*3+2]
#define Y pYuv->y[i*w+j]
#define U pYuv->u[(i/2)*((w+1)/2)+(j/2)]
#define V pYuv->v[(i/2)*((w+1)/2)+(j/2)]

	for(i=0;i<h;i++)
		for(j=0;j<w;j++)
		{
			if(pYuv->u!=NULL&&pYuv->v!=NULL)
			{
				R = CLIP(Y+1.140*V);
				G = CLIP(Y-0.395*U-0.581*V);
				B = CLIP(Y+2.032*U);
			}
			else
			{
				R=G=B=CLIP(Y);
			}
		}
#undef R
#undef G
#undef B
#undef Y
#undef U
#undef V
	return this->CreateBitmap(w,h,24,lpbits);
}
