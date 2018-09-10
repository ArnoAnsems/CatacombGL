// Copyright (C) 2018 Arno Ansems
// 
// This program is free software: you can redistribute it and/or modify 
// it under the terms of the GNU General Public License as published by 
// the Free Software Foundation, either version 3 of the License, or 
// (at your option) any later version. 
// 
// This program is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
// GNU General Public License for more details. 
// 
// You should have received a copy of the GNU General Public License 
// along with this program.  If not, see http://www.gnu.org/licenses/ 

//
// This source file is the main entry point of the CatacombGL executable. It contains the WinMain function.
// It creates the OpenGL window and an instance of the EngineCore class. Then it runs the main game loop.
//
// The code to create the OpenGL window was taken from "Your First Polygon", an OpenGL tutorial from Nehe Productions.
// http://nehe.gamedev.net/tutorial/your_first_polygon/13002/
//
// TODO: Move more code to the OS-independent Engine. The creation of the OpenGL window can be done via SDL2. 
//

// Windows specific includes
#include <windows.h>
#include <gl\gl.h>
#include <Shlwapi.h>
#include <time.h>
#include "RendererOpenGLWin32.h"
#include "SystemWin32.h"

#include "..\Engine\EngineCore.h"
#include "..\Engine\PlayerInput.h"
#include "..\Engine\GameDetection.h"

#include "..\Abyss\GameAbyss.h"
#include "..\Abyss\GameDetectionAbyss.h"

#include "..\..\ThirdParty\RefKeen\be_st.h"
#include "..\..\ThirdParty\RefKeen\id_sd.h"
#include "..\..\ThirdParty\SDL\include\SDL_mouse.h"

HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application

bool	active=TRUE;		// Window Active Flag Set To TRUE By Default

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc
RECT orgMouseClipArea;

EngineCore* engineCore;
IGame* game;
RendererOpenGLWin32 renderer;
PlayerInput playerInput;
SystemWin32 systemWin32;
int32_t cursorDisplayCount = 0;

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	renderer.SetWindowDimensions(width, height);
}

bool GetCatacombsPackGOGPath(std::string& path)
{
    CHAR gog_catacombs_path[256];
    DWORD dwType = 0;
    DWORD dwSize = sizeof(gog_catacombs_path);
    LSTATUS status = SHGetValueA(HKEY_LOCAL_MACHINE, "SOFTWARE\\GOG.COM\\GOGCATACOMBSPACK", "PATH", &dwType, gog_catacombs_path, &dwSize);
    const bool isGogCatacombsPathFound = ((status == ERROR_SUCCESS) && (dwType == REG_SZ));
    if (isGogCatacombsPathFound)
    {
        path.assign(std::string(gog_catacombs_path));
    }
    return isGogCatacombsPathFound;
}

GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
{
	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}
 
BOOL CreateGLWindow(int width, int height, int bits)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}
	
	dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
	dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

    const std::string windowTitle = "CatacombGL " + EngineCore::GetVersionInfo() + " [" + game->GetName() + "]";

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
                                windowTitle.c_str(),				// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		(BYTE)bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
    renderer.Setup();
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

    engineCore = new EngineCore(*game, systemWin32, playerInput);

    BE_ST_InitCommon();
    BE_ST_PrepareForGameStartup();

    SD_Startup();

	return TRUE;									// Success
}

// Function MapLeftRightKeys was first published on StackOverflow by Michael Burr:
// https://stackoverflow.com/questions/5681284/how-do-i-distinguish-between-left-and-right-keys-ctrl-and-alt
WPARAM MapLeftRightKeys(WPARAM vk, LPARAM lParam)
{
    WPARAM new_vk = vk;
    UINT scancode = (lParam & 0x00ff0000) >> 16;
    int extended = (lParam & 0x01000000) != 0;

    switch (vk) {
    case VK_SHIFT:
        new_vk = MapVirtualKey(scancode, MAPVK_VSC_TO_VK_EX);
        break;
    case VK_CONTROL:
        new_vk = extended ? VK_RCONTROL : VK_LCONTROL;
        break;
    case VK_MENU:
        new_vk = extended ? VK_RMENU : VK_LMENU;
        break;
    default:
        // not a key we map from generic to left/right specialized
        //  just return it.
        new_vk = vk;
        break;
    }

    return new_vk;
}

LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
			{
				active=TRUE;						// Program Is Active
			}
			else
			{
				active=FALSE;						// Program Is No Longer Active
			}

			return 0;								// Return To The Message Loop
		}

		case WM_SYSCOMMAND:
		{
			switch (wParam)
			{
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
					return 0;
			}
			break;
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}

		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
            playerInput.SetKeyPressed(MapLeftRightKeys(wParam, lParam), true);
			return 0;
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
			playerInput.SetKeyPressed(MapLeftRightKeys(wParam, lParam), false);					// If So, Mark It As FALSE
			return 0;
		}

        case WM_SYSKEYDOWN:
            {
                if (wParam == VK_F10)
                {
                    playerInput.SetKeyPressed(wParam, true);
                }
                if (wParam == VK_MENU)
                {
                    playerInput.SetKeyPressed(MapLeftRightKeys(wParam, lParam), true);
                }
                return 0;
            }

        case WM_SYSKEYUP:
            {
                if (wParam == VK_F10)
                {
                    playerInput.SetKeyPressed(wParam, false);
                }
                if (wParam == VK_MENU)
                {
                    playerInput.SetKeyPressed(MapLeftRightKeys(wParam, lParam), false);
                }
                return 0;
            }

		case WM_SIZE:								// Resize The OpenGL Window
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}

        case WM_INPUT: 
            {
                UINT dwSize;

                GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, 
                    sizeof(RAWINPUTHEADER));
                LPBYTE lpb = new BYTE[dwSize];
                if (lpb == NULL) 
                {
                    return 0;
                } 

                if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, 
                    sizeof(RAWINPUTHEADER)) != dwSize )
                {

                }

                RAWINPUT* raw = (RAWINPUT*)lpb;

                if (raw->header.dwType == RIM_TYPEMOUSE) 
                {
                    playerInput.SetMouseXPos(raw->data.mouse.lLastX);
                    playerInput.SetMouseYPos(raw->data.mouse.lLastY);
                    if (raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN)
                    {
                        playerInput.SetMouseButtonPressed(SDL_BUTTON_LEFT, true);
                    }
                    if (raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP)
                    {
                        playerInput.SetMouseButtonPressed(SDL_BUTTON_LEFT, false);
                    }
                    if (raw->data.mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN)
                    {
                        playerInput.SetMouseButtonPressed(SDL_BUTTON_MIDDLE, true);
                    }
                    if (raw->data.mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP)
                    {
                        playerInput.SetMouseButtonPressed(SDL_BUTTON_MIDDLE, false);
                    }
                    if (raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN)
                    {
                        playerInput.SetMouseButtonPressed(SDL_BUTTON_RIGHT, true);
                    }
                    if (raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP)
                    {
                        playerInput.SetMouseButtonPressed(SDL_BUTTON_RIGHT, false);
                    }
                    if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_4_DOWN)
                    {
                        playerInput.SetMouseButtonPressed(SDL_BUTTON_X1, true);
                    }
                    if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_4_UP)
                    {
                        playerInput.SetMouseButtonPressed(SDL_BUTTON_X1, false);
                    }
                    if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_5_DOWN)
                    {
                        playerInput.SetMouseButtonPressed(SDL_BUTTON_X2, true);
                    }
                    if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_5_UP)
                    {
                        playerInput.SetMouseButtonPressed(SDL_BUTTON_X2, false);
                    }
                } 

                delete[] lpb; 
                return 0;
            } 

	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}


int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure

    /* initialize random seed: */
    srand ((unsigned int)time(NULL));

    const uint8_t GameIdNotDetected = 0;
    const uint8_t GameIdCatacombAbyssv113 = 1;
    const uint8_t GameIdCatacombAbyssv124 = 2;
    GameDetection gameDetection;

    // Try to find shareware game files in local path
    if (gameDetection.GetDetectionReport(GameIdCatacombAbyssv113, ".\\", abyssFilesv113).score != 0)
    {
        // Try to find registered game files in GOG path
        std::string gogPath;
        if (GetCatacombsPackGOGPath(gogPath))
        {
            const std::string gogAbyssPath = gogPath + "Abyss\\";
            gameDetection.GetDetectionReport(GameIdCatacombAbyssv124, gogAbyssPath, abyssFilesv124);  
        }   

        // Try to find registered game files in local path
        if (gameDetection.GetBestMatch().score != 0)
        {
            gameDetection.GetDetectionReport(GameIdCatacombAbyssv124, ".\\", abyssFilesv124);
        }
    }
    
    const DetectionReport& report = gameDetection.GetBestMatch();
    if (report.score == 0)
    {
        game = new GameAbyss(report.gameId, report.folder, renderer);
    }
    else
    {
        const std::string errorMessage = "Failed to detect game files! Please make sure the Catacombs Pack [GOG.com] is correctly installed. Alternatively, the (shareware) game files can be placed in the same folder as CatacombGL.exe. Detailed info: " + report.infoString;
        MessageBox(NULL,errorMessage.c_str(),"CatacombGL ERROR",MB_OK | MB_ICONERROR);
        return 0;
    }

	// Create Our OpenGL Window
	if (!CreateGLWindow(1024,768,16))
	{
		return 0;									// Quit If Window Was Not Created
	}

    RAWINPUTDEVICE Rid[1];

    Rid[0].usUsagePage = 0x01; 
    Rid[0].usUsage = 0x02; 
    Rid[0].dwFlags = RIDEV_INPUTSINK; //0x0; //RIDEV_NOLEGACY;   // adds HID mouse and also ignores legacy mouse messages
    Rid[0].hwndTarget = hWnd;

    RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));
    GetClipCursor(&orgMouseClipArea);

    BOOL done = FALSE;								// Bool Variable To Exit Loop
	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
            if (!active || engineCore->Think())
            {
                done=TRUE;
            }
			else								// Not Time To Quit, Update Screen
			{
                playerInput.SetHasFocus(GetFocus() != NULL);
                engineCore->DrawScene(renderer);
				SwapBuffers(hDC);					// Swap Buffers (Double Buffering)
			}

            if (engineCore->RequiresMouseCapture())
            {
                RECT r;
                GetWindowRect( hWnd, &r);
                if (r.bottom - r.top > 60 && r.right - r.left > 45)
                {
                    r.top += 35;
                    r.bottom -= 20;
                    r.right -= 20;
                    r.left += 20;
                }
                ClipCursor( &r );
                while (cursorDisplayCount >= 0)
                {
                    cursorDisplayCount = ShowCursor(FALSE);
                }	
            }
            else
            {
                ClipCursor(&orgMouseClipArea);
                while (cursorDisplayCount < 0)
                {
                    cursorDisplayCount = ShowCursor(TRUE);
                }
            }
		}
	}

	// Shutdown
    SD_Shutdown();
    BE_ST_ShutdownAll();

	KillGLWindow();									// Kill The Window
    delete engineCore;
    delete game;
	return (msg.wParam);							// Exit The Program
}
