#pragma once
#include "GameObject.h"
class HidenWall: public GameObject
{
public:
	int type;

	HidenWall();
	HidenWall(float x, float y, int type);

	~HidenWall();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

