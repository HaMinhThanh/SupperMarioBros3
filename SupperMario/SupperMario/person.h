#pragma once

#include "GameObject.h"

class person: public GameObject
{
public:
	float max, min;

	person(float max, float min);
	~person();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

