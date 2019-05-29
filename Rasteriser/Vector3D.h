#pragma once
class Vector3D
{
public:
	Vector3D();
	Vector3D(float x, float y, float z);
	~Vector3D();

	Vector3D ToUnitVector() const;
	float Magnitude() const;

	float GetX() const;
	void SetX(float x);
	float GetY() const;
	void SetY(float y);
	float GetZ() const;
	void SetZ(float z);

	float Gradient() const;

	const Vector3D operator+ (const Vector3D& rhs) const;
	const Vector3D operator/ (const float divisor) const;

	static float DotProduct(Vector3D &u, Vector3D &v);
	static Vector3D CrossProduct(Vector3D &u, Vector3D &v);

private:
	float _x;
	float _y;
	float _z;
};

