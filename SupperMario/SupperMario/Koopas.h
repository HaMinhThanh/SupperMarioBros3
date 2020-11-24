#pragma once

#include "GameObject.h"

#define KOOPAS_WALKING_SPEED 0.03f
#define KOOPAS_GRAVITY 0.001f
#define KOOPAS_DIE_SPEED	0.05f

#define KOOPAS_BBOX_WIDTH 16
#define KOOPAS_BBOX_HEIGHT 26

#define KOOPAS_BBOX_HEIGHT_DIE 16
#define KOOPAS_BBOX_WIDTH_DIE	16

#define KOOPAS_STATE_WALKING_RIGHT	0
#define KOOPAS_STATE_WALKING_LEFT 100
#define KOOPAS_STATE_DIE 200
#define	KOOPAS_STATE_DIE_WALKING_RIGHT	300	
#define KOOPAS_STATE_DIE_WALKING_LEFT	400	
#define KOOPAS_STATE_BE_FOLLOW_MARIO	500	
#define KOOPAS_STATE_DIE_UP				600
#define KOOPAS_STATE_WING_RIGHT		700
#define KOOPAS_STATE_WING_LEFT		800

#define KOOPAS_ANI_WALKING_RIGHT 0
#define KOOPAS_ANI_WALKING_LEFT 1
#define KOOPAS_ANI_WING_RIGHT	2
#define KOOPAS_ANI_WING_LEFT	3
#define KOOPAS_ANI_DIE_RUNNING_DOWN	4
#define KOOPAS_ANI_DIE_RUNNING_UP	5
#define KOOPAS_ANI_WAIT_DOWN		6
#define KOOPAS_ANI_WAIT_UP			7
#define KOOPAS_ANI_DIE_DOWN			8
#define KOOPAS_ANI_DIE_UP			9	

class Koopas: public GameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:
	Koopas();
	virtual void SetState(int state);

	bool isDie;

	int ani_set;
};

