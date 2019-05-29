#pragma once
#include <windows.h>
#include "Resource.h"
#include <string>
#include <exception>
#include "Bitmap.h"

using namespace std;

class Framework
{
public:
	Framework();
	virtual ~Framework();

	int Run(HINSTANCE hInstance, int nCmdShow);

	LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	virtual unsigned int GetInitialWindowHeight() const;
	virtual unsigned int GetInitialWindowWidth() const;
	virtual bool Initialise();
	virtual void Update(Bitmap &bitmap);
	virtual void Render(Bitmap &bitmap);
	virtual void Shutdown();

private:
	HINSTANCE		_hInstance;
	HWND			_hWnd;
	Bitmap			_bitmap;

	// Used in timing loop
	double			_timeSpan;

	bool InitialiseMainWindow(int nCmdShow);
	int MainLoop();
};

