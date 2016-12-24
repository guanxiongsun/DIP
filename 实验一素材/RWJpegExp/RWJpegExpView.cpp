// RWJpegExpView.cpp : implementation of the CRWJpegExpView class
//

#include "stdafx.h"
#include "RWJpegExp.h"
#include"MainFrm.h"
#include "RWJpegExpDoc.h"
#include "RWJpegExpView.h"
#include"Dib.h"

#include <io.h>
#include <string.h>
#include <stdio.h>
#include "math.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#ifndef THIS_FILE
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif
#endif

//#include "jpeg.cpp"

/////////////////////////////////////////////////////////////////////////////
// CRWJpegExpView

IMPLEMENT_DYNCREATE(CRWJpegExpView, CScrollView)

BEGIN_MESSAGE_MAP(CRWJpegExpView, CScrollView)
	//{{AFX_MSG_MAP(CRWJpegExpView)
	ON_COMMAND(ID_STEGANALYSIS_F5, OnSteganalysisF5)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRWJpegExpView construction/destruction

CRWJpegExpView::CRWJpegExpView()
{
	// TODO: add construction code here

}

CRWJpegExpView::~CRWJpegExpView()
{
}

BOOL CRWJpegExpView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CRWJpegExpView drawing

void CRWJpegExpView::OnDraw(CDC* pDC)
{
	CRWJpegExpDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	if (! pDoc->m_pDib->IsEmpty())
		pDoc->m_pDib->Display(pDC, 0, 0);
}

