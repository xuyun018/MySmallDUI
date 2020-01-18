// TestUI.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

#include <tchar.h>
//---------------------------------------------------------------------------
#pragma comment(linker,"/ALIGN:4096")
#pragma comment(linker,"/ENTRY:NewMain")
//---------------------------------------------------------------------------
#define FORM_WIDTH														500
#define FORM_HEIGHT														300
//---------------------------------------------------------------------------
typedef struct tagBACKGROUND_BITMAP
{
	HDC hdc;

	// background
	BITMAPINFO bi;
	// button
	BITMAPINFO bis[3];

	HGLOBAL hobject;

	// background
	HBITMAP hbitmap;
	// button
	HBITMAP hbitmaps[3];
	RECT rectangles[3];

	// background
	LPVOID lpbits;
	LPVOID lpbitss[3];

	UINT buttonindex0;
	UINT buttonindex1;
}BACKGROUND_BITMAP, *PBACKGROUND_BITMAP;
//---------------------------------------------------------------------------
PBACKGROUND_BITMAP pbb;
//---------------------------------------------------------------------------
inline LPVOID XYAlloc(UINT size)
{
	return(HeapAlloc(GetProcessHeap(), 0, size));
}
inline VOID XYFree(LPVOID lpdata)
{
	HeapFree(GetProcessHeap(), 0, lpdata);
}

