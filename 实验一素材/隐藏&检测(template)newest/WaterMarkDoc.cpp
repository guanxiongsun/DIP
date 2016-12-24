// WaterMarkDoc.cpp : implementation of the CWaterMarkDoc class
//

#include "stdafx.h"
#include "WaterMark.h"

#include "WaterMarkDoc.h"
#include "mainfrm.h"
#include "dibapi.h"
#include "jpegapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWaterMarkDoc

IMPLEMENT_DYNCREATE(CWaterMarkDoc, CDocument)

BEGIN_MESSAGE_MAP(CWaterMarkDoc, CDocument)
	//{{AFX_MSG_MAP(CWaterMarkDoc)
	ON_BN_CLICKED(IDC_ADD_MARK, OnAddMark)
	ON_BN_CLICKED(IDC_ABSTRACT_MARK, OnAbstractMark)
	ON_COMMAND(ID_FILE_OPEN_DEST, OnFileOpenDest)
	ON_BN_CLICKED(IDC_DETECT, OnDetect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaterMarkDoc construction/destruction

CWaterMarkDoc::CWaterMarkDoc()
{
	// TODO: add one-time construction code here
	m_flag[0]=FALSE;
	m_flag[1]=FALSE;
	m_pBmp[0]=&m_sourceBmp;
	m_pBmp[1]=&m_resultBmp;
	m_fileType[1]=m_fileType[0]=NONE;
}

CWaterMarkDoc::~CWaterMarkDoc()
{
}

BOOL CWaterMarkDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CWaterMarkDoc serialization

void CWaterMarkDoc::Serialize(CArchive& ar)
{
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
// CWaterMarkDoc diagnostics

#ifdef _DEBUG
void CWaterMarkDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWaterMarkDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

////////
/////////////////////////////////////////////////////////////////////
// CWaterMarkDoc commands

BOOL CWaterMarkDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	CString s=lpszPathName;
	if(s.Right(3)==CString("jpg")||s.Right(3)==CString("JPG"))
		m_fileType[0]=JPG;
	else if(s.Right(3)==CString("bmp")||s.Right(3)==CString("BMP"))
		m_fileType[0]=BMP;
	else m_fileType[0]=NONE;
	switch(m_fileType[0])
	{
	case BMP:
	m_pBmp[0]->DeleteObject();
	m_flag[0]=m_sourceBmp.LoadBitmap(lpszPathName);
	break;
	case JPG:
	{
	m_srcJpg=lpszPathName;
	m_pBmp[0]->DeleteObject();
	CFile fjpg;
	fjpg.Open(lpszPathName,CFile::modeRead|CFile::shareDenyWrite);
	HDIB dib=ReadJPEGFile(fjpg);
	HANDLE handle=::DIBToBitmap(dib,NULL);
	m_flag[0]=m_sourceBmp.Attach(handle);
	fjpg.Close();
	}
	break;
	default:return FALSE;
	}
	return TRUE;
}


void CWaterMarkDoc::OnAddMark() 
{
	CFileDialog dlg(TRUE);
	if(dlg.DoModal()==IDOK)
	{
		BYTE *pData;
		int length;
		CFile f;
		CString s;
		s=dlg.GetPathName();
		f.Open(s.GetBuffer(1),CFile::modeRead|CFile::shareDenyWrite);
		length=f.GetLength();
		pData=new BYTE[length];
		f.ReadHuge(pData,length);
		f.Close();

	switch(m_fileType[0])
	{
	case BMP:
		m_flag[1]=FALSE;
		if(m_flag[0]==FALSE)
			return;
		m_resultBmp.DeleteObject();
		m_resultBmp.CreateDuplicate(&m_sourceBmp);

		m_flag[1]=m_resultBmp.HideDataPack(pData,length);
		m_fileType[1]=BMP;
	break;
	case JPG:
		m_jpg.NewImage(m_srcJpg.GetBuffer(1));
		m_jpg.ReadRawDCTData();
		
		m_jpg.HideDataPack(pData,length);

		m_jpg.WriteJpg("c:\\tmp.jpg");
		m_jpg.FreeImage();
	m_pBmp[1]->DeleteObject();
	CFile fjpg;
	fjpg.Open("c:\\tmp.jpg",CFile::modeRead|CFile::shareDenyWrite);
	HDIB dib=ReadJPEGFile(fjpg);
	HANDLE handle=::DIBToBitmap(dib,NULL);
	m_flag[1]=m_resultBmp.Attach(handle);
	fjpg.Close();
	m_fileType[1]=JPG;
	break;
	}
		delete pData;
	}

	this->UpdateAllViews(NULL);
}



BOOL CWaterMarkDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	switch(m_fileType[1])
	{
	case BMP:
		if(m_flag[1]==FALSE)return FALSE;		
		return m_resultBmp.SaveBitmap(lpszPathName);
		break;
	case JPG:
		//复制tmp.jpg到lpszPathName
		m_jpg.NewImage("c:\\tmp.jpg");
		m_jpg.ReadRawDCTData();
		//m_jpg.AdjustCoef();
		m_jpg.WriteJpg((char*)lpszPathName);
		m_jpg.FreeImage();

	break;
	default:return FALSE;
	}
	return TRUE;
}

