#include "Colour.h"
#include "Math.h"


Colour::Colour()
{
	for (int i = 0; i < 3; i++)
	{
		Set(i, 0);
	}
}

Colour::Colour(int r, int g, int b)
{
	SetR(r);
	SetG(g);
	SetB(b);
}

Colour::Colour(COLORREF colour)
{
	Set(0, GetRValue(colour));
	Set(1, GetBValue(colour));
	Set(2, GetGValue(colour));
}

Colour::Colour(const Colour& colour)
{
	Copy(colour);
}

int Colour::GetR() const
{
	return Get(0);
}
void Colour::SetR(int r)
{
	Set(0, r);
}
int Colour::GetG() const
{
	return Get(1);
}
void Colour::SetG(int g)
{
	Set(1, g);
}
int Colour::GetB() const
{
	return Get(2);
}
void Colour::SetB(int b)
{
	Set(2, b);
}
int Colour::Get(int i) const
{
	if (i < 0 || i >= 3)
		throw "invalid index";

	return _v[i];
}
void Colour::Set(int i, int v)
{
	if (i < 0 || i >= 3)
		throw "invalid index";

	_v[i] = Math::Clamp(v, 0, 255);
}

COLORREF Colour::ToColorRef()
{
	return RGB(GetR(), GetG(), GetB());
}

Colour& Colour::operator=(const Colour& colour)
{
	if (this != &colour)
	{
		Copy(colour);
	}

	return *this;
}

Colour Colour::Lerp(Colour a, Colour b, float t)
{
	return Colour(
		Math::LerpInt(a.GetR(), b.GetR(), t),
		Math::LerpInt(a.GetG(), b.GetG(), t),
		Math::LerpInt(a.GetB(), b.GetB(), t));
}

void Colour::Copy(const Colour& colour)
{
	for (int i = 0; i < 3; i++)
	{
		Set(i, colour.Get(i));
	}
}

Colour::~Colour()
{
}
