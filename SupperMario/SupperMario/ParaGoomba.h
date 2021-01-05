#pragma once

#include "GameObject.h"
#include "Goomba.h"

#define PARAGOOMBA_GRAVITY		0.001f

#define PARAGOOMBA_BBOX_WIDTH	20
#define PARAGOOMBA_BBOX_HEIGHT	24
#define PARAGOOMBA_BBOX_WIDTH_NORMAL	16
#define PARAGOOMBA_BBOX_HEIGHT_NORMAL	16

#define PARAGOOMBA_LEVEL_WING		1
#define PARAGOOMBA_LEVEL_NORMAL		2
#define PARAGOOMBA_LEVEL_DIE		9

#define PARAGOOMBA_STATE_WALKING_RIGHT	1
#define PARAGOOMBA_STATE_WALKING_LEFT	2

#define PARAGOOMBA_ANI_WING			2
#define PARAGOOMBA_ANI_NORMAL		3
#define PARAGOOMBA_ANI_DIE			4

#define PARAGOOMBA_TIME_WALKING		1000
#define PARAGOOMBA_TIME_JUMPING		500
#define PARAGOOMBA_TIME_MOMENT		200

class ParaGoomba: public GameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:

	ParaGoomba();
	virtual void SetState(int state);

	DWORD time_walking ;
	int walking ;

	DWORD time_jumping;
	int jumping;

	DWORD time_momenting;
	int momentable;

	bool isDie = false;

	int level;
	void SetLevel(int l) { level = l; }

	bool isJumping;
	bool isWalking;
	bool isMomentable;

	void StartWalking() { walking = 1; time_walking = GetTickCount(); }
	void StartJumping() { jumping = 1; time_jumping = GetTickCount(); }
	void StartMoment() { momentable = 1; time_momenting = GetTickCount(); }
};

