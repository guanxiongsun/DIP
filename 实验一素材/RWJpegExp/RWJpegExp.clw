; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CRWJpegExpView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "rwjpegexp.h"
LastPage=0

ClassCount=7
Class1=CChildFrame
Class2=CMainFrame
Class3=CRWJpegExpApp
Class4=CAboutDlg
Class5=CRWJpegExpDoc
Class6=CRWJpegExpView

ResourceCount=4
Resource1=IDD_ABOUTBOX
Resource2=IDR_RWJPEGTYPE
Resource3=IDR_MAINFRAME
Class7=CFormatJpeg
Resource4=IDD_DLG_JPEG

[CLS:CChildFrame]
Type=0
BaseClass=CMDIChildWnd
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
LastObject=ID_FILE_SAVE_AS

[CLS:CMainFrame]
Type=0
BaseClass=CMDIFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
LastObject=CMainFrame

[CLS:CRWJpegExpApp]
Type=0
BaseClass=CWinApp
HeaderFile=RWJpegExp.h
ImplementationFile=RWJpegExp.cpp
LastObject=CRWJpegExpApp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=RWJpegExp.cpp
ImplementationFile=RWJpegExp.cpp
LastObject=CAboutDlg

[CLS:CRWJpegExpDoc]
Type=0
BaseClass=CDocument
HeaderFile=RWJpegExpDoc.h
ImplementationFile=RWJpegExpDoc.cpp
LastObject=CRWJpegExpDoc

[CLS:CRWJpegExpView]
Type=0
BaseClass=CScrollView
HeaderFile=RWJpegExpView.h
ImplementationFile=RWJpegExpView.cpp
LastObject=ID_STEGANALYSIS_F5
Filter=C
VirtualFilter=VWC

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_RWJPEGTYPE]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_EDIT_UNDO
Command12=ID_EDIT_CUT
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=ID_VIEW_TOOLBAR
Command16=ID_VIEW_STATUS_BAR
Command17=ID_STEGANALYSIS_F5
Command18=ID_WINDOW_NEW
Command19=ID_WINDOW_CASCADE
Command20=ID_WINDOW_TILE_HORZ
Command21=ID_WINDOW_ARRANGE
Command22=ID_APP_ABOUT
CommandCount=22

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[DLG:IDD_DLG_JPEG]
Type=1
Class=CFormatJpeg
ControlCount=11
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_SLIDER1,msctls_trackbar32,1342242836
Control4=IDC_VALUE,static,1342308354
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,button,1342177287
Control8=IDC_RADIO1,button,1342177289
Control9=IDC_RADIO2,button,1342177289
Control10=IDC_STATIC,button,1342177287
Control11=IDC_STATIC,static,1342308352

[CLS:CFormatJpeg]
Type=0
HeaderFile=FormatJpeg.h
ImplementationFile=FormatJpeg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_RADIO2
VirtualFilter=dWC

