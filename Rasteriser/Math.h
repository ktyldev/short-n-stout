#pragma once

#include <windows.h>
#include "Vector3D.h"

class Math
{
public:
	Math();

	static float Lerp(float a, float b, float t);
	static int LerpInt(int a, int b, float t);
	static COLORREF LerpRGB(COLORREF a, COLORREF b, float t);
	static Vector3D LerpVector3D(Vector3D a, Vector3D b, float t);
	static int Clamp(int val, int min, int max);

	~Math();
};

