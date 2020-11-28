#pragma once

#include "GameObject.h"
#include "FireBall.h"

#define FIRE_HIDEN		0
#define FIRE_TOP		1
#define FIRE_TOP_LEFT	2
#define FIRE_TOP_RIGHT	3
#define FIRE_BOT_LEFT	4
#define FIRE_BOT_RIGHT	5

class Fire: public GameObject
{
public:
	Fire();
	~Fire();

	//void StartUnFinish() { untouchable = 1; untouchable_start = GetTickCount(); }

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual void SetState(int state);
};

