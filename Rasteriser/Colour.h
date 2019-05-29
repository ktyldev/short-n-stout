#pragma once

#include <Windows.h>

class Colour
{
public:
	Colour();
	Colour(int r, int g, int b);
	Colour(const Colour& colour);
	Colour(COLORREF colour);
	~Colour();

	int GetR() const;
	void SetR(int r);
	int GetG() const;
	void SetG(int g);
	int GetB() const;
	void SetB(int b);
	int Get(int i) const;
	void Set(int i, int v);

	COLORREF ToColorRef();

	Colour& operator=(const Colour& colour);

	static Colour Lerp(Colour a, Colour b, float t);

private:
	int _v[3];

	void Copy(const Colour& colour);
};

