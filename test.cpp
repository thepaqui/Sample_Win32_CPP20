/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 18:43:14 by thepaqui          #+#    #+#             */
/*   Updated: 2023/10/06 18:43:14 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// entry point for a Windows application =======================================
// aruments are in order:
// - handle to current instance of the app
// - handle to previous instance of the app
// - command line for the app
// - a mode of showing the window
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int nWinMode)
{
	// define the window class name
	static const TCHAR	szAppName[] = TEXT("MinimalWindowsApp");

	// Create an instance of the window class structure
	// Every window must be associated with a window class! Even if there's only 1 window!
	WNDCLASSEX wc;

	wc.cbSize			= sizeof(WNDCLASSEX);						// always do that, it sets the right size
	wc.style			= CS_HREDRAW | CS_VREDRAW;					// redraws the windiw while user resizes it vertically and horizontally
	wc.lpfnWndProc		= WndProc;									// placeholder for handler of messages sent to window (pointer to window procedure)
	wc.cbClsExtra		= 0;										// how many extra bytes to allocate for class (idk)
	wc.cbWndExtra		= 0;										// how many extra bytes to allocate for window (idk)
	wc.hInstance		= hInstance;								// identifies instance that contains the procedure
	wc.hIcon			= (HICON) LoadImage(NULL, IDI_APPLICATION,
								IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);	// icon
	wc.hIconSm			= (HICON) LoadImage(NULL, IDI_APPLICATION,
								IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);	// small icon
	wc.hCursor			= (HCURSOR) LoadImage(NULL, IDC_ARROW,
								IMAGE_CURSOR, 0, 0, LR_SHARED);		// cursor
	wc.hbrBackground	= (HBRUSH) (COLOR_WINDOW + 1);				// sets background brush to given color (always increment color by 1)
	wc.lpszMenuName		= NULL;										// name of menu resource (???)
	wc.lpszClassName	= szAppName;								// sets the window class name

	// tries to register the window class to the OS.
	// If it fails, prints error box and returns failure.
	if (RegisterClassEx(&wc) == 0)
	{
		MessageBox(NULL, TEXT("Can't Register the Window Class!"), szAppName, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	// define the application title
	static const TCHAR szAppTitle[] = TEXT("Win32 API Skeletal Application");

	// create the window
	HWND hwnd = CreateWindow(szAppName, szAppTitle,					// szAppName identifies the window class we just registered. szAppTitle is our window's name.
									 WS_OVERLAPPEDWINDOW,			// chooses a window style, here the window will have a title bar, a thick border, minimize and maximize buttons and a Windows menu
									 CW_USEDEFAULT, CW_USEDEFAULT,	// initial position
									 CW_USEDEFAULT, CW_USEDEFAULT,	// initial size
									 NULL, NULL, hInstance, NULL);	// in order:
									 								// - handle to parent window
																	// - handle to a menu or a child window
																	// - handle to instance of the module to be associated with the window
																	// - pointer to something weird idk

	// check if the window was created, exit if fail
	if (hwnd == NULL)
	{
		MessageBox(NULL, TEXT("Unable to Create the Main Window!"), szAppName, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	// show and update the window
	ShowWindow(hwnd, nWinMode);
	UpdateWindow(hwnd);

	static BOOL bRet;
	static MSG  msg;

	// enter the main message loop
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0) // 0 = WM_QUIT
	{
		// check for error
		if (bRet == -1)
		{
			// for this app simply report error and exit
			MessageBox(NULL, TEXT("Unable to Continue!"), szAppName, MB_OK | MB_ICONERROR);
			return E_FAIL;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	// the app is done, exit normally, return control to Windows
	return (int) msg.wParam;
}


// processes the messages that Windows sends to the application ================
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// choose which Windows messages you want to use
	switch(message)
	{
	case WM_PAINT:
		HDC			hdc;
		PAINTSTRUCT ps;
		hdc = BeginPaint(hwnd, &ps);

		// draw some text centered in the client area
		RECT rect;
		GetClientRect(hwnd, &rect);
		DrawText(hdc, TEXT("Fuck you Windows!"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		EndPaint(hwnd, &ps);
		return S_OK;

	case WM_DESTROY:
		// exit the application
		PostQuitMessage(0);
		return S_OK;
	}

	// let Windows process any unhandled messages
	return DefWindowProc(hwnd, message, wParam, lParam);
}
