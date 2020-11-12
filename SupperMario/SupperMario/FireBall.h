#pragma once

#include "GameObject.h"

#define FIREBALL_BBOX_WIDTH		4
#define FIREBALL_BBOX_HEIGHT	5	

#define FIREBALL_GOING_RIGHT	1
#define FIREBALL_GOING_LEFT		2	

#define FIREBALL_GRAVITY	0.0005f
#define FIREBALL_SPEED_X	0.1f
#define FIREBALL_SPEED_Y	0.0001f	

#define FIREBALL_SPEED_MAX_Y	0.0001f

#define FIREBALL_UNTOUCHABLE_TIME	3000


class FireBall: public GameObject
{
public:
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

