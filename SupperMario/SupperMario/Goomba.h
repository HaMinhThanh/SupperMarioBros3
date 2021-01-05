#pragma once

#include "GameObject.h"

#define GOOMBA_WALKING_SPEED 0.025f;

#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 15
#define GOOMBA_BBOX_HEIGHT_DIE 9

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE	200

#define GOOMBA_ANI_WALKING	0
#define GOOMBA_ANI_DIE		1

#define GOOMBA_TIME_DIE		200
#define GOOMBA_TIME_FLY		3000

class Goomba: public GameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:	

	DWORD time_die;
	int _die;

	DWORD time_fly;
	int _fly;

	float maxX;
	float minX;

	Goomba(float max = 2800, float min = 0);
	virtual void SetState(int state);

	void StartDie() { _die = 1; time_die = GetTickCount(); }
	void StartFly() { _fly = 1; time_fly = GetTickCount(); }
};