void CWaterMarkDoc::OnAbstractMark() 
{
	CFileDialog dlg(FALSE);
	if(dlg.DoModal()==IDOK)
	{
		BYTE *pData;
		int length;

	switch(m_fileType[1])
	{
	case BMP:
	if(m_flag[1]==FALSE)return;
		length=m_resultBmp.GetDataCount();
		if(length<=0)return;
		pData=new BYTE[length];
		m_resultBmp.GetDataPack(pData,length);
	break;
	case JPG:
		m_jpg.NewImage(m_destJpg.GetBuffer(1));
		m_jpg.ReadRawDCTData();

		length=m_jpg.GetDataCount();
		if(length<=0)
		{
			m_jpg.FreeImage();
			return;
		}
		pData=new BYTE[length];
		m_jpg.GetDataPack(pData,length);		

		m_jpg.FreeImage();
		
	break;
	}
		CFile f;
		CString s;
		s=dlg.GetPathName();
		f.Open(s.GetBuffer(1),CFile::modeWrite|CFile::modeCreate);
		f.WriteHuge(pData,length);
		f.Close();
		delete pData;
	}
	return;
}

void CWaterMarkDoc::OnFileOpenDest() 
{
	// TODO: Add your command handler code here
	CFileDialog dlg(TRUE);
	if(dlg.DoModal()==IDOK)
	{
	CString s=dlg.GetPathName();
	if(s.Right(3)==CString("jpg")||s.Right(3)==CString("JPG"))
		m_fileType[1]=JPG;
	else if(s.Right(3)==CString("bmp")||s.Right(3)==CString("BMP"))
		m_fileType[1]=BMP;
	else m_fileType[1]=NONE;
	switch(m_fileType[1])
	{
	case BMP:
		m_resultBmp.DeleteObject();
		m_flag[1]=m_resultBmp.LoadBitmap(dlg.GetPathName());
	break;
	case JPG:
		m_destJpg=s;

	break;
	}
		this->UpdateAllViews(NULL);
	}

}



void CWaterMarkDoc::OnDetect() 
{
	float res=0;
	// TODO: Add your control notification handler code here
	switch(m_fileType[1])
	{
	case BMP:
	if(m_flag[1]==FALSE)return;
		res=m_resultBmp.Detect();
	break;
	case JPG:
		m_jpg.NewImage(m_destJpg.GetBuffer(1));
		m_jpg.ReadRawDCTData();
		res=m_jpg.Detect();
		m_jpg.FreeImage();
		
	break;
	}
	if(res>0.5)::AfxMessageBox("有隐藏信息");
	else ::AfxMessageBox("没有隐藏信息");
}