void CRWJpegExpView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

		CRWJpegExpDoc* pDoc = GetDocument();

	CSize sizeTotal(pDoc->m_pDib->GetWidth(), pDoc->m_pDib->GetHeight());
	SetScrollSizes(MM_TEXT, sizeTotal);
	
	CMainFrame* pAppFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
	ASSERT_KINDOF(CMainFrame, pAppFrame);
	CRect rc;
	pAppFrame->GetClientRect(&rc);
	if (rc.Width() >= sizeTotal.cx && rc.Height() >= sizeTotal.cy &&
		(sizeTotal.cx>0 || sizeTotal.cy>0))
		ResizeParentToFit(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CRWJpegExpView printing

BOOL CRWJpegExpView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRWJpegExpView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRWJpegExpView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CRWJpegExpView diagnostics

#ifdef _DEBUG
void CRWJpegExpView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CRWJpegExpView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CRWJpegExpDoc* CRWJpegExpView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRWJpegExpDoc)));
	return (CRWJpegExpDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRWJpegExpView message handlers

void CRWJpegExpView::OnSteganalysisF5() 
{
	char strFilterString[]="jpg file(*.jpg)|*.data|text file(*.txt)|*.txt|All file(*. *)|*.*||";
	CFileDialog dlg(TRUE,NULL,"*.jpg",OFN_OVERWRITEPROMPT,(LPSTR)strFilterString);
	dlg.m_ofn.lpstrTitle="打开待分析的图片";
	
	if(dlg.DoModal() == IDCANCEL)
		return;
	else
		m_strJpegFileName	 = dlg.GetPathName();
	SteganalysisF5(m_strJpegFileName);

}


float CRWJpegExpView::SteganalysisF5(CString m_strJpegFileName)
{

	int f5_elim2compress = 0;

	struct jpeg_decompress_struct jinfo;
	struct jpeg_decompress_struct *jnew, *jtmp;
	struct image image;
	struct jeasy *je, *jne;
	double beta, ekl;
	double minbeta, minekl;
	int minquality;
	int quality, verbose = 0;
	FILE *fin;

//	je = jpeg_prepare_blocks(&cinfo); 
	
	if ((je = (struct jeasy *)malloc(sizeof(struct jeasy))) == NULL)
		printf("malloc");
	if ((jne = (struct jeasy *)malloc(sizeof(struct jeasy))) == NULL)
		printf("malloc");
	if ((jnew = (struct jpeg_decompress_struct *)malloc(sizeof(struct jpeg_decompress_struct ))) == NULL)
		printf("malloc");



	InitJpgComDecStruct(&jinfo,je, m_strJpegFileName);

	
	if(infile != NULL)
		fclose(infile);

	image.img = NULL;
	if (f5_elim2compress) {
		minekl = -1;
		for (quality = 90; quality < 99; quality++) {
			/*
			  jtmp = f5_fromfile(filename, &fin);
			  f5_decompress(jtmp, &image);
			  fclose(fin);
			*/
			f5_luminanceimage(je, &image);
			f5_crop(&image);

			/* Re-compress */
			jnew = f5_compress(&image, je, quality, &fin);
			free(image.img); image.img = NULL;
			f5_decompress(jnew, &image);
			fclose(fin);

			f5_blur(&image, 0.05);

			jnew = f5_compress(&image, je, 0, NULL);
			free(image.img); image.img = NULL;
			jne = jpeg_prepare_blocks(jnew);

			f5_dobeta(je, jne, &beta, &ekl, quality, verbose);

			if (minekl == -1 || ekl < minekl) {
				minbeta = beta;
				minekl = ekl;
				minquality = quality;
			}

			jpeg_free_blocks(jne);

			jpeg_destroy_decompress(jnew);
			free(jnew);
		}
	} else {
		CString tempfile = "C:\\stegf5.xxxx";
		f5_luminanceimage(je, &image);
		f5_crop(&image);

		f5_blur(&image, 0.05);

//		jnew = f5_compress(&image, je, 0, temp); //shliu
		F5ReCompress(&image, je, 0, tempfile);
		free(image.img);
		
	//	jne = jpeg_prepare_blocks(jnew);//shliu 

		InitJpgComDecStruct(jnew,jne,tempfile);

		f5_dobeta(je, jne, &beta, &ekl, quality, verbose);

		minbeta = beta;

	//	jpeg_free_blocks(jne);
		FreeJpgComDecStruct(NULL,jne);

		jpeg_destroy_decompress(jnew);
		free(jnew);
	}
//	jpeg_free_blocks(je);
	FreeJpgComDecStruct(NULL,je);

	char tempstr[200];
	int starts = 1;
	if (beta > 0.25)
		starts++;
	if (beta > 0.4)
		starts++;

	sprintf(tempstr,"检测结果为%4.4f:%d",beta,starts);
	AfxMessageBox(tempstr);
	/* fprintf(stderr, "Beta: %f - %f, %d\n", minbeta, minekl, minquality); */

	return (minbeta);
}
struct jeasy * CRWJpegExpView::jpeg_prepare_blocks(struct jpeg_decompress_struct *jsrc)
{
	jvirt_barray_ptr *dctcoeff = jpeg_read_coefficients(jsrc);
	struct jeasy *je;
	int i, j;

	if ((je = (struct jeasy *)malloc(sizeof(struct jeasy))) == NULL)
		printf("malloc");

	memset(je, 0, sizeof(struct jeasy));
	je->jinfo = jsrc;
	je->blocks = (short ***)malloc(jsrc->num_components * sizeof(short **));
	if (je->blocks == NULL)
		printf("malloc");

	je->comp = jsrc->num_components;
	/* Read first ten rows of first component */
	for (i = 0; i < jsrc->num_components; i++) {
		JBLOCKARRAY rows;
		int wib = jsrc->comp_info[i].width_in_blocks;
		int hib = jsrc->comp_info[i].height_in_blocks;

		je->table[i] = jsrc->comp_info[i].quant_table;
		je->height[i] = hib;
		je->width[i] = wib;

		je->blocks[i] = (short **)malloc(wib * hib * sizeof(short *));
		if (je->blocks[i] == NULL)
				printf("malloc");

		for (j = 0; j < wib * hib; j++)
		{
			je->blocks[i][j] = (short *)malloc(DCTSIZE2 * sizeof(short));
			if (je->blocks[i][j] == NULL)
				printf("malloc");
		}

		for (j = 0; j < hib; j++)
		{
			JBLOCKROW row;
			int k, l;

			rows = jsrc->mem->access_virt_barray((j_common_ptr)&jsrc, dctcoeff[i], j, 1, 1);
			if (rows == NULL)
				printf("Access failed");

			row = rows[0];

			for (k = 0; k < wib; k++)
				for (l = 0; l < DCTSIZE2; l++)
					je->blocks[i][j * wib + k][l] = row[k][l];
		}
	}
	return (je);
}

void CRWJpegExpView::f5_luminanceimage(struct jeasy *je, struct image *image)
{
	BYTE *img;
	int i, k, l;
	int hib, wib;
	short **blocks = je->blocks[0];
	short tmp[DCTSIZE2], dq[DCTSIZE2];
	int rowspan;

	hib = je->height[0];
	wib = je->width[0];

	image->x = je->width[0] * DCTSIZE;
	image->y = je->height[0] * DCTSIZE;
	image->max = 255;
	image->depth = 1;

	rowspan = image->x * image->depth;
	
	if ((img = (BYTE*)malloc(image->x*image->y)) == NULL)
		printf("malloc error");

	image->img = img;

	for (k = 0; k < hib; k++) {
		for (l = 0; l < wib; l++) {
			dequant_block(dq, blocks[k*wib + l], je->table[0]);
			idct(tmp, dq);

			for (i = 0; i < DCTSIZE2; i++) 
			{
				int x, y;
				tmp[i] += 128;

				x = l * DCTSIZE + (i % DCTSIZE);
				y = k * DCTSIZE + (i / DCTSIZE);

				if (tmp[i] > image->max)
					tmp[i] = image->max;
				if (tmp[i] < 0)
					tmp[i] = 0;

				img[y * rowspan + x] = tmp[i];
			}
		}
	}
}

struct jpeg_decompress_struct *f5_fromfile(char *filename, FILE **pfin)
{
	struct jpeg_decompress_struct *jinfo;
	static struct jpeg_error_mgr jsrcerr;
	FILE *fin;

	if ((jinfo = (struct jpeg_decompress_struct *)calloc(1, sizeof(struct jpeg_decompress_struct))) == NULL)
		printf("calloc error");

	if ((fin = fopen(filename, "r")) == NULL)
		printf("open %s error", filename);

	jinfo->err = jpeg_std_error(&jsrcerr);
	jpeg_create_decompress(jinfo);
	jpeg_stdio_src(jinfo, fin);

	*pfin = fin;

	return (jinfo);
}

int CRWJpegExpView::f5_decompress(struct jpeg_decompress_struct *jinfo, struct image *image)
{
	JSAMPARRAY buf;
	int rowstep;

	jpeg_read_header(jinfo, TRUE);

	jinfo->do_fancy_upsampling = FALSE;
	jinfo->do_block_smoothing = FALSE;

	jpeg_start_decompress(jinfo);

	image->x = jinfo->output_width;
	image->y = jinfo->output_height;
	image->depth = jinfo->output_components;
	image->max = 255;

	image->img = (BYTE*)malloc(jinfo->output_width * jinfo->output_height *
	    jinfo->output_components);

	if (image->img == NULL)
		printf("malloc error");

	rowstep = jinfo->output_width * jinfo->output_components;

	buf = (jinfo->mem->alloc_sarray)((j_common_ptr) &jinfo, JPOOL_IMAGE, rowstep, 1);

	while (jinfo->output_scanline < jinfo->output_height) {
		jpeg_read_scanlines(jinfo, buf, 1);

		memcpy(&image->img[(jinfo->output_scanline-1)*rowstep],
		    buf[0], rowstep);
	}

	jpeg_finish_decompress(jinfo);
	jpeg_destroy_decompress(jinfo);

	free(jinfo);

	return (0);
}

#define VAL(a, x, y, c)	((a)[(y)*rowspan + (x)*image->depth + c])

void CRWJpegExpView::f5_blur(struct image *image, double d)
{
	int x, y, c;
	int rowspan, size;
	BYTE *newimg, *img, tmp;

	size = image->y * image->x * image->depth;
	if ((newimg = (BYTE*)malloc(size)) == NULL)
		printf("malloc error");
	memcpy(newimg, image->img, size);

	img = (BYTE*)image->img;
	rowspan = image->x * image->depth;
	for (y = 1; y < image->y - 1; y++) {
		for (x = 1; x < image->x - 1; x++) {
			for (c = 0; c < image->depth; c++) {
				tmp = d*VAL(img, x - 1, y, c)
				    + d*VAL(img, x + 1, y, c)
				    + d*VAL(img, x, y - 1, c)
				    + d*VAL(img, x, y + 1, c)
				    + (1 - 4*d)*VAL(img, x, y, c);
				VAL(newimg, x, y, c) = tmp;
			}
		}
	}

	free (img);
	image->img = newimg;
}

void CRWJpegExpView::f5_crop(struct image *image)
{
	struct image newimage;
	int x, y, nx, ny;
	BYTE *buf;
	int rowstep = image->x * image->depth;
	int depth = image->depth;

	newimage.x = image->x - 8;
	newimage.y = image->y - 8;
	newimage.depth = image->depth;
	newimage.max = image->max;

	newimage.img = (BYTE*)malloc(newimage.x * newimage.y * newimage.depth);
	if (newimage.img == NULL)
		printf("malloc error");

	buf = newimage.img;
	nx = ny = 0;
	for (y = 4; y < image->y - 4; y++, ny++) {
		for (x = 4; x < image->x - 4; x++, nx++) {
			memcpy(buf, &image->img[y*rowstep + x*depth], depth);
			buf += depth;
		}
	}

	free(image->img);
	*image = newimage;
}

double CRWJpegExpView::betakl(struct jeasy *orig, struct jeasy *est, int k, int l)
{
	int first, second, third, fourth;
	double beta;

	first = f5_hkl(est, k, l, 1) * (f5_hkl(orig, k, l, 0)
	    - f5_hkl(est, k, l, 0) );
	second = (f5_hkl(orig, k, l, 1) - f5_hkl(est, k, l, 1))
	    * (f5_hkl(est, k, l, 2) - f5_hkl(est, k, l, 1) );

	third = f5_hkl(est, k, l, 1) ;
	third *= third;

	fourth = (f5_hkl(est, k, l, 2) - f5_hkl(est, k, l, 1));
	fourth *= fourth;

	beta = ((double)first + second)/((double)third + fourth);

	return (beta);
}

double CRWJpegExpView::f5_ekl(struct jeasy *orig, struct jeasy *est, double beta, int k, int l)
{
	double first, second;
	int j;

	first = f5_hkl(orig, k, l, 0) - f5_hkl(est, k, l, 0)
	    - beta * f5_hkl(est, k, l, 1);
	first *= first;

	for (j = 1; j < 3; j++) {
		second = f5_hkl(orig, k, l, j) - (1 - beta) * f5_hkl(est, k, l, j)
		    - beta * f5_hkl(est, k, l, j + 1);
		second *= second;

		first += second;
	}

	return (first);
}

void CRWJpegExpView::f5_dobeta(struct jeasy *je, struct jeasy *jne, double *pbeta, double *pekl,
    int quality, int verbose)
{
	double beta, ekl;
	double b1, b2, b3;
	int i;

	jne->needscale = 1;
	for (i = 0; i < jne->comp; i++) {
		jne->scale[i] = ((double)je->width[i]*je->height[i])/
		    ((double)jne->width[i] * jne->height[i]);
	}

	b1 = betakl(je, jne, 1, 2);
	b2 = betakl(je, jne, 2, 1);
	b3 = betakl(je, jne, 2, 2);

	beta = (b1 + b2 + b3)/3;
	ekl = f5_ekl(je, jne, beta, 1, 2)
	    + f5_ekl(je, jne, beta, 2, 1)
	    + f5_ekl(je, jne, beta, 2, 2);

	if (verbose) {
		fprintf(stderr, "Quality: %d\n", quality);

		statistic(je);
		statistic(jne);

		fprintf(stderr, "Original:\n");
		fprintf(stderr, "%d %d\n",
		    f5_hkl(je, 1, 2, 0), f5_hkl(je, 1, 2, 1));
		fprintf(stderr, "%d %d\n",
		    f5_hkl(je, 2, 1, 0), f5_hkl(je, 2, 1, 1));
		fprintf(stderr, "%d %d\n",
		    f5_hkl(je, 2, 2, 0), f5_hkl(je, 2, 2, 1));

		fprintf(stderr, "Estimated:\n");
		fprintf(stderr, "%d %d\n",
		    (int)(f5_hkl(jne, 1, 2, 0)), (int)(f5_hkl(jne, 1, 2, 1)));
		fprintf(stderr, "%d %d\n",
		    (int)(f5_hkl(jne, 2, 1, 0)), (int)(f5_hkl(jne, 2, 1, 1)));
		fprintf(stderr, "%d %d\n",
		    (int)(f5_hkl(jne, 2, 2, 0)), (int)(f5_hkl(jne, 2, 2, 1)));

		fprintf(stderr, "Result:\n");
		fprintf(stderr, "(1,2): %f\n", b1);
		fprintf(stderr, "(2,1): %f\n", b2);
		fprintf(stderr, "(2,2): %f\n", b3);

		fprintf(stderr, "Beta: %f - %f\n", beta, ekl);
	}

	*pbeta = beta;
	*pekl = ekl;
}
int CRWJpegExpView::f5_hkl(struct jeasy *je, short ik, short il, short val)
{
	int i, k;
	int sum = 0;
	short ***blocks = je->blocks;

	for (i = 0; i < 1 /* je->comp */; i++) {
		int presum = 0;
		int hib = je->height[i];
		int wib = je->width[i];

		for (k = 0; k < hib * wib; k++) {
			if (blocks[i][k][il * DCTSIZE + ik] == val)
					presum++;
		}

		if (je->needscale)
			presum *= je->scale[i];
		sum += presum;
	}
	return (sum);
}

struct jpeg_decompress_struct * CRWJpegExpView::f5_compress(struct image *image, struct jeasy *je, int quality, FILE **pfin)
{
	struct jpeg_compress_struct cinfo;
	struct jpeg_decompress_struct *jinfo;
	static struct jpeg_error_mgr jerr, jsrcerr;
	char tempfile[] = "d:\\stegdetect.XXXX";
	JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
	FILE *fout, *fin;
	int row_stride;		/* physical row width in image buffer */
	char * fd;
	
	jinfo = (struct jpeg_decompress_struct *)malloc(sizeof(struct jpeg_decompress_struct));
	if (jinfo == NULL)
		printf("malloc error");

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);

	jpeg_copy_critical_parameters(je->jinfo, &cinfo);

   //if( (fd = _mktemp( tempfile )) == -1 )
     // exit( 1 );


	//if ((fd = mkstemp(template)) == -1)
	//	err(1, "mkstemp");

//	if ((fout = fdopen(tempfile, "w")) == NULL)
//		printf("fdopen error");
	if ((fout = fopen(tempfile, "wb")) == NULL)
		printf("fdopen error");

	jpeg_stdio_dest(&cinfo, fout);

	cinfo.image_width = image->x;
	cinfo.image_height = image->y;
	cinfo.input_components = image->depth;
	if (image->depth == 1)
		cinfo.in_color_space = JCS_GRAYSCALE;
	else
		cinfo.in_color_space = JCS_RGB;

	jpeg_set_defaults(&cinfo);

	/* Take table from decompress object */
	if (quality)
		jpeg_set_quality(&cinfo, quality, TRUE);
	else {
		cinfo.quant_tbl_ptrs[0] = je->table[0];
		cinfo.quant_tbl_ptrs[0]->sent_table = FALSE;
		cinfo.quant_tbl_ptrs[1] = je->table[1];
		cinfo.quant_tbl_ptrs[1]->sent_table = FALSE;
	}

	jpeg_start_compress(&cinfo, TRUE);

	/* JSAMPLEs per row in image_buffer */
	row_stride = image->x * image->depth;

	while (cinfo.next_scanline < cinfo.image_height) {
		row_pointer[0] = & image->img[cinfo.next_scanline * row_stride];
		(void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);
  
	fclose(fout);
//	close(fd);

	/* XXX - ugly reread */
    //打开JPEG文件

	if ((fin = fopen(tempfile, "rb")) == NULL)
		printf("fopen error");

	memset(jinfo, 0, sizeof(struct jpeg_decompress_struct));
	jinfo->err = jpeg_std_error(&jsrcerr);
//	jerr->pub.error_exit = my_error_exit;
//
//	if (setjmp(jerr.setjmp_buffer)) 
//	{
//
//		jpeg_destroy_decompress(&cinfo);
//		fclose(infile);
//		return NULL;
//	}

	jpeg_create_decompress(jinfo);
	jpeg_stdio_src(jinfo, fin);

	if (!quality) {
		jpeg_read_header(jinfo, TRUE);
		jpeg_read_coefficients(jinfo);

		fclose(fin);
	} else
		*pfin = fin;

//	unlink(template);

	return (jinfo);
}
void CRWJpegExpView::jpeg_free_blocks(struct jeasy *je)
{
	int i, j;
	short ***blocks = je->blocks;

	/* Read first ten rows of first component */
	for (i = 0; i < je->comp; i++) {
		int wib = je->width[i];
		int hib = je->height[i];

		for (j = 0; j < hib; j++) {
			int k;

			for (k = 0; k < wib; k++)
				free (blocks[i][j * wib + k]);
		}
		free(blocks[i]);
	}
	free(blocks);
	free(je);
}

/* Dequantize block */

void CRWJpegExpView::dequant_block(short *out, short *in, JQUANT_TBL *table)
{
	int j;

	for (j = 0; j < DCTSIZE2; j++) {
		out[j] = in[j] * table->quantval[j];
	}
}

/* Quantize block */

void CRWJpegExpView::quant_block(short *out, short *in, JQUANT_TBL *table)
{
	int j;

	for (j = 0; j < DCTSIZE2; j++) {
		out[j] = in[j] / table->quantval[j];
	}
}
void CRWJpegExpView::statistic(struct jeasy *je)
{
//	u_int64_t sum; shliu
	long int sum;
	double var;
	int i, total;

	total = 0;
	for (i = 0; i < je->comp; i++)
		total += je->height[i]*je->width[i];

	sum = 0;
	iterate_all_doroughness(je, &sum);
	var = 0;
	iterate_all_dovariance(je,  &var);

	fprintf(stderr, "Roughness:   %lld, Variance:  %f\n", sum, var);
	fprintf(stderr, "Rough/block: %f, Var/block: %f\n",
	    sum/(float)total, var/total);
}

int CRWJpegExpView::iterate_all_doroughness(struct jeasy *je, void *arg)
{
	int i, j, k;

	for (i = 0; i < je->comp; i++)
		for (j = 0; j < je->height[i]; j++)
			for (k = 0; k < je->width[i]; k++)
				if (doroughness(je, i, j, k, arg) == -1)
					return (-1);

	return (0);
}
int CRWJpegExpView::iterate_all_dovariance(struct jeasy *je, void *arg)
{
	int i, j, k;

	for (i = 0; i < je->comp; i++)
		for (j = 0; j < je->height[i]; j++)
			for (k = 0; k < je->width[i]; k++)
				if (dovariance(je, i, j, k, arg) == -1)
					return (-1);

	return (0);
}
int CRWJpegExpView::doroughness(struct jeasy *je, int i, int j, int k, void *arg)
{
	//u_int64_t *psum = arg, sum;
	long int *psum = (long int*)arg, sum;
	int hib, wib;
	short real[DCTSIZE2], n[DCTSIZE2];
	short ***blocks = je->blocks;

	hib = je->height[i];
	wib = je->width[i];
	sum = *psum;

	dequant_block(real, blocks[i][j*wib+k], je->table[i]);
	idct(real, real);

	if (k < wib - 1) {
		dequant_block(n, blocks[i][j*wib+k+1], je->table[i]);
		idct(n, n);

		sum += diff_horizontal(real, n);
	}

	if (j < hib -1) {
		dequant_block(n, blocks[i][(j+1)*wib+k], je->table[i]);
		idct(n, n);

		sum += diff_vertical(real, n);
	}

	*psum = sum;

	return (0);
}

int CRWJpegExpView::diff_horizontal(short *left, short *right)
{
	int i, sum;

	sum = 0;
	for (i = 0; i < DCTSIZE; i++)
		sum += abs(left[i*DCTSIZE + DCTSIZE - 1] - right[i*DCTSIZE]);

	return (sum);
}

int CRWJpegExpView::diff_vertical(short *up, short *down)
{
	int i, sum;

	sum = 0;
	for (i = 0; i < DCTSIZE; i++)
		sum += abs(up[(DCTSIZE-1)*DCTSIZE+i] - down[i]);

	return (sum);
}


int CRWJpegExpView::dovariance(struct jeasy *je, int i, int j, int k, void *arg)
{
	double *psum = (double*) arg, sum;
	int hib, wib;
	short real[DCTSIZE2];
	short ***blocks = je->blocks;

	hib = je->height[i];
	wib = je->width[i];
	sum = *psum;

	dequant_block(real, blocks[i][j*wib+k], je->table[i]);
	idct(real, real);

	sum += variance(real);

	*psum = sum;

	return (0);
}
double CRWJpegExpView::variance(short *block)
{
	double mean, tmp;
	int n;

	mean = 0;
	for (n = 0; n < DCTSIZE2; n++)
		mean += block[n];
	mean /= DCTSIZE2;

	tmp = 0;
	for (n = 0; n < DCTSIZE2; n++)
		tmp += (block[n] - mean)*(block[n] - mean);

	return (sqrt(tmp / (DCTSIZE2-1)));
}

static int dct_inited;
static double **D, **Dt;
static double **tmp1, **tmp2;

static double _D[8][8] = {
	{0.35355339059327, 0.35355339059327, 0.35355339059327, 0.35355339059327, 0.35355339059327, 0.35355339059327, 0.35355339059327, 0.35355339059327},

	{0.49039264020162, 0.41573480615127, 0.27778511650980, 0.09754516100806, -0.09754516100806, -0.27778511650980, -0.41573480615127, -0.49039264020162},

	{0.46193976625564, 0.19134171618254, -0.19134171618254, -0.46193976625564, -0.46193976625564, -0.19134171618255, 0.19134171618255, 0.46193976625564},

	{0.41573480615127, -0.09754516100806, -0.49039264020162, -0.27778511650980, 0.27778511650980, 0.49039264020162, 0.09754516100806, -0.41573480615127},

	{0.35355339059327, -0.35355339059327, -0.35355339059327, 0.35355339059327, 0.35355339059327, -0.35355339059327, -0.35355339059327, 0.35355339059327},

	{0.27778511650980, -0.49039264020162, 0.09754516100806, 0.41573480615127, -0.41573480615127, -0.09754516100806, 0.49039264020162, -0.27778511650980},

	{0.19134171618254, -0.46193976625564, 0.46193976625564, -0.19134171618254, -0.19134171618255, 0.46193976625564, -0.46193976625564, 0.19134171618254},

	{0.09754516100806, -0.27778511650980, 0.41573480615127, -0.49039264020162, 0.49039264020162, -0.41573480615127, 0.27778511650980, -0.09754516100806}
};

void CRWJpegExpView::mat_transpose(double **out, double **in, int size)
{
	int i, j;

	for (i = 0; i < size; i++)
		for (j = 0; j < size; j++)
			out[i][j] = in[j][i];
}

double ** CRWJpegExpView::mat_new(int size)
{
	double **mat;
	int i;

	if ((mat = (double**)malloc(sizeof(double) * size)) == NULL)
		printf("malloc");

	for (i = 0; i < size; i++) {
		if ((mat[i] = (double*)calloc(size, sizeof(double))) == NULL)
			printf("calloc");
	}

	return (mat);
}

void CRWJpegExpView::mat_mul(double **out, double **a, double **b, int size)
{
	int i, j, k;

	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			double sum = 0;

			for (k = 0; k < size; k++)
				sum += a[i][k]*b[k][j];

			out[i][j] = sum;
		}
	}
}

