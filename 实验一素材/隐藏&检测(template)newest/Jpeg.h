// Jpeg.h: interface for the CJpegR class.
//
//////////////////////////////////////////////////////////////////////
//jpg系数处理类，作者: 范晓鹏

#if !defined(AFX_JPEG_H__2B04A781_2160_11D5_A275_000021E9F5AB__INCLUDED_)
#define AFX_JPEG_H__2B04A781_2160_11D5_A275_000021E9F5AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ijl.h"
#include "Bmp.h"	// Added by ClassView


#define MAX_RAW_QTBLS 4
#define MAX_RAW_HTBLS 8


class CJpegR  
{
public:
	CJpegR();
	virtual ~CJpegR();
public:
	bool FreeImage();
	bool NewImage(char *input_file);
private:
	bool InitIjl();
	bool FreeIjl();
public:
	void AdjustCoef();
	BOOL WriteJpg(char *input_file);
	CBmp m_bmp;
	void DoMark();

	int FileNum;

	CString m_path;
	bool ReadRawDCTData();
	bool DecDCTData();
  JPEGQuantTable rawquanttables[MAX_RAW_QTBLS];
  unsigned char  quantizer[MAX_RAW_QTBLS][64];
  char buf[MAX_RAW_QTBLS][8][8];
  int            nqtables;
  int            maxquantindex;
  int            qprecision;

  // raw huffman tables
  JPEGHuffTable  rawhufftables[MAX_RAW_HTBLS];
  unsigned char  bits[MAX_RAW_HTBLS][16];
  unsigned char  vals[MAX_RAW_HTBLS][256];
  int            nhuffActables;
  int            nhuffDctables;
  int            maxhuffindex;

  // JPEG specific I/O data specifiers. 
  int                JPGWidth;
  int                JPGHeight;
  int                BufWidth[3];
  int				 BufHeight[3];
  int                JPGChannels;
  IJL_COLOR          JPGColor;
  IJL_JPGSUBSAMPLING JPGSubsampling;
  int                progressive_found;

  RAW_DATA_TYPES_STATE raw_coefs;
  short* lp_dctdata[4];
	JPEG_CORE_PROPERTIES jcprops;
private:	IJLERR               jerr;
};

#endif // !defined(AFX_JPEG_H__2B04A781_2160_11D5_A275_000021E9F5AB__INCLUDED_)
