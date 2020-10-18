#pragma once

#include "GameObject.h"

#define MARIO_WALKING_SPEED 0.1f
#define MARIO_RUN_SPEED 0.15f

#define MARIO_START_X 10.0f
#define MARIO_START_Y 130.0f
#define MARIO_START_VX 0.1f

#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_GRAVITY			0.002f
#define MARIO_JUMP_DEFLECT_SPEED 0.2f
#define MARIO_DIE_DEFLECT_SPEED 0.5f

#define MARIO_STATE_IDLE 0
#define MARIO_STATE_WALKING_RIGHT 100
#define MARIO_STATE_WALKING_LEFT 200
#define MARIO_STATE_JUMP 300
#define MARIO_STATE_DIE 400

#define MARIO_STATE_FLY 500
#define MARIO_STATE_HOLD 600
#define MARIO_STATE_KICK 700
#define MARIO_STATE_RUN 800

#define MARIO_ANI_BIG_IDLE_RIGHT 0
#define MARIO_ANI_BIG_IDLE_LEFT 1
#define MARIO_ANI_SMALL_IDLE_RIGHT 2
#define MARIO_ANI_SMALL_IDLE_LEFT 3

#define MARIO_ANI_BIG_WALKING_RIGHT 4
#define MARIO_ANI_BIG_WALKING_LEFT 5
#define MARIO_ANI_SMALL_WALKING_RIGHT 6
#define MARIO_ANI_SMALL_WALKING_LEFT 7
#define MARIO_ANI_TAIL_WALKING_RIGHT 8
#define MARIO_ANI_TAIL_WALKING_LEFT 9

#define MARIO_ANI_SMALL_JUMP_RIGHT 10
#define MARIO_ANI_SMALL_JUMP_LEFT 11
#define MARIO_ANI_BIG_JUMP_RIGHT 12
#define MARIO_ANI_BIG_JUMP_LEFT 13
#define MARIO_ANI_TAIL_JUMP_RIGHT 14
#define MARIO_ANI_TAIL_JUMP_LEFT 15

#define MARIO_ANI_FLY_RIGHT 16
#define MARIO_ANI_FLY_LEFT 17

#define MARIO_ANI_KICK_RIGHT 18
#define MARIO_ANI_KICK_LEFT 19

#define MARIO_ANI_HOLD_RIGHT 20
#define MARIO_ANI_HOLD_LEFT 21


#define MARIO_ANI_TAIL_IDLE_RIGHT 22
#define MARIO_ANI_TAIL_IDLE_LEFT 23

#define MARIO_ANI_DIE 99

#define MARIO_LEVEL_SMALL 1
#define MARIO_LEVEL_BIG 2
#define MARIO_LEVEL_TAIL 3

#define MARIO_TAIL_BBOX_WIDTH 20
#define MARIO_TAIL_BBOX_HEIGHT 27

#define MARIO_BIG_BBOX_WIDTH 15
#define MARIO_BIG_BBOX_HEIGHT 27

#define MARIO_SMALL_BBOX_WIDTH 13
#define MARIO_SMALL_BBOX_HEIGHT 15

#define MARIO_UNTOUCHABLE_TIME 5000

class Mario: public GameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;
public:
	//Mario(float x, float y, float vx);
	Mario():GameObject()
	{
		level = MARIO_LEVEL_TAIL;
		untouchable = 0;
	}	
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void SetLevel(int lev) { level = lev; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

