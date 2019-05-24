#ifndef _MY_DISPLAYMANAGER_H_
#define _MY_DISPLAYMANAGER_H_

#include<Windows.h>


// global-variable delcations
extern bool gbDone; //for game loop

void createDisplay(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLIne, int iCmdShow); // creates display

void updateDisplay(MSG msg); // updates display each frame

void closeDisplay(void); // closes display

void toggleFullscreen(void); // toggle fullscreen

#endif /* _MY_DISPLAYMANAGER_H_ */
