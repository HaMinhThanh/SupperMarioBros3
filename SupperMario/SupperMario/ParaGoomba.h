#pragma once

#include "GameObject.h"
#include "Goomba.h"

class ParaGoomba: public GameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:

	ParaGoomba();
	virtual void SetState(int state);
};

