#include "Math.h"

Math::Math()
{
}

float Math::Lerp(float a, float b, float t)
{
	if (t < 0 || t > 1) 
		throw "invalid interpolant";

	return t * (b - a) + a;
}

int Math::LerpInt(int a, int b, float t)
{
	return (int)Lerp((float)a, (float)b, t);
}

COLORREF Math::LerpRGB(COLORREF a, COLORREF b, float t)
{
	return RGB(
		Lerp(GetRValue(a), GetRValue(b), t),
		Lerp(GetGValue(a), GetGValue(b), t),
		Lerp(GetBValue(a), GetBValue(b), t));
}

Vector3D Math::LerpVector3D(Vector3D a, Vector3D b, float t)
{
	return Vector3D(
		Lerp(a.GetX(), b.GetX(), t),
		Lerp(a.GetY(), b.GetY(), t),
		Lerp(a.GetZ(), b.GetZ(), t));
}

int Math::Clamp(int val, int min, int max)
{
	if (val < min)
		return min;

	if (val > max)
		return max;

	return val;
}

Math::~Math()
{
}
