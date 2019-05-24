// headers
#include"DisplayManager.h"

//macros
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// global variable declarations
//for full screen of window
bool gbFullscreen = false;
DWORD dwStyle;
HWND ghwnd = NULL;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };
TCHAR gpszExitMessage[255]; //to display while exiting the window
bool gbDone = false;

// function prototype declarations
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);


// creates the display
void createDisplay(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLIne, int iCmdShow)
{
	// local variables
	WNDCLASSEX wndclass;
	TCHAR AppName[] = TEXT("window");
	TCHAR WinName[] = TEXT("Display");
	HWND hwnd;
	RECT rect;
	
	//initialize window class
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(hInstance, TEXT("MYICON")); //it must be handle to an icon resource
	wndclass.hCursor = LoadCursor(hInstance, TEXT("MYCURSOR"));
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = AppName;
	//this is the icon which is shown on left hand side of the window title
	wndclass.hIconSm = LoadIcon(
		hInstance, //loads default
		TEXT("MYICONSM") //it must be handle to an icon resource
	);
	//initialization complete

	RegisterClassEx(&wndclass); //Register Class

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


	//create window in memory
	hwnd = CreateWindow(
		AppName,
		WinName,
		WS_OVERLAPPEDWINDOW,
		x,
		y,
		WIN_WIDTH,
		WIN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
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

	ShowWindow(hwnd, iCmdShow); //Show Window
	UpdateWindow(hwnd); // color/paint the background of the window
}

// updates display each frame
void updateDisplay(MSG msg) 
{
	TranslateMessage(&msg);
	DispatchMessage(&msg);
	//translate and dispatch message
}

void closeDisplay(void) // closes display
{
	PostQuitMessage(0);
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


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_DESTROY:
		closeDisplay();
		break;
		//key press events
	case WM_KEYDOWN:
		switch (LOWORD(wParam))
		{
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
			/*wsprintf(gpszExitMessage, TEXT("Press OK to exit now..."));
			MessageBox(hwnd,
				gpszExitMessage, //message to display
				"Thanks For Visiting Me", //message box title
				MB_OK //message box style (OK/Cancel/YesNo/etc)
			);*/
			gbDone = true;
			break;
		}
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}