#pragma once

#include "Vector3D.h"
#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight();
	DirectionalLight(int r, int g, int b, Vector3D &dir);
	DirectionalLight(const DirectionalLight &dLight);
	~DirectionalLight();

	Vector3D GetDirection() const;
	void SetDirection(Vector3D &dir);

private:
	Vector3D _direction;
};

