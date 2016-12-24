// RWJpegExpDoc.cpp : implementation of the CRWJpegExpDoc class
//

#include "stdafx.h"
#include "RWJpegExp.h"

#include "RWJpegExpDoc.h"
#include"FormatJpeg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRWJpegExpDoc

IMPLEMENT_DYNCREATE(CRWJpegExpDoc, CDocument)

BEGIN_MESSAGE_MAP(CRWJpegExpDoc, CDocument)
	//{{AFX_MSG_MAP(CRWJpegExpDoc)
	
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRWJpegExpDoc construction/destruction

CRWJpegExpDoc::CRWJpegExpDoc()
{
	// TODO: add one-time construction code here
	m_pDib=new CDib;

}

CRWJpegExpDoc::~CRWJpegExpDoc()
{
	delete m_pDib;
}

BOOL CRWJpegExpDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CRWJpegExpDoc serialization

void CRWJpegExpDoc::Serialize(CArchive& ar)
{
	m_pDib->Serialize(ar);
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CRWJpegExpDoc diagnostics

#ifdef _DEBUG
void CRWJpegExpDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRWJpegExpDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRWJpegExpDoc commands



void CRWJpegExpDoc::OnFileSaveAs() 
{
	// TODO: Add your command handler code here
	CString szSaveFilter= "单色位图 (*.bmp;*.dib)|*.bmp; *.dib|16色位图 (*.bmp;*.dib)|*.bmp; *.dib|256色位图 (*.bmp;*.dib)|*.bmp; *.dib|24位位图 (*.bmp;*.dib)|*.bmp; *.dib|JPEG文件 (*.jpg;*.jpe;*.jpeg)|*.jpg; *.jpe; *.jpeg|All Files (*.*)|*.*||";

	CFileDialog FileDlg(FALSE, "*.bmp", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szSaveFilter);

	CString strPathName = GetPathName();
	char drive[_MAX_DRIVE];   
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];   
	char ext[_MAX_EXT];
	_splitpath((LPCSTR)strPathName, drive, dir, fname, ext);
	
	 if (! stricmp(ext, ".jpg") ||
			 ! stricmp(ext, ".jpe") ||
			 ! stricmp(ext, ".jpeg") ) // JPEG file
		FileDlg.m_ofn.nFilterIndex = 5;
	
	else
	{
		switch (m_pDib->GetBitCount())
		{
		case 1:
			FileDlg.m_ofn.nFilterIndex = 1;
			break;
		case 4:
			FileDlg.m_ofn.nFilterIndex = 2;
			break;
		case 8:
			FileDlg.m_ofn.nFilterIndex = 3;
			break;
		case 24:
		default:
			FileDlg.m_ofn.nFilterIndex = 4;
			break;
		}
	}

	if (FileDlg.DoModal() == IDOK)
	{
		switch (FileDlg.m_ofn.nFilterIndex)
		{
		case 5:
				break;
		case 1:
			m_pDib->ConvertFormat(1);
			break;
		case 2:
			m_pDib->ConvertFormat(4);
			break;
		case 3:
			m_pDib->ConvertFormat(8);
			break;
		case 4:
		default:
			m_pDib->ConvertFormat(24);
			break;
		}

		OnSaveDocument(FileDlg.m_ofn.lpstrFile);
		SetPathName(FileDlg.m_ofn.lpstrFile);
		SetTitle(FileDlg.m_ofn.lpstrFile);
		UpdateAllViews(NULL);
		SetModifiedFlag(FALSE);
	}
	
}

BOOL CRWJpegExpDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	char drive[_MAX_DRIVE];   
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];   
	char ext[_MAX_EXT];
	_splitpath((LPCSTR)lpszPathName, drive, dir, fname, ext);
		if (! stricmp(ext, ".jpg") ||
			 ! stricmp(ext, ".jpe") ||
			 ! stricmp(ext, ".jpeg") ) // JPEG file
	{
	CJpeg jpeg;
		if (! jpeg.Load(lpszPathName))
			return FALSE;

		HDIB hDIB = CopyHandle(jpeg.GetDib()->GetHandle());
		if (hDIB == NULL)
			return FALSE;
		m_pDib->Attach(hDIB);
		return TRUE;
	
	}
	
	return CDocument::OnOpenDocument(lpszPathName);
}

BOOL CRWJpegExpDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	//获取文件信息
	char drive[_MAX_DRIVE];   
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];   
	char ext[_MAX_EXT];
	_splitpath(lpszPathName, drive, dir, fname, ext);  
	//判断是否JPEG文件
	 if (! stricmp(ext, ".jpg") ||
			 ! stricmp(ext, ".jpe") ||
			 ! stricmp(ext, ".jpeg") ) // JPEG file
	{
		 //创建CFormatJpeg对话框
		CFormatJpeg JpegDlg;
		if (JpegDlg.DoModal() == IDOK)
		{
			//创建CJpeg类
			CJpeg jpeg;
			//将文件存储为JPEG格式
			return jpeg.Save(lpszPathName, m_pDib, JpegDlg.m_bColor, JpegDlg.m_nQuality);
	
		}
		return FALSE;
	}
	
	
	
	return CDocument::OnSaveDocument(lpszPathName);
}
