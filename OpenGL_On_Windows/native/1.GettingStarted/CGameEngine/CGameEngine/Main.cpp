// header files
#include"DisplayManager.h"

// global variable declarations
extern bool gbDone;

// function prototype declarations
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

// entry-point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLIne, int iCmdShow)
{
	// local variables
	MSG msg;

	// code
	createDisplay(hInstance, hPrevInstance, lpszCmdLIne, iCmdShow); // create our display for rendering

	//message loop
	//get message first
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
			//play game here
		}
	}

	return (int)msg.wParam;
}