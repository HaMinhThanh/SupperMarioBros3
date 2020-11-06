#pragma once
#include <d3dx9.h>
#include "GameObject.h"
#include "Animations.h"
#include "Camera.h"
#include "MarioData.h"
#include "MarioState.h"
#include "MarioWalking.h"
#include "BrickColor.h"

#define MARIO_WALKING_SPEED		0.15f 
//0.1f
#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_DEFLECT_SPEED 0.2f
#define MARIO_GRAVITY			0.002f
#define MARIO_DIE_DEFLECT_SPEED	 0.5f

#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_JUMP			300
#define MARIO_STATE_DIE				400
#define MARIO_STATE_FALL			500
#define MARIO_STATE_RUN				600

#define MARIO_ANI_BIG_IDLE_RIGHT		0
#define MARIO_ANI_BIG_IDLE_LEFT			1
#define MARIO_ANI_SMALL_IDLE_RIGHT		2
#define MARIO_ANI_SMALL_IDLE_LEFT			3
#define MARIO_ANI_TAIL_IDLE_RIGHT			17
#define MARIO_ANI_TAIL_IDLE_LEFT			18

#define MARIO_ANI_BIG_WALKING_RIGHT			4
#define MARIO_ANI_BIG_WALKING_LEFT			5
#define MARIO_ANI_SMALL_WALKING_RIGHT		6
#define MARIO_ANI_SMALL_WALKING_LEFT		7
#define MARIO_ANI_TAIL_WALKING_RIGHT		8
#define MARIO_ANI_TAIL_WALKING_LEFT			9

#define MARIO_ANI_DIE				10

// jumping animation
#define MARIO_ANI_BIG_JUMPING_RIGHT			11
#define MARIO_ANI_BIG_JUMPING_LEFT			12
#define MARIO_ANI_SMALL_JUMPING_RIGHT		13
#define MARIO_ANI_SMALL_JUMPING_LEFT		14
#define MARIO_ANI_TAIL_JUMPING_RIGHT		15
#define MARIO_ANI_TAIL_JUMPING_LEFT			16

// falling animation
#define MARIO_ANI_BIG_FALLING_RIGHT			19
#define MARIO_ANI_BIG_FALLING_LEFT			20
#define MARIO_ANI_SMALL_FALLING_RIGHT		21
#define MARIO_ANI_SMALL_FALLING_LEFT		22
#define MARIO_ANI_TAIL_FALLING_RIGHT		23
#define MARIO_ANI_TAIL_FALLING_LEFT			24

#define MARIO_ANI_BIG_KICK_RIGHT			31
#define MARIO_ANI_BIG_KICK_LEFT				32
#define MARIO_ANI_SMALL_KICK_RIGHT			33
#define MARIO_ANI_SMALL_KICK_LEFT			35
#define MARIO_ANI_TAIL_KICK_RIGHT			35
#define MARIO_ANI_TAIL_KICK_LEFT			36

#define MARIO_ANI_IDLE_BIG_HOLD_RIGHT		37
#define MARIO_ANI_IDLE_BIG_HOLD_LEFT		38
#define MARIO_ANI_IDLE_TAIL_HOLD_RIGHT		39
#define MARIO_ANI_IDLE_TAIL_HOLD_LEFT		40
#define MARIO_ANI_WALKING_BIG_HOLD_RIGHT	41
#define MARIO_ANI_WALKING_BIG_HOLD_LEFT		42
#define MARIO_ANI_WALKING_TAIL_HOLD_RIGHT	43
#define MARIO_ANI_WALKING_TAIL_HOLD_LEFT	44


#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_LEVEL_TAIL	3

#define ANIMATION_SET_1		1
#define ANIMATION_SET_2		100

#define MARIO_BIG_BBOX_WIDTH  15
#define MARIO_BIG_BBOX_HEIGHT 27

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 15

#define MARIO_TAIL_BBOX_WIDTH  22
#define MARIO_TAIL_BBOX_HEIGHT 27

#define MARIO_UNTOUCHABLE_TIME 5000

const float PLAYER_MAX_JUMP_VELOCITY = 0.5f; //van toc nhay lon nhat
const float PLAYER_MIN_JUMP_VELOCITY = -0.5f; //van toc nhay thap nhat
const float PLAYER_MAX_RUNNING_SPEED = 0.15; //toc do chay nhanh nhat cua player
const float PLAYER_BOTTOM_RANGE_FALLING = 6.0f; // do dai va cham voi bottom neu nhu va cham

class Mario: public GameObject
{
public:
	int level;
	int untouchable;
	DWORD untouchable_start;

	float start_x;
	float start_y;

	int direct;

public:

	bool isTurnToSmall;
	bool isTurnToBig;
	bool isTurnToTail;

	bool isPressed;

	bool isAllowKick;

	bool isCollisionOnAxisY;

	bool isAllowHold;

	bool isHoldingItem;

	Mario(float x = 0.0f, float y = 0.0f);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);

	void SetStateName(MarioState* newState);

	void SetLevel(int lev) { level = lev; }
	int GetLevel();

	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void Reset();

	int GetAni();
	void SetAni(int ani);

	void changeAni();
	

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void changeAnimation(MarioState *state);

	void OnKeyDown(int key);
	void OnKeyUp(int key);
	void KeyState(BYTE* states);

	void NoCollisionWithAxisY();

	bool isCollisionWithItem(LPGAMEOBJECT item);

	void SetHodingItem(LPGAMEOBJECT item);

	bool allowMoveLeft;
	bool allowMoveRight;

protected:
	MarioData* marioData;

	MarioState * CurrentState;

	MarioState::StateName mCurrentState;

	bool allowJump;

	int currentAni;	

};

