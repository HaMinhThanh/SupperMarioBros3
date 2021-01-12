#pragma once
#include "GameObject.h"

#define BOOMERANG_BBOX_WIDHT	15	
#define BOOMERANG_BBOX_HEIGHT	16

class Boomerang: public GameObject
{
public:
	Boomerang();
	~Boomerang();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	int direct;
};

