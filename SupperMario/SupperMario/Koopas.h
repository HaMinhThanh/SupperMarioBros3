#pragma once

#include "GameObject.h"

#define KOOPAS_WALKING_SPEED 0.03f
#define KOOPAS_GRAVITY 0.002f
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

#define KOOPAS_ANI_WALKING_RIGHT 0
#define KOOPAS_ANI_WALKING_LEFT 1
#define KOOPAS_ANI_DIE 2
#define KOOPAS_ANI_DIE_WALKING 3
#define KOOPAS_ANI_DIE_WALKING_LEFT		

class Koopas: public GameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:
	Koopas();
	virtual void SetState(int state);

	bool isDie;
};

