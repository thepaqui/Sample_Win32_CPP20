/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 17:21:28 by thepaqui          #+#    #+#             */
/*   Updated: 2023/10/07 17:21:28 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UNICODE
# define UNICODE
#endif

#define BASIC_CLASS_NAME L"Sample Windows Class"
#define BASIC_WINDOW_NAME L"Sample Windows Window"

#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

//#include <cstdio> // for fprintf() debug

LRESULT CALLBACK	WindowProc(HWND, UINT, WPARAM, LPARAM);

// entry point for a Windows application
// aruments are in order:
// - handle to current instance of the app
// - handle to previous instance of the app
// - command line for the app
// - showing mode for the window
int WINAPI	WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int nWinMode)
{
	// define the window class name
	static const wchar_t	ClassName[] = BASIC_CLASS_NAME;

	// Create an instance of the window class structure
	// Every window must be associated with a window class! Even if there's only 1 window of its class!
	WNDCLASSEXW	wc;

	wc.cbSize			= sizeof(WNDCLASSEXW);						// always do that, it sets the right memory size
	wc.style			= CS_HREDRAW | CS_VREDRAW;					// automatically redraws the window while user resizes it vertically and horizontally
	wc.lpfnWndProc		= WindowProc;								// placeholder for handler of messages sent to window (pointer to window procedure function)
	wc.cbClsExtra		= 0;										// how many extra bytes to allocate for class (idk)
	wc.cbWndExtra		= 0;										// how many extra bytes to allocate for window (idk)
	wc.hInstance		= hInstance;								// identifies instance that contains the procedure
	wc.hIcon			= (HICON) LoadImage(NULL, IDI_APPLICATION,
								IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);	// icon
	wc.hIconSm			= (HICON) LoadImage(NULL, IDI_APPLICATION,
								IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);	// small icon
	wc.hCursor			= (HCURSOR) LoadImage(NULL, IDC_ARROW,
								IMAGE_CURSOR, 0, 0, LR_SHARED);		// cursor
	wc.hbrBackground	= CreateSolidBrush(RGB(82,35,0));			// sets default background brush to given color (if using a color macro, use (HBRUSH) (COLOR_WHATEVER+1))
	wc.lpszMenuName		= NULL;										// name of menu resource (???)
	wc.lpszClassName	= ClassName;								// sets the window class name

	// tries to register the window class to the OS. (a sort of lookup table)
	// If it fails, prints error box and returns failure.
	if (RegisterClassExW(&wc) == 0)
	{
		MessageBoxW(NULL, L"Can't register the main Class!", ClassName, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	// define the window's name
	static const wchar_t	WindowName[] = BASIC_WINDOW_NAME;

	// create the window
	HWND	hwnd = CreateWindowExW(0,	// Optional window styles
		ClassName, WindowName,			// ClassName identifies the window class we just registered. WindowName is our window's name.
		WS_OVERLAPPEDWINDOW,			// chooses a window style, here the window will have a title bar, a thick border, minimize and maximize buttons and a Windows menu
		CW_USEDEFAULT, CW_USEDEFAULT,	// initial position
		400, 300,						// initial size
		NULL,							// - handle to parent window
		NULL,							// - handle to a menu
		hInstance,						// - handle to instance of the module to be associated with the window
		NULL);							// - pointer to something weird idk

	// check if the window was created, exit if fail
	if (hwnd == NULL)
	{
		MessageBoxW(NULL, L"Unable to create the main window!", WindowName, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	srand(time(NULL));

	// show the window with specified showing options
	ShowWindow(hwnd, nWinMode);

	// sends WM_PAINT message if there are invalidated zones in the client area
	UpdateWindow(hwnd);

	static BOOL	bRet;
	static MSG	msg;

	// enter the main message loop
	// GetMessage takes the next message in the queue (the queue is handled by the OS)
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0) // GetMessage only returns 0 if it receives the message WM_QUIT
	{
		if (bRet == -1) // -1 is returned if an error has occurred, not for WM_QUIT (Example: referencing a window that does not exist anymore)
		{
			MessageBoxW(NULL, L"Unable to Continue!", WindowName, MB_OK | MB_ICONERROR);
			return E_FAIL;
		}
		TranslateMessage(&msg); // translates the message into its different components (actual message and its parameters if there are any)
		DispatchMessage(&msg); // sends the translated message to WindowProc, assigning the correct stuff to hwnd, message, wParam and lParam
	}

	return 0;
}

static void	PaintHandling(HWND hwnd, COLORREF BgColor, COLORREF TextColor, COLORREF TextBgColor)
{
	HDC			hdc;
	PAINTSTRUCT	ps;

	// BeginPaint tells ps what the zone to draw is
	// Overlapping windows that obscure a part of this window or things like that are handled by the OS
	hdc = BeginPaint(hwnd, &ps); // START OF PAINT INSTRUCTIONS

	// Fills the whole update area with a solid color
	FillRect(hdc, &ps.rcPaint, CreateSolidBrush(BgColor));

	// change text style
	SetBkColor(hdc, TextBgColor); // text background color
	SetTextColor(hdc, TextColor); // text color

	// create a new font object
	HFONT	myFont = CreateFontW(40, 0,					// character height and width (width of 0 means closest match for height value)
		0, 0,											// escapement and orientation
		FW_DONTCARE, FALSE, FALSE, FALSE,				// weight, italic, underlined, crossed out
		ANSI_CHARSET, OUT_TT_PRECIS,					// charset, output precision
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,			// clipping precision, output quality (smoothing)
		VARIABLE_PITCH | FF_MODERN, L"Comic Sans MS");	// pitch and family, name of the font
	HFONT	oldFont = NULL;
	if (myFont != NULL)
		oldFont = (HFONT) SelectObject(hdc, myFont); // replace the old font by the new one (store the old one to avoid leaks!)

	// draw some text centered in the client area
	RECT	rect;
	GetClientRect(hwnd, &rect);
	DrawTextW(hdc, L"I love you Binbows <3", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	if (myFont != NULL)
	{
		SelectObject(hdc, oldFont); // reselect the old font
		DeleteObject(myFont); // destroy the new font you created to avoid leaks
	}

	EndPaint(hwnd, &ps); // END OF PAINT INSTRUCTIONS
}

// processes the messages that Windows sends to the application
// NOTE: a message publication is when the message is queued up in the regular queue
// NOTE: a message sent is when the message ignores the queue and the OS calls WindowProc directly instead of through the loop in WinMain
LRESULT CALLBACK	WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
//	wchar_t			str[32];
	static COLORREF	BgColor		= RGB(rand() % 256, rand() % 256, rand() % 256);
	static COLORREF	TextColor	= RGB(rand() % 256, rand() % 256, rand() % 256);
	static COLORREF	TextBgColor	= RGB(rand() % 256, rand() % 256, rand() % 256);

	switch(message)
	{
/*
	case	WM_SYSKEYDOWN:
		swprintf(str, L"WM_SYSKEYDOWN: 0x%x\n", wParam);
		MessageBoxW(hwnd, str, L"Info", MB_OK | MB_ICONINFORMATION);
		break;
	case	WM_SYSCHAR:
		swprintf(str, L"WM_SYSCHAR: %c\n", (wchar_t)wParam);
		MessageBoxW(hwnd, str, L"Info", MB_OK | MB_ICONINFORMATION);
		break;
	case	WM_SYSKEYUP:
		swprintf(str, L"WM_SYSKEYUP: 0x%x\n", wParam);
		MessageBoxW(hwnd, str, L"Info", MB_OK | MB_ICONINFORMATION);
		break;
	case	WM_KEYDOWN:
		swprintf(str, L"WM_KEYDOWN: 0x%x\n", wParam);
		MessageBoxW(hwnd, str, L"Info", MB_OK | MB_ICONINFORMATION);
		break;
	case	WM_KEYUP:
		swprintf(str, L"WM_KEYUP: 0x%x\n", wParam);
		MessageBoxW(hwnd, str, L"Info", MB_OK | MB_ICONINFORMATION);
		break;
	case	WM_CHAR:
		swprintf(str, L"WM_CHAR: %c\n", (wchar_t)wParam);
		MessageBoxW(hwnd, str, L"Info", MB_OK | MB_ICONINFORMATION);
		break;
*/
	case	WM_CHAR:
		if ((wchar_t)wParam == L' ')
		{
			BgColor = RGB(rand() % 256, rand() % 256, rand() % 256);
			TextColor = RGB(rand() % 256, rand() % 256, rand() % 256);
			TextBgColor = RGB(rand() % 256, rand() % 256, rand() % 256);
			RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE); // Invalidates entire client area, forcing complete redraw with new colors
		}
		return S_OK;

	case	WM_PAINT:
		PaintHandling(hwnd, BgColor, TextColor, TextBgColor);
		return S_OK;

	// WM_CLOSE is sent when you click on the cross or Alt+F4 to quit
	case	WM_CLOSE:
		if (MessageBoxW(hwnd, L"Do you really want to quit?", BASIC_WINDOW_NAME, MB_YESNO | MB_ICONQUESTION) == IDYES)
			DestroyWindow(hwnd); // sends WM_DESTROY in the message queue
		return S_OK;

	case	WM_DESTROY:
		// exit the application
		PostQuitMessage(0); // sends WM_QUIT in the message queue
		return S_OK;
	}

	// let Windows process any unhandled messages
	return DefWindowProc(hwnd, message, wParam, lParam);
}
