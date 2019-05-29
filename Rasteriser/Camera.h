#pragma once

#include "Vertex.h"
#include "Matrix.h"

class Camera
{
public:
	Camera();
	Camera(float xRotation, float yRotation, float zRotation, const Vertex& position);
	~Camera();

	// accessors
	Matrix GetViewingMatrix() const;
	Vertex GetPosition() const;
	void SetPosition(Vertex& pos);
	float GetFov() const;

private:
	void Init(float xRot, float yRot, float zRot, const Vertex& pos);

	Matrix _m;
	Vertex _pos;
	float _fov = 70.0f;
};