VOID DrawLines(const BITMAPINFOHEADER *pbih, LPVOID lpbits, LONG height, COLORREF color)
{
	LONG x;
	LONG y;
	DWORD rowsize;
	DWORD b;
	LPBYTE p0;
	LPBYTE p1;
	BYTE red;
	BYTE green;
	BYTE blue;

	red = GetRValue(color);
	green = GetGValue(color);
	blue = GetBValue(color);

	rowsize=((pbih->biWidth * 24 + 31)& ~31) >> 3;

	p0 = (LPBYTE)lpbits;

	p0 += (pbih->biHeight - 2) * rowsize;

	for (y = 0; y < height - 2; y++)
	{
		p1 = p0;
		p1 += 3;

		for (x = 1; x < pbih->biWidth; x++)
		{
			b = p1[0];
			p1[0] = (b * y + blue * (height - y)) / height;

			b = p1[1];
			p1[1] = (b * y + green * (height - y)) / height;

			b = p1[2];
			p1[2] = (b * y + red * (height - y)) / height;

			p1 += 3;
		}

		p0 -= rowsize;
	}
}
VOID DrawEdge(const BITMAPINFOHEADER *pbih, LPVOID lpbits, COLORREF color)
{
	UINT i;
	LONG y;
	DWORD rowsize;
	DWORD b;
	LPBYTE p0;
	LPBYTE p1;
	BYTE red;
	BYTE green;
	BYTE blue;
	
	red = GetRValue(color);
	green = GetGValue(color);
	blue = GetBValue(color);
	
	rowsize=((pbih->biWidth*24+31)&~31)>>3;
	
	p0 = (LPBYTE)lpbits;
	
	p0 += (pbih->biHeight-1)*rowsize;
	
	for (y = 0; y < pbih->biHeight; y++)
	{
		p1 = p0;
		p1 += 3;
		
		for (i = 0; i < 2; i++)
		{
			b = p1[0];
			p1[0] = (b * y + blue * (pbih->biHeight - y)) / pbih->biHeight;
			
			b = p1[1];
			p1[1] = (b * y + green * (pbih->biHeight - y)) / pbih->biHeight;
			
			b = p1[2];
			p1[2] = (b * y + red * (pbih->biHeight - y)) / pbih->biHeight;

			p1 += 3 * (pbih->biWidth - 2);
		}
		
		p0 -= rowsize;
	}
}
VOID DrawEdges(const BITMAPINFOHEADER *pbih, LPVOID lpbits,const RECT *rectangles, LONG width, LONG height, COLORREF color)
{
	UINT i;
	LONG y;
	DWORD rowsize;
	DWORD b;
	LPBYTE p0;
	LPBYTE p1;
	BYTE red;
	BYTE green;
	BYTE blue;

	red = GetRValue(color);
	green = GetGValue(color);
	blue = GetBValue(color);
	
	rowsize=((pbih->biWidth*24+31)&~31)>>3;
	
	p0 = (LPBYTE)lpbits;
	
	p0 += (pbih->biHeight-1)*rowsize;

	for (y = 0; y < height; y++)
	{
		for (i = 0; i < 3; i++)
		{
			p1 = p0;

			p1 += rectangles[i].left * 3;
			
			//for (j = 0; j < 2; j++)
			{
				b = p1[0];
				p1[0] = (b * y + blue * (height - y)) / height;
				
				b = p1[1];
				p1[1] = (b * y + green * (height - y)) / height;
				
				b = p1[2];
				p1[2] = (b * y + red * (height - y)) / height;
			}
		}

		//
		p1 += 3 * (width - 1);

		b = p1[0];
		p1[0] = (b * y + blue * (height - y)) / height;
		
		b = p1[1];
		p1[1] = (b * y + green * (height - y)) / height;
		
		b = p1[2];
		p1[2] = (b * y + red * (height - y)) / height;
		//

		p0 -= rowsize;
	}
}
VOID DrawHollowButtons(const RECT *rectangles, COLORREF color)
{
	const RECT *lprectangle;
	UINT i;
	UINT j;
	LONG left;
	LONG top;
	HGLOBAL hobject;
	HPEN hpen;
	HDC hdc = pbb->hdc;

	lprectangle = &rectangles[0];

	left = (lprectangle->right + lprectangle->left) >> 1;
	top = (lprectangle->bottom + lprectangle->top) >> 1;

	hpen = CreatePen(PS_SOLID, 1, color);
	hobject = SelectObject(hdc, hpen);
	j = 10;
	for (i = 0; i < 5; i++)
	{
		MoveToEx(hdc, left - 5, top - 3 + i, NULL);
		LineTo(hdc, left - 5 + j, top - 3 + i);

		left += 1;
		j -= 2;
	}

	lprectangle++;

	left = (lprectangle->right + lprectangle->left) >> 1;
	top = (lprectangle->bottom + lprectangle->top) >> 1;

	for (i = 0; i < 2; i++)
	{
		MoveToEx(hdc, left - 5, top + 2 + i, NULL);
		LineTo(hdc, left + 6, top + 2 + i);
	}

	lprectangle++;

	left = (lprectangle->right + lprectangle->left) >> 1;
	top = (lprectangle->bottom + lprectangle->top) >> 1;
	
	left -= 6;
	top -= 5;
	for (i = 0; i < 3; i++)
	{
		MoveToEx(hdc, left + i, top, NULL);
		LineTo(hdc, left + 9 + i, top + 9);

		MoveToEx(hdc, left + 8 + i, top, NULL);
		LineTo(hdc, left - 1 + i, top + 9);
	}

	SelectObject(hdc, hobject);
	DeleteObject(hpen);
}
VOID CopyButtons(HDC hdc0, HDC hdc1)
{
	const RECT *lprectangle;
	UINT i;
	HGLOBAL hobject;
	
	for (i = 0; i < 3; i++)
	{
		lprectangle = &pbb->rectangles[i];
		
		hobject = SelectObject(hdc0, pbb->hbitmaps[i]);
		//BitBlt(hdc0, lprectangle->left, lprectangle->top, lprectangle->right - lprectangle->left, lprectangle->bottom - lprectangle->top, hdc1, 0, 0, SRCCOPY);
		BitBlt(hdc0, 0, 0, lprectangle->right - lprectangle->left, lprectangle->bottom - lprectangle->top, hdc1, lprectangle->left, lprectangle->top, SRCCOPY);
		SelectObject(hdc0, hobject);
	}
}
VOID DrawButton(HDC hdc0, HDC hdc1, const RECT *rectangles, UINT buttonindex, LONG increment)
{
	const RECT *lprectangle;
	UINT i;
	UINT j;
	LONG left;
	LONG top;
	HGLOBAL hobject;
	HPEN hpen;
	COLORREF color = RGB(255, 255, 255);
	
	if (buttonindex != -1)
	{
		lprectangle = &rectangles[buttonindex];
		
		hobject = SelectObject(hdc1, pbb->hbitmaps[buttonindex]);
		BitBlt(hdc0, lprectangle->left, lprectangle->top, lprectangle->right - lprectangle->left, lprectangle->bottom - lprectangle->top, hdc1, 0, 0, SRCCOPY);
		SelectObject(hdc1, hobject);
		
		hpen = CreatePen(PS_SOLID, 1, color);
		hobject = SelectObject(hdc0, hpen);
		
		switch(buttonindex)
		{
		case 0:
			left = (lprectangle->right + lprectangle->left) >> 1;
			top = (lprectangle->bottom + lprectangle->top) >> 1;
			
			top -= increment;
			j = 10;
			for (i = 0; i < 5; i++)
			{
				MoveToEx(hdc0, left - 5, top - 3 + i, NULL);
				LineTo(hdc0, left - 5 + j, top - 3 + i);
				
				left += 1;
				j -= 2;
			}
			break;
		case 1:
			left = (lprectangle->right + lprectangle->left) >> 1;
			top = (lprectangle->bottom + lprectangle->top) >> 1;
			
			top -= increment;
			for (i = 0; i < 2; i++)
			{
				MoveToEx(hdc0, left - 5, top + 2 + i, NULL);
				LineTo(hdc0, left + 6, top + 2 + i);
			}
			break;
		case 2:
			left = (lprectangle->right + lprectangle->left) >> 1;
			top = (lprectangle->bottom + lprectangle->top) >> 1;
			
			top -= increment;
			left -= 6;
			top -= 5;
			for (i = 0; i < 3; i++)
			{
				MoveToEx(hdc0, left + i, top, NULL);
				LineTo(hdc0, left + 9 + i, top + 9);
				
				MoveToEx(hdc0, left + 8 + i, top, NULL);
				LineTo(hdc0, left - 1 + i, top + 9);
			}
			break;
		default:
			break;
		}
		
		SelectObject(hdc0, hobject);
		DeleteObject(hpen);
	}
}
VOID DrawButtons(HWND hwnd,const RECT *rectangles, UINT buttonindex0, UINT buttonindex1)
{
	const RECT *lprectangle;
	HDC hdc0;
	HDC hdc1;
	COLORREF color = RGB(255, 255, 255);

	hdc0 = GetDC(hwnd);
	hdc1 = CreateCompatibleDC(hdc0);

	if (buttonindex0 != -1)
	{
		lprectangle = &rectangles[buttonindex0];

		BitBlt(hdc0, lprectangle->left, lprectangle->top, lprectangle->right - lprectangle->left, lprectangle->bottom - lprectangle->top, pbb->hdc, lprectangle->left, lprectangle->top, SRCCOPY);
	}

	DrawButton(hdc0, hdc1, rectangles, buttonindex1, 1);

	DeleteDC(hdc1);
	ReleaseDC(hwnd, hdc0);
}

