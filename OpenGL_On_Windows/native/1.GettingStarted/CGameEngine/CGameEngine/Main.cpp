/*
Created By - Rohit Muneshwar
Date - 15 Oct 2018
e-mail - rohit.muneshwar@ymail.com
program - Create Window with Full Screen capability in Win32
Icons and Cursor also changed

This program show the changes which we need to do in order to prepare our
window for OpenGL
1. add CS_OWNDC style to Wnd Class
2. use CreateWindowEx with WS_EX_APPWINDOW
3. Remove UpdateWindow and use SetForegroundWindow and SetFocus
4. Add call to initialize
5. In Gameloop, check if window is activated or not then accordingly exit
6. Handle WM_ACTIVATE message in WndProc
7. Handle WM_SIZE to get the resized window dimensions
8. Add resize and display functions
9. call display from WM_PAINT
10. Uninitialize from WM_CLOSE
11.Uninitialize the context data in WinMain

Steps For Single Buffer Application -
1. Code in initialization
Create PixelFormatDescriptor
Get OpenGLContext
Make it current context
Clear Color
2. Code in display
Clear Color
3. Code in resize
set viewport
4. Code in uninitialze
uninitialize data


Steps to Convert from Single Buffer Window to Double Buffer Window -
1. Remove WM_PAINT
2. Call Display in GameLoop
3. Add PFD_DOUBLEBUFFER in pfd
4. Remove glFlush() and add SwapBuffers() in display()
5. Remove ERASEBKGND
6. Remove resize() call from initialize()
*/
#include"OpenGLRenderer.h"


//link libraries
//#pragma comment is a compiler directive which indicates Visual C++ to leave a comment in the generated object file. The comment can then be read by the linker when it processes object files.
//it tells the linker to add the 'libname' library to the list of library dependencies, as if you had added it in the project properties at Linker->Input->Additional dependencies
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")

//entry-point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// local variables
	MSG msg; //to get the message from the pool

	init(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	
	//message loop
	/*
		Since GetMessage takes only Hardware Messages,
		we must use PeekMessage for OpenGL/Direct3D
	*/
	while (gbDone == false)
	{
		if (PeekMessage(&msg,
			NULL, //retrieves messages for any window that belongs to the calling thread
			0, //specifies the value of the first message in the rangle of messages to be examined
			0, //specifies the value of the last message in the rangle of messages to be examined
			PM_REMOVE //remove the message from the message queue; exception is WM_PAINT which is not removed
		))
		{
			if (msg.message == WM_QUIT)
			{
				gbDone = true;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			//if escape key is pressed while window is activated then exit the loop
			if (gbActiveWindow == true)
			{
				//play game here
				render();
				if (gbEscapeKeyPressed == true)
				{
					gbDone = true;
				}
			}
		}
	}
	//uninitialize the context data
	uninitializeOpenGL();
	return ((int)msg.wParam);
}




