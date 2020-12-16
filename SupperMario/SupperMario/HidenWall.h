#pragma once
#include "GameObject.h"
class HidenWall: public GameObject
{
public:
	int type;

	HidenWall(float x = 0, float y = 0, int type = 0, int ani_id = 0);

	~HidenWall();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

