#pragma once
#include <d3dx9.h>
#include "GameObject.h"
#include "Animations.h"
#include "Camera.h"

#include "BrickColor.h"

#define MARIO_WALKING_SPEED		0.15f 
#define MARIO_WALKING_MAX_SPEED	0.15f

//0.1f
#define MARIO_JUMP_SPEED_Y		0.32f
#define MARIO_JUMP_DEFLECT_SPEED 0.1f
#define MARIO_GRAVITY			0.0008f
#define MARIO_GRAVITY_TAIL		0.25f
#define MARIO_DIE_DEFLECT_SPEED	 0.5f
#define MARIO_FALLING_TAIL_SPEED 0.00075f;

#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_JUMP			300
#define MARIO_STATE_DIE				400
#define MARIO_STATE_FALL			500
#define MARIO_STATE_RUN				600
#define MARIO_STATE_FLY				700	

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

#define MARIO_ANI_BIG_KICK_RIGHT			25
#define MARIO_ANI_BIG_KICK_LEFT				26
#define MARIO_ANI_SMALL_KICK_RIGHT			27
#define MARIO_ANI_SMALL_KICK_LEFT			28
#define MARIO_ANI_TAIL_KICK_RIGHT			29
#define MARIO_ANI_TAIL_KICK_LEFT			30

#define MARIO_ANI_IDLE_BIG_HOLD_RIGHT		31
#define MARIO_ANI_IDLE_BIG_HOLD_LEFT		32
#define MARIO_ANI_IDLE_TAIL_HOLD_RIGHT		33
#define MARIO_ANI_IDLE_TAIL_HOLD_LEFT		34
#define MARIO_ANI_WALKING_BIG_HOLD_RIGHT	35
#define MARIO_ANI_WALKING_BIG_HOLD_LEFT		36
#define MARIO_ANI_WALKING_TAIL_HOLD_RIGHT	37
#define MARIO_ANI_WALKING_TAIL_HOLD_LEFT	38

#define MARIO_ANI_SWING_RIGHT				39
#define MARIO_ANI_SWING_LEFT				40	

#define MARIO_ANI_IDLE_FIRE_RIGHT			41
#define MARIO_ANI_IDLE_FIRE_LEFT			42
#define MARIO_ANI_WALKING_FIRE_RIGHT		43
#define MARIO_ANI_WALKING_FIRE_LEFT			44

#define MARIO_ANI_FLY_RIGHT					45
#define MARIO_ANI_FLY_LEFT					46
#define MARIO_ANI_BE_FALL_RIGHT				47
#define MARIO_ANI_BE_FALL_LEFT				48

#define MARIO_ANI_JUMPING_FIRE_RIGHT		49
#define MARIO_ANI_JUMPING_FIRE_LEFT			50

#define MARIO_ANI_WAGGING_RIGHT				51
#define MARIO_ANI_WAGGING_LEFT				52


#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_LEVEL_TAIL	3
#define MARIO_LEVEL_FIRE	4
#define MARIO_LEVEL_FLY		5

#define ANIMATION_SET_1		1
#define ANIMATION_SET_2		100

#define MARIO_BIG_BBOX_WIDTH  15
#define MARIO_BIG_BBOX_HEIGHT 27

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 15

#define MARIO_TAIL_BBOX_WIDTH  23
#define MARIO_TAIL_BBOX_HEIGHT 27
#define MARIO_TAIL_SWING_BBOX_WIDTH	34

#define MARIO_UNTOUCHABLE_TIME	4000
#define MARIO_FLY_TIME			4000	
#define MARIO_MOMENTUM_TIME		2000
#define MARIO_KICKING_TIME		200
#define MARIO_SWING_TIME		400

const float PLAYER_MAX_JUMP_VELOCITY = 0.5f; //van toc nhay lon nhat
const float PLAYER_MIN_JUMP_VELOCITY = -0.5f; //van toc nhay thap nhat
const float PLAYER_MAX_RUNNING_SPEED = 0.15; //toc do chay nhanh nhat cua player
const float PLAYER_BOTTOM_RANGE_FALLING = 6.0f; // do dai va cham voi bottom neu nhu va cham

class Mario: public GameObject
{
	static Mario* instance;

public:
	int level;

	int untouchable;
	DWORD untouchable_start;

	int flyable;
	DWORD flyable_start;

	int momentable;
	DWORD momentable_start;

	int kicking;
	DWORD kicking_start;

	int swing;
	DWORD swing_start;

	float start_x;
	float start_y;

	int direct;

public:

	bool isTurnToSmall;
	bool isTurnToBig;
	bool isTurnToTail;

	bool isTurnRight;
	bool isTurnLeft;

	bool isPressed;	
	bool isAllowKick;
	bool isAllowHold;
	bool isHoldingItem;
	bool isAllowMoment;
	bool isAllowSwing;
	
	bool isAutoGo;
	bool isUseFire;
	bool isSwing;
	bool isFlying;
	bool isWagging;
	bool isJumping;

	bool noCollision;
	bool collision_x;
	bool collision_y;
	bool isCollisionOnAxisY;

	bool isMomentum;

	static Mario* GetInstance();
	Mario(float x = 0.0f, float y = 0.0f);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);

	void SetLevel(int lev) { level = lev; }
	int GetLevel();

	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void StartFlyable() { isFlying = true; flyable_start = GetTickCount(); level = MARIO_LEVEL_FLY; }
	void StartMomentum() { isMomentum = true; momentable = 1; momentable_start = GetTickCount(); }
	void StartKick() { kicking = 1; kicking_start = GetTickCount(); }
	void StartSwing() { swing = 1; swing_start = GetTickCount(); }

	void Reset();

	int GetAni();
	void SetAni(int ani);

	void changeAni();
	

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);


	void NoCollisionWithAxisY();

	bool isCollisionWithItem(LPGAMEOBJECT item);

	void SetHodingItem(LPGAMEOBJECT item);

	bool allowMoveLeft;
	bool allowMoveRight;

protected:

	bool allowJump;

	int currentAni;	

};

