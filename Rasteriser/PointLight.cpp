#include "PointLight.h"



PointLight::PointLight()
	: Light()
{
}

PointLight::PointLight(Colour colour, Vertex position, float attenuation)
	: Light(colour)
{
	SetPosition(position);
	SetAttenuation(attenuation);
}

Vertex PointLight::GetPosition() const
{
	return _position;
}
void PointLight::SetPosition(Vertex& position)
{
	_position = position;
}
float PointLight::GetAttenuation() const
{
	return _attenuation;
}
void PointLight::SetAttenuation(float a)
{
	if (a < 0 || a > 1)
		throw "invalid range";

	_attenuation = a;
}

PointLight::~PointLight()
{
}