void CRWJpegExpView::mat_sadd(double **out, double **in, double val, int size)
{
	int i, j;

	for (i = 0; i < size; i++)
		for (j = 0; j < size; j++)
			out[i][j] = in[i][j] + val;

}

void CRWJpegExpView::dcttomat(double **mat, short *dct)
{
	int i;

	for (i = 0; i < DCTSIZE2; i++) {
		mat[i >> 3][i & 7] = dct[i];
	}
}

void CRWJpegExpView::mattodct(short *dct, double **mat)
{
	int i;

	for (i = 0; i < DCTSIZE2; i++) {
		dct[i] = mat[i >> 3][i & 7];
	}
}

void CRWJpegExpView::dct_init(void)
{
	int i, j;

	D = mat_new(DCTSIZE);
	Dt = mat_new(DCTSIZE);
	tmp1 = mat_new(DCTSIZE);
	tmp2 = mat_new(DCTSIZE);

	for (i = 0; i < DCTSIZE; i++)
		for (j = 0; j < DCTSIZE; j++)
			D[i][j] = _D[i][j];

	mat_transpose(Dt, D, DCTSIZE);

	dct_inited = 1;
}

void CRWJpegExpView::idct(short *out, short *in)
{
	if (!dct_inited)
		dct_init();

	/* Convert to internal matrix representation */
	dcttomat(tmp1, in);
	/* Do D' * A * D */

	mat_mul(tmp2, Dt, tmp1, DCTSIZE);
	mat_mul(tmp1, tmp2, D, DCTSIZE);

	/* Convert back to dct representation */
	mattodct(out, tmp1);
}

