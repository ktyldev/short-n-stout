#include "DirectionalLight.h"



DirectionalLight::DirectionalLight()
	: Light()
{
}

DirectionalLight::DirectionalLight(int r, int g, int b, Vector3D& dir)
	: Light(r, g, b)
{
	SetDirection(dir);
}

DirectionalLight::DirectionalLight(const DirectionalLight &dLight)
	: Light(dLight.GetColour())
{
	SetDirection(dLight.GetDirection());
}

Vector3D DirectionalLight::GetDirection() const
{
	return _direction.ToUnitVector();
}
void DirectionalLight::SetDirection(Vector3D &dir)
{
	_direction = dir.ToUnitVector();
}

DirectionalLight::~DirectionalLight()
{
}
