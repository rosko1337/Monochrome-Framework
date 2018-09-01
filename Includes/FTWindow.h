#ifndef FTWINDOW_H
#define FTWINDOW_H
#include <Windows.h>

#ifdef __cplusplus
extern "C" {
#endif 

#include "FTBitmask.h"

// Window Customization Flags
#define FT_MINIMIZABLE 1
#define FT_MAXIMIZABLE 2
#define FT_CLOSE_BUTTON 4
#define FT_HIDE_WINDOW_ICON 8
#define FT_BORDERLESS 16
#define FT_NON_RESIZABLE 32

// Window Customizations: Pre-defined Options
#define FT_THICK_BORDERLESS FT_HIDE_WINDOW_ICON | FT_BORDERLESS
#define FT_CLASSIC FT_CLOSE_BUTTON | FT_MAXIMIZABLE | FT_MINIMIZABLE

typedef struct FTWindow
{
	HWND hWnd;
	MSG msg;
	int width;
	int height;
} FTWindow;

// Creates a Win32 window and fills the FTWindow structure
FTWindow* FTCreateWindow(int width, int height, LPCWSTR title, LPCWSTR wndexClassName, LRESULT CALLBACK windowProc);

// Free window resources and delete the window
void FTDestroyWindow(FTWindow* window);

// Show/Open window
void FTShowWindow(FTWindow* window);

// Hide window
void FTHideWindow(FTWindow* window);

// Restores window size and view
void FTRestoreWindow(FTWindow* window);

// Brings specified window to focus
void FTFocusWindow(FTWindow* window);


/***** FUNCTIONS FOR SETTING WINDOW SPECS *****/

void FTSetWindowFlags(int flags);

#ifdef __cplusplus
}
#endif 

#endif // !FTWINDOW_H