#pragma once

#include "GameObject.h"

#define FIREBALL_BBOX_WIDTH		4
#define FIREBALL_BBOX_HEIGHT	5	

#define FIREBALL_GOING_RIGHT	1
#define FIREBALL_GOING_LEFT		2	

#define FIREBALL_GRAVITY	0.00005f
#define FIREBALL_SPEED_X	0.05f
#define FIREBALL_SPEED_Y	0.001f	

#define FIREBALL_SPEED_MAX_Y	0.01f

#define FIREBALL_UNTOUCHABLE_TIME	5000


class FireBall: public GameObject
{
	int untouchable;
	DWORD untouchable_start;

public:
	FireBall();
	~FireBall();

	void StartUnFinish() { untouchable = 1; untouchable_start = GetTickCount(); }

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual void SetState(int state);
};

