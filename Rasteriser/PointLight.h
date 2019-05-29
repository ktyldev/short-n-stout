#pragma once

#include "Light.h"
#include "Vertex.h"

class PointLight : public Light
{
public:
	PointLight();
	PointLight(Colour colour, Vertex position, float attenuation);
	~PointLight();

	Vertex GetPosition() const;
	void SetPosition(Vertex &position);
	float GetAttenuation() const;
	void SetAttenuation(float a);

private:
	Vertex _position;
	float _attenuation;
};

