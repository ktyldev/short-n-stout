#include "Polygon3D.h"



Polygon3D::Polygon3D()
{
	for (int i = 0; i < 3; i++)
	{
		_vIndices[i] = 0;
	}
}

Polygon3D::Polygon3D(int index01, int index02, int index03)
{
	_vIndices[0] = index01;
	_vIndices[1] = index02;
	_vIndices[2] = index03;
}

Polygon3D::Polygon3D(const Polygon3D& p)
{
	Copy(p);
}

Polygon3D::~Polygon3D()
{
}

int Polygon3D::GetIndex(int i) const
{
	if (i < 0 || i >= 3)
		throw "invalid index";

	return _vIndices[i];
}

float Polygon3D::GetZDepth() const
{
	return _zDepth;
}

void Polygon3D::SetZDepth(float zDepth)
{
	_zDepth = zDepth;
}

Colour Polygon3D::GetColour() const
{
	return _colour;
}

void Polygon3D::SetColour(Colour colour)
{
	_colour = colour;
}

Vector3D Polygon3D::GetNormal() const
{
	return _normal;
}

void Polygon3D::SetNormal(Vector3D &v)
{
	_normal = v;
}

Polygon3D& Polygon3D::operator=(const Polygon3D &rhs)
{
	if (this != &rhs)
	{
		Copy(rhs);
	}

	return *this;
}

void Polygon3D::Copy(const Polygon3D& other)
{
	for (int i = 0; i < 3; i++)
	{
		_vIndices[i] = other.GetIndex(i);
	}

	cull = other.cull;
	SetColour(other.GetColour());
	SetNormal(other.GetNormal());
}