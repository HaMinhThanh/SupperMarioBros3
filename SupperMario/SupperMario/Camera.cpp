#include "Camera.h"

Camera::Camera(int width, int height)
{
	Width = width;
	Height = height;
	isFollowMario = true;

	BoundaryLeft = 0;
	BoundaryRight = 0;
	
	vx = 0;
}

Camera::~Camera()
{

}

void Camera::Update(DWORD dt)
{
	this->dt = dt;

	if (xCam < BoundaryLeft)
		xCam = BoundaryLeft;

	if (xCam > BoundaryRight)
		xCam = BoundaryRight;

}

void Camera::SetPosition(float x, float y)
{
	xCam = x;
	yCam = y;
}

float Camera::GetXCam()
{
	return xCam;
}

float Camera::GetYCam()
{
	return yCam;
}

int Camera::GetWidth()
{
	return Width;
}

int Camera::GetHeight()
{
	return Height;
}

bool Camera::FollowMario()
{
	return isFollowMario;
}

void Camera::SetFollowMario(bool b)
{
	isFollowMario = b;
}

void Camera::SetBoundary(float left, float right)
{
	BoundaryLeft = left;
	BoundaryRight = right;
}

float Camera::GetBoundaryLeft()
{
	return BoundaryLeft;
}

float Camera::GetBoundaryRight()
{
	return BoundaryRight;
}
