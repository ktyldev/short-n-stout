#include "Framework.h"

#define DEFAULT_FRAMERATE	30
#define DEFAULT_WIDTH		800
#define DEFAULT_HEIGHT		600

// Reference to ourselves - primarily used to access the message handler correctly
// This is initialised in the constructor
Framework *	_thisFramework = NULL;

// Forward declaration of our window procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(_In_	   HINSTANCE hInstance,
				  	  _In_opt_ HINSTANCE hPrevInstance,
					  _In_	   LPWSTR    lpCmdLine,
					  _In_	   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// We can only run if an instance of a class that inherits from Framework
	// has been created
	if (_thisFramework)
	{
		return _thisFramework->Run(hInstance, nCmdShow);
	}
	return -1;
}

Framework::Framework()
{
	_thisFramework = this;
}

Framework::~Framework()
{
}

int Framework::Run(HINSTANCE hInstance, int nCmdShow)
{
	int returnValue;

	_hInstance = hInstance;
	if (!InitialiseMainWindow(nCmdShow))
	{
		return -1;
	}
	if (!Initialise())
	{
		return -1;
	}
	returnValue = MainLoop();
	Shutdown();
	return returnValue;
}

// Main program loop.  

int Framework::MainLoop()
{
	MSG msg;
	HACCEL hAccelTable = LoadAccelerators(_hInstance, MAKEINTRESOURCE(IDC_RASTERISER));
	LARGE_INTEGER counterFrequency;
	LARGE_INTEGER nextTime;
	LARGE_INTEGER currentTime;
	LARGE_INTEGER lastTime;
	bool updateFlag = true;

	// Initialise timer
	QueryPerformanceFrequency(&counterFrequency);
	DWORD msPerFrame = (DWORD)(counterFrequency.QuadPart / DEFAULT_FRAMERATE);
	double timeFactor = 1.0 / counterFrequency.QuadPart;
	QueryPerformanceCounter(&nextTime);
	lastTime = nextTime;

	// Main message loop:
	msg.message = WM_NULL;
	while (msg.message != WM_QUIT)
	{
		if (updateFlag)
		{
			QueryPerformanceCounter(&currentTime);

			_timeSpan = (currentTime.QuadPart - lastTime.QuadPart) * timeFactor;

			lastTime = currentTime;
			Update(_bitmap);
			updateFlag = false;
		}
			
		QueryPerformanceCounter(&currentTime);
			
		// Is it time to render the frame?
		if (currentTime.QuadPart > nextTime.QuadPart && !PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE))
		{
			Render(_bitmap);
			// Make sure that the window gets repainted
			InvalidateRect(_hWnd, NULL, FALSE);
			// Set time for next frame
			nextTime.QuadPart += msPerFrame;
			// If we get more than a frame ahead, allow one to be dropped
			// Otherwise, we will never catch up if we let the error accumulate
			// and message handling will suffer
			if (nextTime.QuadPart < currentTime.QuadPart)
			{
				nextTime.QuadPart = currentTime.QuadPart + msPerFrame;
			}
			updateFlag = true;
		}
		else
		{
			if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
		}
	}
	return (int)msg.wParam;
}

// The initial required height of the window.  This can be overridden if a different initial height is required

unsigned int Framework::GetInitialWindowHeight() const
{
	return DEFAULT_HEIGHT;
}

// The initial required width of the window.  This can be overridden if a different initial width is required

unsigned int Framework::GetInitialWindowWidth() const
{
	return DEFAULT_WIDTH;
}

// Initialise the application.  Called after the window and bitmap has been
// created, but before the main loop starts
//
// Return false if the application cannot be initialised.

bool Framework::Initialise()
{
	return true;
}

// Perform any updates to the structures that will be used
// to render the window (i.e. transformation matrices, etc).
//
// This should be overridden

void Framework::Update(Bitmap &bitmap)
{
	// Default update method does nothing
}

// Render the window. This should be overridden

void Framework::Render(Bitmap &bitmap)
{
	// Default render method just sets the background to the default window colour
	bitmap.Clear((HBRUSH)(COLOR_WINDOW + 1));
}

// Perform any application shutdown that is needed

void Framework::Shutdown()
{
}

// Register the  window class, create the window and
// create the bitmap that we will use for rendering

bool Framework::InitialiseMainWindow(int nCmdShow)
{
	#define MAX_LOADSTRING 100

	WCHAR windowTitle[MAX_LOADSTRING];          
	WCHAR windowClass[MAX_LOADSTRING];            
	
	LoadStringW(_hInstance, IDS_APP_TITLE, windowTitle, MAX_LOADSTRING);
	LoadStringW(_hInstance, IDC_RASTERISER, windowClass, MAX_LOADSTRING);

	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = _hInstance;
	wcex.hIcon = LoadIcon(_hInstance, MAKEINTRESOURCE(IDI_RASTERISER));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = windowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	if (!RegisterClassExW(&wcex))
	{
		return false;
	}

	_hWnd = CreateWindowW(windowClass, 
						  windowTitle, 
					      WS_OVERLAPPEDWINDOW,
						  CW_USEDEFAULT, 0, GetInitialWindowWidth(), GetInitialWindowHeight(),  
					      nullptr, nullptr, _hInstance, nullptr);
	if (!_hWnd)
	{
		return false;
	}
	ShowWindow(_hWnd, nCmdShow);
	UpdateWindow(_hWnd);

	// Create a bitmap of the same size as the client area of the window.  This is what we
	// will be drawing on
	RECT clientArea;
	GetClientRect(_hWnd, &clientArea);
	_bitmap.Create(_hWnd, clientArea.right - clientArea.left, clientArea.bottom - clientArea.top);
	return true;
}

// The WndProc for the current window.  This cannot be a method, but we can
// redirect all messages to a method.

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (_thisFramework != NULL)
	{
		// If framework is started, then we can call our own message proc
		return _thisFramework->MsgProc(hWnd, message, wParam, lParam);
	}
	else
	{
		// otherwise, we just pass control to the default message proc
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

// Our main WndProc

LRESULT Framework::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_PAINT:
			{
				// Copy the contents of the bitmap to the window
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);
				BitBlt(hdc, 0, 0, _bitmap.GetWidth(), _bitmap.GetHeight(), _bitmap.GetDC(), 0, 0, SRCCOPY);
				EndPaint(hWnd, &ps);
			}
			break;

		case WM_SIZE:
			// Delete any existing bitmap and create a new one of the required size.
			_bitmap.Create(hWnd, LOWORD(lParam), HIWORD(lParam));
			// Now render to the resized bitmap
			Update(_bitmap);
			Render(_bitmap);
			InvalidateRect(hWnd, NULL, FALSE);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}