void CRWJpegExpView::dct(short *out, short *in)
{
	if (!dct_inited)
		dct_init();

	/* Convert to internal matrix representation */
	dcttomat(tmp1, in);
	/* Do D' * A * D */

	mat_mul(tmp2, D, tmp1, DCTSIZE);
	mat_mul(tmp1, tmp2, Dt, DCTSIZE);

	/* Convert back to dct representation */
	mattodct(out, tmp1);
}

BOOL CRWJpegExpView::InitJpgComDecStruct(struct jpeg_decompress_struct *cinfo,struct jeasy *je, CString filename)
{
	
	m_nWidth=0;
	m_nHeight=0;

	//定义JPEG文件的解压信息
	struct jpeg_decompress_struct *jsrc = cinfo;
	
	//定义JPEG文件的错误信息
//	struct my_error_mgr jerr;
	static struct jpeg_error_mgr jerr, jsrcerr;
	
	//定义缓冲区
//	FILE * infile;		
	JSAMPARRAY buffer;	
	int row_stride;		
	char buf[250];

	

    //打开JPEG文件
	if ((infile = fopen(filename, "rb")) == NULL) 
	{
		sprintf(buf, "JPEG :\nCan't open %s\n", filename);
		//m_strJPEGError = buf;
		return NULL;
	}

	
    //为JPEG文件解压对象分配内存并对其初始化

	cinfo->err = jpeg_std_error(&jerr);
//	jerr.pub.error_exit = my_error_exit;



//	if (setjmp(jerr.setjmp_buffer)) 
//	{
//		
//
//		jpeg_destroy_decompress(cinfo);
//		fclose(infile);
//		return NULL;
//	}


	jpeg_create_decompress(cinfo);


    //设定数据源 
	jpeg_stdio_src(cinfo, infile);
    //读取JPEG文件参数

	(void) jpeg_read_header(cinfo, TRUE);
	jvirt_barray_ptr *dctcoeff = jpeg_read_coefficients(cinfo);

	int i, j;


	memset(je, 0, sizeof(struct jeasy));
	je->jinfo = jsrc;
	je->blocks = (short ***)malloc(jsrc->num_components * sizeof(short **));
	if (je->blocks == NULL)
		printf("malloc");

	je->comp = jsrc->num_components;
	/* Read first ten rows of first component */
	for (i = 0; i < jsrc->num_components; i++) {
		JBLOCKARRAY rows;
		int wib = jsrc->comp_info[i].width_in_blocks;
		int hib = jsrc->comp_info[i].height_in_blocks;

		je->table[i] = jsrc->comp_info[i].quant_table;
		je->height[i] = hib;
		je->width[i] = wib;

		je->blocks[i] = (short **)malloc(wib * hib * sizeof(short *));
		if (je->blocks[i] == NULL)
				printf("malloc");

		for (j = 0; j < wib * hib; j++)
		{
			je->blocks[i][j] = (short *)malloc(DCTSIZE2 * sizeof(short));
			if (je->blocks[i][j] == NULL)
				printf("malloc");
		}

		for (j = 0; j < hib; j++)
		{
			JBLOCKROW row;
			int k, l;

			rows = jsrc->mem->access_virt_barray((j_common_ptr)&jsrc, dctcoeff[i], j, 1, 1);
			if (rows == NULL)
				printf("Access failed");

			row = rows[0];

			for (k = 0; k < wib; k++)
				for (l = 0; l < DCTSIZE2; l++)
					je->blocks[i][j * wib + k][l] = row[k][l];
		}
	}

	fclose(infile);
	return TRUE;
}

