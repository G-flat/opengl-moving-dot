#include <windows.h>
#include "rect.h"

#define EXIT_IF(cond) if (cond) ExitProcess(1)

#define SCREEN_WIDTH 780
#define SCREEN_HEIGHT 480
#define RECT_WIDTH 10
#define RECT_HEIGHT 10

#define WHITE 0xFFFFFF

GL_RECT dot;

// Callback for arrow keys
void onArrowKeyPress(WPARAM vkCode)
{
	switch (vkCode)
	{
		case VK_LEFT:	moveRect(&dot, -RECT_WIDTH / 2,                0, SCREEN_WIDTH, SCREEN_HEIGHT); break;
		case VK_RIGHT:	moveRect(&dot,  RECT_WIDTH / 2,                0, SCREEN_WIDTH, SCREEN_HEIGHT); break;
		case VK_DOWN:	moveRect(&dot,               0, -RECT_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT); break;
		case VK_UP:		moveRect(&dot,               0,  RECT_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT); break;
	}
}

// Set up the pixel format for the window
void setupPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd = {sizeof(PIXELFORMATDESCRIPTOR), 1,
										PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL,
										PFD_TYPE_RGBA,								// RGBA or palettes?
										32,											// Colour depth
										0, 0, 0, 0, 0, 0, 0, 0,						// Bitplanes in RGBA colour buffer
										0, 0, 0, 0, 0,								// Bitplanes in accumulation buffer
										24,											// z-axis buffer depth
										8,											// Stencil buffer depth
										0, 0, 0, 0, 0, 0};
	int pixel_format = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, pixel_format, &pfd);
	DescribePixelFormat(hdc, pixel_format, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
}

// Callback for the message loop
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HDC hdc;
	static HGLRC hglrc;
	static PAINTSTRUCT ps;

	switch (msg)
	{
		case WM_PAINT:
		{
			drawRect(dot.x_L, dot.y_T, dot.x_R, dot.y_B, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);

			// These 2 functions are necessary to clear WM_PAINT messages from the queue
			BeginPaint(hwnd, &ps);
			EndPaint(hwnd, &ps);
			break;
		}
		case WM_KEYDOWN:
		{
			if (BETWEEN(wParam, VK_LEFT, VK_DOWN))	// If an arrow key's pressed
			{
				onArrowKeyPress(wParam);
				PostMessage(hwnd, WM_PAINT, 0, 0);	// Re-paint the window
			}
			break;
		}
		case WM_SIZE:
		{
			glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
			PostMessage(hwnd, WM_PAINT, 0, 0);	// Re-paint the window
			break;
		}
		case WM_DESTROY:
		{
			wglMakeCurrent(hdc, NULL);		// De-select the OpenGL context
			wglDeleteContext(hglrc);		// Delete the context
			PostQuitMessage(0);
			break;
		}
		case WM_CLOSE:
		{
			DestroyWindow(hwnd);
			break;
		}
		case WM_CREATE:
		{						 
			hdc = GetDC(hwnd);				// Get the device context for the window
			setupPixelFormat(hdc);			// Set up the pixel format for the device context

			hglrc = wglCreateContext(hdc);	// Create an OpenGL context
			wglMakeCurrent(hdc, hglrc);		// Make the context current

			setRect(&dot, (SCREEN_WIDTH - RECT_WIDTH) / 2, (SCREEN_HEIGHT - RECT_HEIGHT) / 2, (SCREEN_WIDTH + RECT_WIDTH) / 2, (SCREEN_HEIGHT + RECT_HEIGHT) / 2);
			break;
		}
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

// Entry point (analogous to 'int main()')
int WINAPI WinMainCRTStartup()
{
	// Get the handle of the current process
	HMODULE hInstance = GetModuleHandleA(NULL);

	// Initialize the window class
	WNDCLASSEXA windowClass = {sizeof(WNDCLASSEX),
	                           CS_OWNDC | CS_HREDRAW | CS_VREDRAW,
	                           WindowProc,
	                           0, 0,
	                           hInstance,
	                           NULL,								// Handle to class icon
	                           LoadCursorA(NULL, IDC_ARROW),		// Handle to class cursor
	                           NULL,								// Handle to background brush
	                           NULL,
	                           "a",
	                           NULL};								// Handle to small icon

	// Register the window class
	EXIT_IF(!RegisterClassExA(&windowClass));

	// Create the window
	HWND window = CreateWindowExA(0,								// Extended styles
		                          "a",								// Window class name
		                          "Moving dot",						// Window title
		                          WS_OVERLAPPEDWINDOW,				// Window style
		                          CW_USEDEFAULT, CW_USEDEFAULT,		// Position
								  SCREEN_WIDTH, SCREEN_HEIGHT,		// Size
		                          NULL, NULL, hInstance, NULL);
	EXIT_IF(!window);

	// Show the window
	ShowWindow(window, SW_SHOW);
	UpdateWindow(window);

	// Run the message loop
	MSG msg = {0};
	BOOL ret;
	while ((ret = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (ret == -1)	// Error!
			break;
		TranslateMessage(&msg);
		DispatchMessage(&msg); 
	}

	// Stop the program (analogous to 'return 0')
	ExitProcess(0);
}