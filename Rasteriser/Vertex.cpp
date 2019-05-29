#include "Vertex.h"
#include <cmath>

Vertex::Vertex()
{
	_x = 0.0f;
	_y = 0.0f;
	_z = 0.0f;

	_w = 1.0f;
	_normal = Vector3D();
	_colour = Colour();
	_normalContributions = 0;
}

Vertex::Vertex(float x, float y)
{
	_x = x;
	_y = y;

	_z = 0.0f; 
	_w = 1.0f;
	_normal = Vector3D();
	_colour = Colour();
	_normalContributions = 0;
}

Vertex::Vertex(float x, float y, float z)
{
	_x = x;
	_y = y;
	_z = z;

	_w = 1.0f;
	_normal = Vector3D();
	_colour = Colour();
	_normalContributions = 0;
}

Vertex::Vertex(const Vertex & other)
{
	Copy(other);
}

void Vertex::Dehomogenise()
{
	_x /= _w;
	_y /= _w;
	_z /= _w;
	_w /= _w;
}

float Vertex::GetX() const
{
	return _x;
}

void Vertex::SetX(const float x)
{
	_x = x;
}

float Vertex::GetY() const
{
	return _y;
}

void Vertex::SetY(const float y)
{
	_y = y;
}

float Vertex::GetZ() const 
{ 
	return _z; 
}

void Vertex::SetZ(const float z) 
{
	_z = z;
}

float Vertex::GetW() const 
{ 
	return _w; 
}

void Vertex::SetW(const float w) 
{
	_w = w;
}

Vector3D Vertex::GetNormal() const
{
	return _normal;
}

void Vertex::SetNormal(Vector3D &v)
{
	_normal = v;
}

void Vertex::AddNormal(Vector3D &v)
{
	Vector3D n = GetNormal() + v;
	SetNormal(n);
	_normalContributions++;
}

Colour Vertex::GetColour() const
{
	return _colour;
}

void Vertex::SetColour(Colour colour)
{
	_colour = colour;
}

int Vertex::NormalContributions() const
{
	return _normalContributions;
}

void Vertex::ResetContributions()
{
	_normalContributions = 0;
}

POINT Vertex::ToPoint() const
{
	POINT p;
	p.x = (LONG)GetX();
	p.y = (LONG)GetY();

	return p;
}

Vertex& Vertex::operator=(const Vertex& rhs)
{
	// Only do the assignment if we are not assigning
	// to ourselves
	if (this != &rhs)
	{
		Copy(rhs);
	}
	return *this;
}

// The const at the end of the declaraion for '==' indicates that this 
// operation does not change any of the member variables in this class.
bool Vertex::operator==(const Vertex& rhs) const
{
	return (
		_x == rhs.GetX() && 
		_y == rhs.GetY() &&
		_z == rhs.GetZ() &&
		_w == rhs.GetW());
}

// You can see three different uses of 'const' here:
//
// The first const indicates that the method changes the return value, but it is not moved in memory
// The second const indicates that the parameter is passed by reference, but it is not modified
// The third const indicates that the operator does not change any of the memory variables in the class.
const Vertex Vertex::operator+(const Vertex& rhs) const
{
	return Vertex(
		_x + rhs.GetX(), 
		_y + rhs.GetY(), 
		_z + rhs.GetZ());
}

const Vector3D Vertex::operator-(const Vertex& rhs) const
{
	return Vector3D(
		rhs.GetX() - _x, 
		rhs.GetY() - _y, 
		rhs.GetZ() - _z);
}

void Vertex::OrderPolyVerticesByAscendingY(Vertex* vertices)
{
	Vertex temp;

	if (vertices[0].GetY() > vertices[1].GetY())
	{
		temp = vertices[0];
		vertices[0] = vertices[1];
		vertices[1] = temp;
	}
	// 0.y <= 1.y
	if (vertices[0].GetY() > vertices[2].GetY())
	{
		temp = vertices[0];
		vertices[0] = vertices[2];
		vertices[2] = temp;
	}
	// 0.y <= 1.y and 0.y <= 2.y, so test 1 and 2	
	if (vertices[1].GetY() > vertices[2].GetY())
	{
		temp = vertices[1];
		vertices[1] = vertices[2];
		vertices[2] = temp;
	}
}

void Vertex::Copy(const Vertex& other)
{
	SetX(other.GetX());
	SetY(other.GetY());
	SetZ(other.GetZ());
	SetW(other.GetW());
	SetNormal(other.GetNormal());
	SetColour(other.GetColour());
	_normalContributions = other.NormalContributions();
}