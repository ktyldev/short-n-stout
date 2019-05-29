#include "Vector3D.h"
#include <cmath>


Vector3D::Vector3D()
{
	_x = 0;
	_y = 0;
	_z = 0;
}

Vector3D::Vector3D(float x, float y, float z)
{
	_x = x;
	_y = y;
	_z = z;
}

Vector3D Vector3D::ToUnitVector() const
{
	float magnitude = Magnitude();
	// avoid division by zero
	if (magnitude == 0)
		return *this;

	Vector3D result = Vector3D(
		_x / magnitude,
		_y / magnitude,
		_z / magnitude);

	return result;
}

float Vector3D::Magnitude() const
{
	return sqrtf(pow(_x, 2) + pow(_y, 2) + pow(_z, 2));
}

float Vector3D::GetX() const
{
	return _x;
}
void Vector3D::SetX(float x)
{
	_x = x;
}
float Vector3D::GetY() const
{
	return _y;
}
void Vector3D::SetY(float y)
{
	_y = y;
}
float Vector3D::GetZ() const
{
	return _z;
}
void Vector3D::SetZ(float z)
{
	_z = z;
}

float Vector3D::Gradient() const
{
	return _y / _x;
}

const Vector3D Vector3D::operator+(const Vector3D &rhs) const
{
	return Vector3D(
		GetX() + rhs.GetX(), 
		GetY() + rhs.GetY(),
		GetZ() + rhs.GetZ());
}

const Vector3D Vector3D::operator/(const float divisor) const
{
	return Vector3D(
		GetX() / divisor,
		GetY() / divisor,
		GetZ() / divisor);
}

float Vector3D::DotProduct(Vector3D &u, Vector3D &v)
{
	return u.GetX() * v.GetX() 
		+ u.GetY() * v.GetY()
		+ u.GetZ() * v.GetZ();
}

Vector3D Vector3D::CrossProduct(Vector3D &u, Vector3D &v)
{
	float x = u.GetY() * v.GetZ() - u.GetZ() * v.GetY();
	float y = u.GetZ() * v.GetX() - u.GetX() * v.GetZ();
	float z = u.GetX() * v.GetY() - u.GetY() * v.GetX();

	return Vector3D(x, y, z);
}

Vector3D::~Vector3D()
{
}