VOID SetRectangles(LONG cx, LONG cy, LONG width, LONG height)
{
	LPRECT lprectangle;
	LONG right;
	UINT i;

	lprectangle = &pbb->rectangles[2];

	lprectangle->right = cx - 6;
	lprectangle->left = lprectangle->right - width;
	lprectangle->top = 0;
	lprectangle->bottom = lprectangle->top + height;

	for (i = 0; i < 2; i++)
	{
		right = lprectangle->left;

		lprectangle--;

		lprectangle->right = right;
		lprectangle->left = lprectangle->right - width;
		lprectangle->top = 0;
		lprectangle->bottom = lprectangle->top + height;
	}
}

VOID CreateDeviceDependentBitmapSection(HDC hdc, UINT index, INT bitcount, LONG width, LONG height)
{
	LPBITMAPINFO pbi;
	LPBITMAPINFOHEADER pbih;
	HBITMAP *lphbitmap;
	LPVOID *lplpbits;

	if (index == -1)
	{
		pbi = &pbb->bi;

		lphbitmap = &pbb->hbitmap;
		lplpbits = &pbb->lpbits;
	}
	else
	{
		pbi = &pbb->bis[index];

		lphbitmap = &pbb->hbitmaps[index];
		lplpbits = &pbb->lpbitss[index];
	}

	pbih = &pbi->bmiHeader;
	pbih->biSize = sizeof(BITMAPINFOHEADER);
	pbih->biWidth = width;
	pbih->biHeight = height;
	pbih->biPlanes = 1;
	pbih->biBitCount = bitcount;
	pbih->biCompression = BI_RGB;
	pbih->biXPelsPerMeter = 0;
	pbih->biYPelsPerMeter = 0;
	pbih->biClrUsed = 0;
	pbih->biClrImportant = 0;
	pbih->biSizeImage = (((pbih->biWidth*pbih->biBitCount+31)&~31)>>3)*pbih->biHeight;

	*lphbitmap = CreateDIBSection(hdc, pbi, DIB_RGB_COLORS, lplpbits, NULL, NULL);

	if (index == -1)
	{
		pbb->hobject = SelectObject(pbb->hdc, *lphbitmap);
	}
}
VOID DeleteDeviceDependentBitmapSection(UINT index)
{
	HBITMAP hbitmap;
	
	if (index == -1)
	{
		hbitmap = pbb->hbitmap;

		SelectObject(pbb->hdc, pbb->hobject);
	}
	else
	{
		hbitmap = pbb->hbitmaps[index];
	}
	DeleteObject(hbitmap);
}

