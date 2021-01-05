#pragma once

#include "GameObject.h"

#define KOOPAS_WALKING_SPEED 0.03f
#define KOOPAS_GRAVITY 0.01f
#define KOOPAS_DIE_SPEED	0.2f

#define KOOPAS_BBOX_WIDTH 16
#define KOOPAS_BBOX_HEIGHT 26

#define KOOPAS_BBOX_HEIGHT_DIE 16
#define KOOPAS_BBOX_WIDTH_DIE	16

#define KOOPAS_LEVEL_WING	4
#define KOOPAS_LEVEL_NORMAL	3
#define KOOPAS_LEVEL_DIE_DOWN	2
#define KOOPAS_LEVEL_DIE_UP	1

#define KOOPAS_STATE_WALKING_RIGHT	100
#define KOOPAS_STATE_WALKING_LEFT	200
#define KOOPAS_STATE_IDLE	300

#define KOOPAS_ANI_NORMAL_RIGHT 0
#define KOOPAS_ANI_NORMAL_LEFT 1
#define KOOPAS_ANI_WING_RIGHT	2
#define KOOPAS_ANI_WING_LEFT	3
#define KOOPAS_ANI_DIE_RUNNING_DOWN	4
#define KOOPAS_ANI_DIE_RUNNING_UP	5
#define KOOPAS_ANI_WAIT_DOWN		6
#define KOOPAS_ANI_WAIT_UP			7
#define KOOPAS_ANI_DIE_DOWN			8
#define KOOPAS_ANI_DIE_UP			9	

#define KOOPAS_WAIT_TO_NORMAL_TIME	5000

class Koopas: public GameObject
{
public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:
	Koopas(float x,float y,int level, int type);
	virtual void SetState(int state);

	float backupX;
	float backupY;

	bool isDie;

	int ani_set;

	int level;
	int type;
	void SetLevel(int l) { level = l; }

	bool isWait;
	bool turnToNormal;

	int isWaiting;
	DWORD isWaiting_start;

	int jumping;
	DWORD time_jumping;

	void StartJumping() { jumping = 1; time_jumping = GetTickCount(); }
	void StartWaitToNormal() { isWaiting = 1; isWaiting_start = GetTickCount(); }

};

