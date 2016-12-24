// RWJpegExpView.h : interface of the CRWJpegExpView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWJPEGEXPVIEW_H__CD50400A_AEA7_4039_84D6_0F4C3D006E71__INCLUDED_)
#define AFX_RWJPEGEXPVIEW_H__CD50400A_AEA7_4039_84D6_0F4C3D006E71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef __cplusplus
	extern "C" {
#endif // __cplusplus

#include "jpeglib.h"

#ifdef __cplusplus
	}
#endif // __cplusplus
	
//#include"Dib.h"
//#include"Jpeg.h"
#include "jpeglib.h"

struct image {
	int x, y, depth, max;
	unsigned char *img;
};

//extern struct jpeg_decompress_struct *jinfo;

struct jeasy {
	int comp;
	int height[MAX_COMPS_IN_SCAN];
	int width[MAX_COMPS_IN_SCAN];
	struct jpeg_decompress_struct *jinfo;
	JQUANT_TBL *table[MAX_COMPS_IN_SCAN];
	short ***blocks;
	int needscale;
	double scale[MAX_COMPS_IN_SCAN];
};

class CRWJpegExpView : public CScrollView
{
protected: // create from serialization only
	CRWJpegExpView();
	DECLARE_DYNCREATE(CRWJpegExpView)

// Attributes
public:
	CRWJpegExpDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRWJpegExpView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
	public:
		BOOL F5ReCompress(struct image *image, struct jeasy *je, int quality, CString filename);
		BOOL FreeJpgComDecStruct(struct jpeg_decompress_struct *cinfo,struct jeasy *je);
		FILE * infile;		

		int m_nHeight;
		int m_nWidth;
		BOOL InitJpgComDecStruct(struct jpeg_decompress_struct *cinfo,struct jeasy *je, CString filename);
		//BOOL InitJpgComDecStruct();
//		struct jpeg_decompress_struct jinfo;
//		struct jpeg_decompress_struct cinfo;
		void mat_transpose(double **out, double **in, int size);
		void dct_init(void);
		void idct(short *, short *);
		void dct(short *, short *);
		double ** mat_new(int size);
		void mat_mul(double **out, double **a, double **b, int size);
		void mat_sadd(double **out, double **in, double val, int size);
		void dcttomat(double **mat, short *dct);
		void mattodct(short *dct, double **mat);
		
		double variance(short *block);
		int dovariance(struct jeasy *je, int i, int j, int k, void *arg);
		int iterate_all_doroughness(struct jeasy *je, void *arg);
		int iterate_all_dovariance(struct jeasy *je, void *arg);
		int diff_horizontal(short *left, short *right);
		int diff_vertical(short *up, short *down);
		int doroughness(struct jeasy *je, int i, int j, int k, void *arg);
		
		int iterate_all(struct jeasy *je,int (*f)(struct jeasy *, int, int, int, void *), void *arg);
		
		void statistic(struct jeasy *je);
		void quant_block(short *out, short *in, JQUANT_TBL *table);

		void dequant_block(short *out, short *in, JQUANT_TBL *table);
		void jpeg_free_blocks(struct jeasy *je);
		struct jpeg_decompress_struct * f5_compress(struct image *image, struct jeasy *je, int quality, FILE **pfin);
		int f5_hkl(struct jeasy *je, short ik, short il, short val);
		void f5_dobeta(struct jeasy *je, struct jeasy *jne, double *pbeta, double *pekl,int quality, int verbose);
		double f5_ekl(struct jeasy *orig, struct jeasy *est, double beta, int k, int l);
		double betakl(struct jeasy *orig, struct jeasy *est, int k, int l);
		void f5_crop(struct image *image);
		void f5_blur(struct image *image, double d);
		int f5_decompress(struct jpeg_decompress_struct *jinfo, struct image *image);
		void f5_luminanceimage(struct jeasy *je, struct image *image);
		struct jeasy * jpeg_prepare_blocks(struct jpeg_decompress_struct *jsrc);
		float SteganalysisF5(CString m_strJpegFileName);
		CString m_strJpegFileName;
		virtual ~CRWJpegExpView();
#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRWJpegExpView)
	afx_msg void OnSteganalysisF5();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in RWJpegExpView.cpp
inline CRWJpegExpDoc* CRWJpegExpView::GetDocument()
   { return (CRWJpegExpDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RWJPEGEXPVIEW_H__CD50400A_AEA7_4039_84D6_0F4C3D006E71__INCLUDED_)
