#pragma once
#include <minwindef.h>
class Camera
{
protected:
	float xCam;
	float yCam;

	int Width;
	int Height;

	float BoundaryLeft;
	float BoundaryRight;

	float vx;
	DWORD dt;

	bool isFollowMario;

public:
	Camera(int width, int height);
	~Camera();

	void Update(DWORD dt);

	void SetPosition(float x, float y);

	float GetXCam();
	float GetYCam();
	int GetWidth();
	int GetHeight();

	bool FollowMario();
	void SetFollowMario(bool b);

	void SetBoundary(float left, float right);
	float GetBoundaryLeft();
	float GetBoundaryRight();
};