BOOL CRWJpegExpView::FreeJpgComDecStruct(jpeg_decompress_struct *cinfo, jeasy *je)
{
	if( cinfo != NULL)
	{
		free(cinfo);
	}
	if(je != NULL)
	{
		int i, j;
		short ***blocks = je->blocks;
		
		/* Read first ten rows of first component */
		for (i = 0; i < je->comp; i++) {
			int wib = je->width[i];
			int hib = je->height[i];
			
			for (j = 0; j < hib; j++) {
				int k;
				
				for (k = 0; k < wib; k++)
					free (blocks[i][j * wib + k]);
			}
			free(blocks[i]);
		}
		free(blocks);
	}
	return TRUE;
}

BOOL CRWJpegExpView::F5ReCompress(image *image, jeasy *je, int quality, CString filename)
{
	struct jpeg_compress_struct cinfo;
	static struct jpeg_error_mgr jerr, jsrcerr;
//	char tempfile[] = "d:\\stegdetect.XXXX";
	JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
	FILE *fout, *fin;
	int row_stride;		/* physical row width in image buffer */
	char * fd;
	

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);

	jpeg_copy_critical_parameters(je->jinfo, &cinfo);

	if ((fout = fopen(filename, "wb")) == NULL)
		printf("fdopen error");

	jpeg_stdio_dest(&cinfo, fout);

	cinfo.image_width = image->x;
	cinfo.image_height = image->y;
	cinfo.input_components = image->depth;
	if (image->depth == 1)
		cinfo.in_color_space = JCS_GRAYSCALE;
	else
		cinfo.in_color_space = JCS_RGB;

	jpeg_set_defaults(&cinfo);

	/* Take table from decompress object */
	if (quality)
		jpeg_set_quality(&cinfo, quality, TRUE);
	else {
		cinfo.quant_tbl_ptrs[0] = je->table[0];
		cinfo.quant_tbl_ptrs[0]->sent_table = FALSE;
		cinfo.quant_tbl_ptrs[1] = je->table[1];
		cinfo.quant_tbl_ptrs[1]->sent_table = FALSE;
	}

	jpeg_start_compress(&cinfo, TRUE);

	/* JSAMPLEs per row in image_buffer */
	row_stride = image->x * image->depth;

	while (cinfo.next_scanline < cinfo.image_height) {
		row_pointer[0] = & image->img[cinfo.next_scanline * row_stride];
		(void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);
  
	fclose(fout);
	return TRUE;
//	/* XXX - ugly reread */
//    //打开JPEG文件
//
//	struct jpeg_decompress_struct *jinfo;
//	jinfo = (struct jpeg_decompress_struct *)malloc(sizeof(struct jpeg_decompress_struct));
//	if (jinfo == NULL)
//		printf("malloc error");
//
//	if ((fin = fopen(tempfile, "rb")) == NULL)
//		printf("fopen error");
//
//	memset(jinfo, 0, sizeof(struct jpeg_decompress_struct));
//	jinfo->err = jpeg_std_error(&jsrcerr);
//
//	jpeg_create_decompress(jinfo);
//	jpeg_stdio_src(jinfo, fin);
//
//	if (!quality) {
//		jpeg_read_header(jinfo, TRUE);
//		jpeg_read_coefficients(jinfo);
//
//		fclose(fin);
//	} else
//		*pfin = fin;
//
////	unlink(template);
//
//	return (jinfo);
}
