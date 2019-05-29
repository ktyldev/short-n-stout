#pragma once

#include "Colour.h"

class Light
{
public:
	Light();
	Light(Colour& colour);
	Light(int r, int g, int b);
	~Light();

	Colour GetColour() const;
	void SetColour(Colour colour);

private:
	Colour _colour;
};

