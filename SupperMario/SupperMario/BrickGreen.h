#pragma once

#include "GameObject.h"

class BrickGreen : public GameObject
{
public:

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

