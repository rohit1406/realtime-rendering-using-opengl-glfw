#include"DisplayManager.h"

//global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//global variables
bool gbActiveWindow = false;
bool gbEscapeKeyPressed = false;
bool gbDone = false;
bool gbFullscreen = false;

//bool gbDone = false; //for game loop
TCHAR gpszExitMessage[255]; //to display while exiting the window
TCHAR gpszWindowTitle[] = TEXT("OpenGL - Double Buffer Window"); //Window Title

//for full screen of window
//bool gbFullscreen = false;
DWORD dwStyle;
HWND ghwnd = NULL;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };

//single buffer changes
HDC ghdc = NULL;
HGLRC ghrc = NULL;

float gMixParam = 0.0f;

//window procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//function prototype declaration
	void toggleFullscreen(void);
	void resize(int, int);
	void uninitializeWindow(void);

	//local variables

	//code
	switch (iMsg)
	{
	case WM_ACTIVATE://check if window is activated
		//HIWORD(wParam) tells whether window is active or inactive
		if (HIWORD(wParam) == 0)
		{
			gbActiveWindow = true;
		}
		else
		{
			gbActiveWindow = false;
		}
		break;
	case WM_SIZE: //when resize happens
		//LOWORD(lParam) gives us width and HIWORD(lParam) gives us height
		resize(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_DESTROY: //this is mandatory message which needs to be handled
		//post WM_QUIT message with status 0
		PostQuitMessage(0);
		break;

		//By Law, windows has given permission only to WM_PAINT to paint our window. By handling this message, we are telling that my window will be paint by someone else
		/*case WM_ERASEBKGND: //this message is handled for discipline;
			return (0);
			break;*/ //this is not needed for double buffering
			//key press events
	case WM_KEYDOWN:
		switch (LOWORD(wParam))
		{
		case 0x52: //R

			break;
		case 0x47: //G

			break;
		case 0x42: //B

			break;
		case 0x43: //C

			break;
		case 0x4D: //M

			break;
		case 0x59: //Y

			break;
		case 0x4B: //K

			break;
		case 0x57: //W

			break;
			//full screen
		case 0x46: //f
			if (gbFullscreen == true)
			{
				toggleFullscreen();
				gbFullscreen = false;
			}
			else
			{
				toggleFullscreen();
				gbFullscreen = true;
			}
			break;
			//exit on escape
		case VK_ESCAPE:
			gbEscapeKeyPressed = true;
			/*wsprintf(gpszExitMessage, TEXT("Press OK to exit now..."));
			MessageBox(hwnd,
				gpszExitMessage, //message to display
				"Thanks For Visiting Me", //message box title
				MB_OK //message box style (OK/Cancel/YesNo/etc)
			);*/
			gbDone = true;
			break;

		case VK_UP:
			gMixParam += 0.1f;
			if (gMixParam > 1.0f)
			{
				gMixParam = 1.0f;
			}
			break;

		case VK_DOWN:
			gMixParam -= 0.1f;
			if (gMixParam < 0.0f)
			{
				gMixParam = 0.0f;
			}
			break;
		default:

			break;
		}
		break;
	case WM_CLOSE: //while closing the window
		uninitializeWindow();
		break;

	}

	return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void createWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//function prototype declaration
	void display(void);

	//local variables
	WNDCLASSEX wndclass; //window class
	HWND hwnd; //handle to my window
	TCHAR lpszAppName[] = TEXT("Windows");
	RECT rect;

	//code
	//steps for creating the window
	//step1 - initialise WNDCLASSEX - 12 members to initialize
	wndclass.cbSize = sizeof(WNDCLASSEX); //count of byte size of this structure

	//Redraws the entire window if a movement or size adjustment changes the width/height of the client area
	wndclass.style = CS_HREDRAW | CS_VREDRAW //class style - horizontal/verticle
		| CS_OWNDC; //allocate unique device context for each window in the class; Gives me some DC which is non-purgeable or non-discardable

	wndclass.cbClsExtra = 0; //no. of extra bytes to allocate following the window-class structure

	wndclass.cbWndExtra = 0; //no. of extra bytes to allocate following the window instance

	wndclass.lpszClassName = lpszAppName; //Window class name

	wndclass.lpszMenuName = NULL; //resource name of the class menu

	//function name is it's address
	wndclass.lpfnWndProc = WndProc; //pointer to the window procedure

	wndclass.hInstance = hInstance; //handle to the instance that contains the window procedure for the class

	//this member can be handle to the physical brush to be used for painting the background or it can be a color value
	wndclass.hbrBackground = NULL;//(HBRUSH) GetStockObject(WHITE_BRUSH);

	//providing second param null loads default icon
	wndclass.hIcon = LoadIcon(hInstance, TEXT("MYICON")); //it must be handle to an icon resource

	/*wndclass.hIconSm = LoadIcon(
						NULL, //loads default
						IDI_APPLICATION //default macro - ID of Icon For Application
						);*/

						//this is the icon which is shown on left hand side of the window title
	wndclass.hIconSm = LoadIcon(
		hInstance, //loads default
		TEXT("MYICONSM") //it must be handle to an icon resource
	);

	//wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hCursor = LoadCursor(hInstance, TEXT("MYCURSOR"));

	//step 2 - Register the initialised window class
	RegisterClassEx(&wndclass);

	//step 3 - create the window in memory

	//position the window to center
	//GetSystemMetrics can also be used to retrieve the dimentions of the window display
	SystemParametersInfo(
		//specifies the system-wide parameters to query or set
		SPI_GETWORKAREA, //retrieve the size of the work area on the primary screen
		0, //this depends on uiAction i.e. above parameter
		&rect, //buffer which gets filled with values
		0 //specifies whether user profile is to be updated
	);
	int x = ((rect.right - rect.left) / 2) - (WIN_WIDTH / 2);
	int y = ((rect.bottom - rect.top) / 2) - (WIN_HEIGHT / 2);

	hwnd = CreateWindowEx(
		WS_EX_APPWINDOW, //forces the top-level window on the taskbar when window is visible
		lpszAppName, //window class name
		gpszWindowTitle, //window name
		//WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_SYSMENU | WS_MINIMISEBOX | WS_MAXIMISEBOX
		WS_OVERLAPPEDWINDOW //specifies style of the window being created
		| WS_CLIPCHILDREN //excludes the area occupied by child windows when drawing occurs within parent window
		| WS_CLIPSIBLINGS //clip child windows relative to each other
		| WS_VISIBLE, //creates a window that is initially visible
		x, //horizontal position of window
		y,// vertical position of the window
		WIN_WIDTH, //width of the window
		WIN_HEIGHT, //height of the window
		NULL, //handle to the parent window of the window being created
		NULL, //handle to menu or child window identifier
		hInstance, //handle to the instance of the module to be associated with the window
		NULL //long ptr to the value to be passed to the window
	);

	ghwnd = hwnd;
	//error checking for window
	if (hwnd == NULL)
	{
		//show message box
		MessageBox(NULL,
			TEXT("Window Not Created"), //text
			TEXT("Error..."), //caption
			0 //specifies the contents and beharior of the dialog box
		);
		exit(0); //normal exit the application
	}

	//step 4 - show window on desktop
	ShowWindow(
		hwnd, //handle to window
		nCmdShow //show state; maximized, minimized, hide, show, etc
	);


	//step 5 - color/paint the background of the window
	//Paint the Window Background by using brush provided in wnd class using UpdateWindow()
	//it paints the entire client area by sending WM_PAINT message
	//when part of the window needs to be painted then we can call InvalidateRect()
	//UpdateWindow(hwnd);
	//instead of UpdateWindow(), using below
	//SetForegroundWindow puts the thread that created the window into foreground and activates the window
	//by maximizing the thread priority, keyboard input is directed to the window
	SetForegroundWindow(hwnd);
	SetFocus(hwnd); //sets the keyboard focus to the specified window
}

