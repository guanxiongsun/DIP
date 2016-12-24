// test.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "resource.h"

#define MAX_LOADSTRING 100

#define NAME "DDExample1"
#define TITLE "Direct Draw Example 1"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <fstream.h>
#include <ddraw.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <io.h>
#include "cjpeg.h"

#define TIMER_ID        1
#define TIMER_RATE      30

void showAFile();
DWORD WINAPI demo(LPVOID lpparm);

LPDIRECTDRAW            lpDD;           // DirectDraw object
LPDIRECTDRAWSURFACE     lpDDSPrimary;   // DirectDraw primary surface
LPDIRECTDRAWSURFACE     lpDDSBack;      // DirectDraw back surface
DWORD RBitMask,GBitMask,BBitMask;
BOOL                    bActive;        // is application active?
BOOL                    bQuit = false;
HANDLE					thandle;
HWND                hwnd;

WORD (*RGBto16bit)(unsigned char r,unsigned char g,unsigned char b);

//BOOL initWindow( HINSTANCE hInstance, int nCmdShow );
BOOL initDDraw(void);
//long FAR PASCAL WindowProc( HWND hWnd, UINT message, 
//                            WPARAM wParam, LPARAM lParam );
void finiObjects( void );

time_t tt;
long frames,rate,k;



// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TEST, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_TEST);
 
//	if( !initWindow( hInstance, nCmdShow ) )
//    {    return FALSE;
//    }
	if(!initDDraw())
	{	 goto error;
	}

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
error:
	char buf[40];
	sprintf(buf, "初始化失败！");
    MessageBox( hwnd, buf, "ERROR", MB_OK );
	return FALSE;
}

WORD RGBto16bit565(unsigned char r,unsigned char g,unsigned char b)
{	return (((WORD)r<<8)&0xf800)|(((WORD)g<<3)&0x07e0)|((WORD)b>>3);
}

WORD RGBto16bit555(unsigned char r,unsigned char g,unsigned char b)
{	return (((WORD)r<<7)&0x7c00)|(((WORD)g<<2)&0x03e0)|((WORD)b>>3);
}

BOOL initDDraw(void)
{   DDSURFACEDESC       ddsd;
	DDSCAPS             ddscaps;
    HRESULT             ddrval;
    char                buf[256];
	ddrval = DirectDrawCreate( NULL, &lpDD, NULL );
    if( ddrval == DD_OK )
    {   // Get exclusive mode
        ddrval = lpDD->SetCooperativeLevel( hwnd,
                                DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN );
        if(ddrval == DD_OK )
        {   ddrval = lpDD->SetDisplayMode( 800,600, 16 );
            if( ddrval == DD_OK )
            {   // Create the primary surface with 1 back buffer
                ddsd.dwSize = sizeof( ddsd );
				ddsd.dwFlags = DDSD_CAPS | 
							   DDSD_BACKBUFFERCOUNT;
                ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE |
                                      DDSCAPS_FLIP | 
                                      DDSCAPS_COMPLEX;
                ddsd.dwBackBufferCount = 1;
                ddrval = lpDD->CreateSurface( &ddsd, &lpDDSPrimary, NULL );
                if( ddrval == DD_OK )
                {   ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
                    ddrval = lpDDSPrimary->GetAttachedSurface(&ddscaps, 
                                                          &lpDDSBack);
					if( ddrval == DD_OK )
					{	DDPIXELFORMAT pixelFormat;
						pixelFormat.dwSize = sizeof(DDPIXELFORMAT);
						lpDDSPrimary->GetPixelFormat(&pixelFormat);
						if((pixelFormat.dwFlags&DDPF_RGB)==0)
							goto error;
						RBitMask = pixelFormat.dwRBitMask;
						GBitMask = pixelFormat.dwGBitMask;
						BBitMask = pixelFormat.dwBBitMask;
						if(((WORD)GBitMask) == 0x07e0)
							RGBto16bit = RGBto16bit565;
						else
							RGBto16bit = RGBto16bit555;
						DWORD tID;
						if((thandle = CreateThread(NULL,0,demo,NULL,0,&tID)) != NULL)
							return TRUE;
					}
                }
			}
        }
    }
error:
    wsprintf(buf, "Direct Draw Init Failed (%08lx)\n", ddrval );
    MessageBox( hwnd, buf, "ERROR", MB_OK );
    finiObjects();
    DestroyWindow( hwnd );
    return FALSE;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_TEST);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_TEST;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);

	switch (message) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case IDM_EXIT:
				   DestroyWindow(hWnd);
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...
			RECT rt;
			GetClientRect(hWnd, &rt);
			DrawText(hdc, szHello, strlen(szHello), &rt, DT_CENTER);
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}

void finiObjects( void )
{
    if( lpDD != NULL )
    {
        if( lpDDSPrimary != NULL )
        {   lpDDSPrimary->Release();
            lpDDSPrimary = NULL;
        }
		lpDD->Release();
        lpDD = NULL;
    }
}

