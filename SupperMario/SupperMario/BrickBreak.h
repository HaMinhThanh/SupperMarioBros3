#pragma once
#include "GameObject.h"
class BrickBreak: public GameObject
{
public:
	BrickBreak(float x, float y);
	~BrickBreak();

	DWORD time_render;
	int render;

	bool isRender;

	float x1, x2, x3, x4;
	float y1, y2, y3, y4;

	void StartRender() { render = 1; time_render = GetTickCount(); }

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

