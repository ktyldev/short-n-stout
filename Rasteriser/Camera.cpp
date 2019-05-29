#include "Camera.h"



Camera::Camera()
{
	Init(0, 0, 0, Vertex());
}

Camera::Camera(float xRotation, float yRotation, float zRotation, const Vertex& position)
{
	Init(xRotation, yRotation, zRotation, position);
}

Matrix Camera::GetViewingMatrix() const
{
	return _m;
}

Vertex Camera::GetPosition() const
{
	return _pos;
}

void Camera::SetPosition(Vertex& pos) 
{
	_pos = pos;
}

float Camera::GetFov() const
{
	return _fov;
}

void Camera::Init(float xRot, float yRot, float zRot, const Vertex& pos)
{
	_m = Matrix::XRotationMatrix(xRot) *
		Matrix::YRotationMatrix(yRot) *
		Matrix::ZRotationMatrix(zRot) *
		Matrix::TranslationMatrix(
			-pos.GetX(),
			-pos.GetY(),
			-pos.GetZ());

	_pos = pos;
}

Camera::~Camera()
{
}