VOID WindowOnCreate(HWND hwnd,WPARAM wparam,LPARAM lparam)
{
	RECT rectangle;
	HRGN hregion;
	HBITMAP hbitmap;
	BITMAP bmp;
	COLORREF color;
	HPEN hpen;
	HBRUSH hbrush;
	HGLOBAL hobject;
	HDC hdc;
	HDC hdc0;
	HDC hdc1;
	LONG width;
	LONG height;
	LONG buttonwidth = 28;
	LONG buttonheight = 20;

	pbb->buttonindex0 = -1;
	pbb->buttonindex1 = -1;

	GetClientRect(hwnd, &rectangle);

	width = rectangle.right - rectangle.left;
	height = rectangle.bottom - rectangle.top;

	SetRectangles(width, height, buttonwidth, buttonheight);

	hregion = CreateRoundRectRgn(rectangle.left, rectangle.top, rectangle.right, rectangle.bottom, 4, 4);
	SetWindowRgn(hwnd, hregion, TRUE);
	DeleteObject(hregion);

	hdc = GetDC(hwnd);
	hdc0 = CreateCompatibleDC(hdc);
	pbb->hdc = hdc0;

	CreateDeviceDependentBitmapSection(hdc, -1, 24, width, height);
	CreateDeviceDependentBitmapSection(hdc, 0, 24, buttonwidth, buttonheight);
	CreateDeviceDependentBitmapSection(hdc, 1, 24, buttonwidth, buttonheight);
	CreateDeviceDependentBitmapSection(hdc, 2, 24, buttonwidth, buttonheight);

	color = RGB(0x4Eu, 0x5Eu, 0x68u);
	hpen = CreatePen(PS_SOLID, 1, color);
	hobject = SelectObject(hdc0, hpen);
	RoundRect(hdc0, 0, 0, width - 1, height - 1, 5, 5);
	SetRect(&rectangle, 1, 1, width - 2, height - 2);
	//hbrush = CreateSolidBrush(0xC0C0C0u);
	hbrush = CreateSolidBrush(0xFFFFFFu);
	FillRect(hdc0, &rectangle, hbrush);
	DeleteObject(hbrush);
	SelectObject(hdc0, hobject);
	DeleteObject(hpen);

	hbitmap = (HBITMAP)LoadImage(NULL, _T("bkimg.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (hbitmap)
	{
		hdc1 = CreateCompatibleDC(hdc0);
		SelectObject(hdc1, hbitmap);
		GetObject(hbitmap, sizeof(bmp), &bmp);
		StretchBlt(hdc0, 2, 2, width - 5, height - 5, hdc1, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
		DeleteDC(hdc1);

		DeleteObject(hbitmap);
	}
	else
	{
		/*
		color = RGB(0x4Eu, 0x5Eu, 0x68u);
		hpen = CreatePen(PS_SOLID, 1, color);
		hobject = SelectObject(hdc0, hpen);
		RoundRect(hdc0, 0, 0, width - 1, height - 1, 5, 5);
		SetRect(&rectangle, 1, 1, width - 2, height - 2);
		hbrush = CreateSolidBrush(0xC0C0C0u);
		FillRect(hdc0, &rectangle, hbrush);
		DeleteObject(hbrush);
		SelectObject(hdc0, hobject);
		DeleteObject(hpen);
		*/
	}

	SetPixel(pbb->hdc, 1, height -3, RGB(0, 0, 0));
	SetPixel(pbb->hdc, width - 3, height -3, RGB(0, 0, 0));

	//DrawLines(&pbb->bi.bmiHeader, pbb->lpbits, height * 2 / 5, RGB(0,204,122));
	DrawLines(&pbb->bi.bmiHeader, pbb->lpbits, height * 2 / 5, RGB(162,214,165));

	DrawEdges(&pbb->bi.bmiHeader, pbb->lpbits, pbb->rectangles, buttonwidth, buttonheight, RGB(0,0,0));

	hdc1 = CreateCompatibleDC(hdc0);
	CopyButtons(hdc1, hdc0);
	DeleteDC(hdc1);

	DrawLines(&pbb->bis[0].bmiHeader, pbb->lpbitss[0],  buttonheight, RGB(10,128,20));
	DrawLines(&pbb->bis[1].bmiHeader, pbb->lpbitss[1],  buttonheight, RGB(10,20,128));
	DrawLines(&pbb->bis[2].bmiHeader, pbb->lpbitss[2],  buttonheight, RGB(128,20,10));

	DrawHollowButtons(pbb->rectangles, RGB(255,255,255));

	/*
	DrawEdge(&pbb->bis[0].bmiHeader, pbb->lpbitss[0],  RGB(0,0,0));
	DrawEdge(&pbb->bis[1].bmiHeader, pbb->lpbitss[1],  RGB(0,0,0));
	DrawEdge(&pbb->bis[2].bmiHeader, pbb->lpbitss[2],  RGB(0,0,0));
	*/

    ReleaseDC(hwnd, hdc0);
}
VOID WindowOnClose(HWND hwnd,WPARAM wparam,LPARAM lparam)
{
	DeleteDeviceDependentBitmapSection(-1);

	DeleteDeviceDependentBitmapSection(0);
	DeleteDeviceDependentBitmapSection(1);
	DeleteDeviceDependentBitmapSection(2);

	DeleteDC(pbb->hdc);
}
VOID WindowOnPaint(HWND hwnd,WPARAM wparam,LPARAM lparam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	hdc = BeginPaint(hwnd, &ps);
	BitBlt(hdc, 0, 0, pbb->bi.bmiHeader.biWidth, pbb->bi.bmiHeader.biHeight, pbb->hdc, 0, 0, SRCCOPY);
	EndPaint(hwnd, &ps);
}
VOID WindowOnLeftButtonDown(HWND hwnd,WPARAM wparam,LPARAM lparam)
{
	HDC hdc0;
	HDC hdc1;

	if (pbb->buttonindex0 == -1)
	{
		ReleaseCapture();//释放窗体的鼠标焦点
		SendMessage(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
	}
	else
	{
		hdc0 = GetDC(hwnd);
		hdc1 = CreateCompatibleDC(hdc0);
		
		DrawButton(hdc0, hdc1, pbb->rectangles, pbb->buttonindex0, 0);

		DeleteDC(hdc1);
		ReleaseDC(hwnd, hdc0);
	}
}
VOID WindowOnLeftButtonUp(HWND hwnd,WPARAM wparam,LPARAM lparam)
{
	HDC hdc0;
	HDC hdc1;
	
	if (pbb->buttonindex0 != -1)
	{
		hdc0 = GetDC(hwnd);
		hdc1 = CreateCompatibleDC(hdc0);
		
		DrawButton(hdc0, hdc1, pbb->rectangles, pbb->buttonindex0, 1);
		
		DeleteDC(hdc1);
		ReleaseDC(hwnd, hdc0);

		if (pbb->buttonindex0 == pbb->buttonindex1)
		{
			switch(pbb->buttonindex0)
			{
			case 0:
				break;
			case 1:
				ShowWindow(hwnd, SW_MINIMIZE); 
				break;
			case 2:
				PostMessage(hwnd, WM_CLOSE, 0, 0);
				break;
			default:
				break;
			}
		}
	}
	else
	{
		//
	}
}
VOID OutputDebugValue(const TCHAR *string, int value0, int value1)
{
	TCHAR text[1024];
	UINT k;
	
	_tcscpy(text,string);
	k=_tcslen(text);
	text[k++]=_T(' ');
	text[k++]=_T('(');
	_itot(value0,text+k,10);
	k+=_tcslen(text+k);
	text[k++]=_T(',');
	_itot(value1,text+k,10);
	k+=_tcslen(text+k);
	text[k++]=_T(')');
	text[k]=_T('\0');
	OutputDebugString(text);
}
VOID WindowOnMouseLeave(HWND hwnd,WPARAM wparam,LPARAM lparam)
{
	/*
	HDC hdc0;
	HDC hdc1;
	
	if (pbb->buttonindex != -1)
	{
		hdc0 = GetDC(hwnd);
		hdc1 = CreateCompatibleDC(hdc0);
		
		DrawButton(hdc0, hdc1, pbb->rectangles, pbb->buttonindex, 0);
		
		DeleteDC(hdc1);
		ReleaseDC(hwnd, hdc0);

		OutputDebugValue(_T("Mouse Leave"), 0, 1);

		InvalidateRect(hwnd, NULL, 0);

		pbb->buttonindex = -1;
	}
	*/
}
VOID WindowOnMouseMove(HWND hwnd,WPARAM wparam,LPARAM lparam)
{
	const RECT *lprectangle;
	LONG x, y;
	UINT i;
	UINT buttonindex = -1;

	x = LOWORD(lparam);  // horizontal position of cursor 
	y = HIWORD(lparam);  // vertical position of cursor
	
	lprectangle = &pbb->rectangles[0];
	
	for (i = 0; i < 3; i++)
	{
		if (x >= lprectangle->left && y >= lprectangle->top && x < lprectangle->right && y < lprectangle->bottom)
		{
			buttonindex = i;

			break;
		}
		
		lprectangle++;
	}

	if (wparam == 0)
	{
		if (buttonindex != pbb->buttonindex0)
		{
			DrawButtons(hwnd, pbb->rectangles, pbb->buttonindex0, buttonindex);

			pbb->buttonindex0 = buttonindex;
		}

		SetCapture(hwnd);
	}

	if (buttonindex != pbb->buttonindex1)
	{
		pbb->buttonindex1 = buttonindex;
	}
}
UINT WindowOnNCHittest(HWND hwnd,WPARAM wparam,LPARAM lparam)
{
	RECT rectangle;
	const RECT *lprectangle;
	LONG x, y;
	UINT i;
	UINT buttonindex = -1;
	
	x = LOWORD(lparam);  // horizontal position of cursor 
	y = HIWORD(lparam);  // vertical position of cursor

	GetWindowRect(hwnd, &rectangle);

	x -= rectangle.left;
	y -= rectangle.top;

	lprectangle = &pbb->rectangles[0];

	//HDC hdc = GetDC(NULL);

	for (i = 0; i < 3; i++)
	{
		//Rectangle(hdc, lprectangle->left, lprectangle->top, lprectangle->right, lprectangle->bottom);

		if (x >= lprectangle->left && y >= lprectangle->top && x < lprectangle->right && y < lprectangle->bottom)
		{
			buttonindex = i;
			
			break;
		}
		
		lprectangle++;
	}

	//SetPixel(hdc, x, y, RGB(255, 0, 0));

	//ReleaseDC(NULL, hdc);

	return(buttonindex);
}

LRESULT CALLBACK WindowProc(HWND hwnd,UINT message,WPARAM wparam,LPARAM lparam)
{
	LRESULT result=0;
	
	switch(message)
	{
	case WM_CREATE:
		WindowOnCreate(hwnd,wparam,lparam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		WindowOnClose(hwnd,wparam,lparam);
		break;
	case WM_PAINT:
		WindowOnPaint(hwnd,wparam,lparam);
		break;
	case WM_LBUTTONDOWN:
		WindowOnLeftButtonDown(hwnd,wparam,lparam);
		break;
	case WM_LBUTTONUP:
		WindowOnLeftButtonUp(hwnd,wparam,lparam);
		break;
	//case WM_MOUSELEAVE:
	//	WindowOnMouseLeave(hwnd,wparam,lparam);
	//	break;
	case WM_MOUSEMOVE:
		WindowOnMouseMove(hwnd,wparam,lparam);
		break;
	case WM_NCHITTEST:
		//if (WindowOnNCHittest(hwnd,wparam,lparam) == -1)
		{
			//return(HTCAPTION);
		}
		break;
	default:
		break;
	}
	
	if(result==NULL)
	{
		result=DefWindowProc(hwnd,message,wparam,lparam);
	}
	
	return(result);
}

INT MyCreateWindow(LPVOID lparam)
{
	HINSTANCE hinstance;
	HWND hwnd;
	MSG msg;
	WNDCLASSEX winclass;
	ULONG width=500,height=300;
	
	hinstance = GetModuleHandle(NULL);
	
	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style = CS_DBLCLKS|CS_OWNDC;
	winclass.lpfnWndProc = WindowProc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = hinstance;
	winclass.hIcon = LoadIcon(hinstance,MAKEINTRESOURCE(100));
	winclass.hIconSm = LoadIcon(hinstance,MAKEINTRESOURCE(100));
	winclass.hCursor = LoadCursor(NULL,IDC_ARROW);
	//winclass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	winclass.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
	winclass.lpszMenuName = NULL;
	winclass.lpszClassName = _T("MyUIClass");
	
	RegisterClassEx(&winclass);
	
	hwnd=CreateWindowEx(0,
		winclass.lpszClassName,
		NULL,
		WS_POPUP|WS_VISIBLE,
		(GetSystemMetrics(SM_CXSCREEN)-width)/2,(GetSystemMetrics(SM_CYSCREEN)-height)/2,
		width,height,
		NULL,
		NULL,
		hinstance,
		NULL);
	
	if(hwnd!=NULL)
	{
		//ShowWindow(hwnd,SW_SHOW);
		
		while(GetMessage(&msg,NULL,0,0))
		{
			//TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	
	return(msg.wParam);
}

//int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
VOID APIENTRY NewMain(VOID)
{
	pbb = (PBACKGROUND_BITMAP)XYAlloc(sizeof(BACKGROUND_BITMAP));

	MyCreateWindow(NULL);

	XYFree((LPVOID)pbb);

	ExitProcess(0);
	//return 0;
}

