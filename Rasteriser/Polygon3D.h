#pragma once

#include "Vector3D.h"
#include "Colour.h"

// an individual polygon of a 3D model. also known as a 'tri'
class Polygon3D
{
public:
	Polygon3D();
	Polygon3D(int index01, int index02, int index03);

	/*
	copy constructor
	
	'&' here signifies a reference to an existing Polygon3D
	such that it isn't passed by value and no copies are made

	'const' promises not to modify the original
	*/
	Polygon3D(const Polygon3D& p);
	~Polygon3D();

	int GetIndex(int) const;
	float GetZDepth() const;
	void SetZDepth(float zDepth);
	Colour GetColour() const;
	void SetColour(Colour colour);
	Vector3D GetNormal() const;
	void SetNormal(Vector3D &v);

	// copy assignment
	Polygon3D& operator= (const Polygon3D &rhs);

	bool cull;

private:
	// the indices of the vertices of this tri in a 
	// one dimensional array
	int _vIndices[3];
	float _zDepth;
	Colour _colour;
	Vector3D _normal;

	void Copy(const Polygon3D& other);
};

