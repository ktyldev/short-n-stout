#pragma once
#include "windows.h"

class Bitmap
{
public:
	Bitmap();
	~Bitmap();

	bool			Create(HWND hWnd, unsigned int width, unsigned int height);
	HDC				GetDC() const;
	unsigned int	GetWidth() const;
	unsigned int	GetHeight() const ;
	void			Clear(HBRUSH hBrush);
	void			Clear(COLORREF colour);

private:
	HBITMAP			_hBitmap;
	HBITMAP			_hOldBitmap;
	HDC				_hMemDC;
	unsigned int	_width;
	unsigned int	_height;

	void DeleteBitmap();

};

