#include "Light.h"



Light::Light()
{
}

Light::Light(int r, int g, int b) 
	: Light(Colour(r, g, b))
{
}

Light::Light(Colour& colour)
{
	SetColour(colour);
}

Colour Light::GetColour() const
{
	return _colour;
}
void Light::SetColour(Colour colour)
{
	_colour = colour;
}

Light::~Light()
{
}