void toggleFullscreen(void)
{
	//variable declaration
	HMONITOR hMonitor;

	//code
	//check if already fullscreen or not; if not then
	if (gbFullscreen == false)
	{
		//get current window style
		dwStyle = GetWindowLong(ghwnd, //handle to window
			GWL_STYLE //get style of the window; GWL - Get Window Long
		);

		//check if it has WS_OVERLAPPEDWINDOW; & multiplies the dwStyle
		if (dwStyle & WS_OVERLAPPEDWINDOW)
		{
			//evaluate window placement
			bool lbWindowPlacement = GetWindowPlacement(ghwnd, &wpPrev);

			//get handle to monitor
			hMonitor = MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY);

			//get monitor information
			MONITORINFO mi;
			mi.cbSize = sizeof(MONITORINFO);
			bool lbMonitorInfo = GetMonitorInfo(hMonitor, &mi);

			//check if we have window placement data and monitor info data successfully got or not
			if (lbWindowPlacement && lbMonitorInfo)
			{
				//set window style accordingly
				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);

				//set window position accordingly
				SetWindowPos(ghwnd, //handle to window 
					HWND_TOP, //places the window at the top of the z order
					mi.rcMonitor.left, //x coordinate of upper left of corner
					mi.rcMonitor.top, //y coordinate of upper left of corner
					(mi.rcMonitor.right - mi.rcMonitor.left), //x coordinate of lower right of corner
					(mi.rcMonitor.bottom - mi.rcMonitor.top), //y coordinate of lower right of corner
					SWP_NOZORDER //retains the current z order and ignores hWndInsertAfter parameter
					| SWP_FRAMECHANGED //Sends WM_NCCALCSIZE message to the window
				);
			}
		}
		//hide the cursor
		ShowCursor(FALSE);
	}
	else //if already in full screen
	{
		//set overlappedwindow style
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);

		//set earlier window placement
		SetWindowPlacement(ghwnd, &wpPrev);

		//set window position
		SetWindowPos(ghwnd,
			HWND_TOP, //places the window at the top of the z order
			0, //because it is
			0, //taken care by
			0, //SetWindowPlacement
			0,
			SWP_NOMOVE //retains the current position and ignores the x and y parameter
			| SWP_NOSIZE //retains the current size and ignores the cx and cy parameters
			| SWP_NOOWNERZORDER //does not change the positio in the Z order of the owner window
			| SWP_NOZORDER //retains the current z order and ignores the hWndInsertAfter parameter
			| SWP_FRAMECHANGED //Sends WM_NCCALCSIZE [non client calculate size] message to the window
		);

		//show cursor now
		ShowCursor(TRUE);
	}
}


void resize(int width, int height)
{
	if (height == 0)
	{
		height = 1;
	}
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

void uninitializeWindow(void)
{
	if (gbFullscreen == true)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);

		ShowCursor(TRUE);

	}
}