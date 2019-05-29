#pragma once

#include "Vector3D.h"
#include <Windows.h>
#include "Colour.h"

class Vertex
{
public:
	Vertex();
	Vertex(float x, float y);
	Vertex(float x, float y, float z);
	Vertex(const Vertex& other);

	void Dehomogenise();

	// Accessors
	float GetX() const;
	void SetX(const float x);
	float GetY() const;
	void SetY(const float y);
	float GetZ() const;
	void SetZ(const float z);
	float GetW() const;
	void SetW(const float w);
	Vector3D GetNormal() const;
	void SetNormal(Vector3D &v);
	void AddNormal(Vector3D &v);
	Colour GetColour() const;
	void SetColour(Colour colour);

	int NormalContributions() const;
	void ResetContributions();
	POINT ToPoint() const;

	Vertex& operator= (const Vertex& rhs);
	bool operator== (const Vertex& rhs) const;
	const Vertex operator+ (const Vertex& rhs) const;
	const Vector3D operator- (const Vertex& rhs) const;

	static void OrderPolyVerticesByAscendingY(Vertex* vertices);

private:
	float _x;
	float _y;
	float _z;
	float _w;
	Vector3D _normal;
	Colour _colour;
	int _normalContributions;

	void Copy(const Vertex& other);
};