void showAFile()
{	static long f = -1;
	_finddata_t fileinfo;
	HDC hdc;
	DDSURFACEDESC       ddsd;
	HRESULT             ddrval;
	ddsd.dwSize = sizeof(ddsd);
	CJpeg jpg;
	if(!jpg)
		return;
re_search:
	if(f == -1)
	{	f = _findfirst("*.jp*g", &fileinfo);
		if(f == -1)
		{	DDBLTFX ddbltfx;
			ddbltfx.dwSize = sizeof(ddbltfx);
			if(((WORD)GBitMask) == 0x07e0)
				ddbltfx.dwFillColor = 0x8000;
			else
				ddbltfx.dwFillColor = 0x4000;
			lpDDSBack->Blt(NULL,NULL,NULL,DDBLT_COLORFILL|DDBLT_WAIT, &ddbltfx);
			if (lpDDSBack->GetDC(&hdc) == DD_OK)
            {	char cc[40];
				sprintf(cc,"没有找到jpeg文件！");
				SetBkColor( hdc, RGB( 128, 0, 0 ) );
				SetTextColor( hdc, RGB( 255, 255, 0 ) );
				TextOut( hdc, 220,240, cc, lstrlen(cc) );
                lpDDSBack->ReleaseDC(hdc);
            }
			goto flip;
		}
	}
	else
	{	if(_findnext(f,&fileinfo) != 0)
		{	f = -1;
			goto re_search;
		}
	}
	if(jpg.open(fileinfo.name,IDCT_INT) == FALSE)
	{	DDBLTFX ddbltfx;
		ddbltfx.dwSize = sizeof(ddbltfx);
		ddbltfx.dwFillColor = 0x0010;
		lpDDSBack->Blt(NULL,NULL,NULL,DDBLT_COLORFILL|DDBLT_WAIT, &ddbltfx);
		if (lpDDSBack->GetDC(&hdc) == DD_OK)
        {	char cc[150];
			if(strlen(fileinfo.name) < 100)
				sprintf(cc,"%s 不支持的jpeg文件格式！",fileinfo.name);
			else
				sprintf(cc,"不支持的jpeg文件格式！");
			SetBkColor( hdc, RGB( 0, 0, 128 ) );
			SetTextColor( hdc, RGB( 255, 255, 0 ) );
			TextOut( hdc, 220,240, cc, lstrlen(cc) );
            lpDDSBack->ReleaseDC(hdc);
        }
		goto flip;
	}
	else
	{	DDBLTFX ddbltfx;
		ddbltfx.dwSize = sizeof(ddbltfx);
		ddbltfx.dwFillColor = 0x0000;
		lpDDSBack->Blt(NULL,NULL,NULL,DDBLT_COLORFILL|DDBLT_WAIT, &ddbltfx);
		while ((ddrval = lpDDSBack->Lock(NULL, &ddsd, 0, NULL)) == DDERR_WASSTILLDRAWING)
			;
		if( ddrval == DD_OK )
		{	UINT imgX,imgY,dspX,dspY,dspWidth,dspHeight,j,k;
			JPEG_IMAGE_INFO info;
			BYTE *p1,*p2,*p3;
			WORD *p;
			jpg.getImageInfo(&info);
			if(info.imgWidth > 800)
				dspWidth = 800,dspX = 0,imgX = (info.imgWidth-800)/2;
			else
				dspWidth = info.imgWidth,imgX = 0,dspX = (800-info.imgWidth)/2;
			if(info.imgHeight > 600)
				dspHeight = 600,dspY = 0,imgY = (info.imgHeight-600)/2;
			else
				dspHeight = info.imgHeight,imgY = 0,dspY = (600-info.imgHeight)/2;
			p = (WORD*)ddsd.lpSurface;
			p1 = info.image[0];
			p2 = info.image[1];
			p3 = info.image[2];
			if(info.comNum == 3)
			{	p += 800*dspY+dspX;
				p1 += info.bufWidth*imgY+imgX;
				p2 += info.bufWidth*imgY+imgX;
				p3 += info.bufWidth*imgY+imgX;
				for(j=0;j<dspHeight;j++)
				{	for(k=0;k<dspWidth;k++)
					{	p[k] = RGBto16bit(p1[k],p2[k],p3[k]);
					}
					p += 800;
					p1 += info.bufWidth;
					p2 += info.bufWidth;
					p3 += info.bufWidth;
				}
			}
			else
			{	p += 800*dspY+dspX;
				p1 += info.bufWidth*imgY+imgX;
				for(j=0;j<dspHeight;j++)
				{	for(k=0;k<dspWidth;k++)
					{	p[k] = (*RGBto16bit)(p1[k],p1[k],p1[k]);
					}
					p += 800;
					p1 += info.bufWidth;
				}
			}
			lpDDSBack->Unlock(NULL);
		}
	}
flip:
	while( 1 )
	{
	    ddrval = lpDDSPrimary->Flip( NULL, 0 );
        if( ddrval == DD_OK )
	    {
		    break;
		}
		if( ddrval == DDERR_SURFACELOST )
		{
			ddrval = lpDDSPrimary->Restore();
			if( ddrval != DD_OK )
			{
				break;
			}
		}
		if( ddrval != DDERR_WASSTILLDRAWING )
		{
			break;
		}
	}
}

DWORD WINAPI demo(LPVOID lpparm)
{	static time_t time1 = 0,time2;
	while(1)
	{	if( bQuit )
			return -1;
		if( bActive )
		{	time(&time2);
			if(difftime(time2,time1) > 1)
			{	showAFile();
				time1 = time2;
			}
		}
	}
}
