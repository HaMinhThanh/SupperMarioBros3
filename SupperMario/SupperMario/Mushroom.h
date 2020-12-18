#pragma once
#include "GameObject.h"

#define	MUSHROOM_BBOX_WIDTH 16
#define MUSHROOM_BBOX_HEIGHT 16

#define MUSHROOM_GRAVITY	0.002f

class Mushroom: public GameObject
{
public:
	bool start;

	Mushroom();
	~Mushroom();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

