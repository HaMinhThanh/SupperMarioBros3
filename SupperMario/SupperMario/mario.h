#pragma once
#include <d3dx9.h>
#include "GameObject.h"
#include "Animations.h"
#include "Camera.h"

#include "BrickColor.h"
#include "Node.h"

#define MARIO_WALKING_SPEED		0.1f 
#define MARIO_WALKING_MAX_SPEED	0.15f

//0.1f
#define MARIO_JUMP_SPEED_Y		0.32f
#define MARIO_JUMP_MAX_SPEED	0.5f;
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

// Kick koopa
#define MARIO_ANI_BIG_KICK_RIGHT			25
#define MARIO_ANI_BIG_KICK_LEFT				26
#define MARIO_ANI_SMALL_KICK_RIGHT			27
#define MARIO_ANI_SMALL_KICK_LEFT			28
#define MARIO_ANI_TAIL_KICK_RIGHT			29
#define MARIO_ANI_TAIL_KICK_LEFT			30

// Hold Koopa
#define MARIO_ANI_IDLE_BIG_HOLD_RIGHT		31
#define MARIO_ANI_IDLE_BIG_HOLD_LEFT		32
#define MARIO_ANI_IDLE_TAIL_HOLD_RIGHT		33
#define MARIO_ANI_IDLE_TAIL_HOLD_LEFT		34
#define MARIO_ANI_WALKING_BIG_HOLD_RIGHT	35
#define MARIO_ANI_WALKING_BIG_HOLD_LEFT		36
#define MARIO_ANI_WALKING_TAIL_HOLD_RIGHT	37
#define MARIO_ANI_WALKING_TAIL_HOLD_LEFT	38

// Swing
#define MARIO_ANI_SWING_RIGHT				39
#define MARIO_ANI_SWING_LEFT				40	

// Fire level
#define MARIO_ANI_IDLE_FIRE_RIGHT			41
#define MARIO_ANI_IDLE_FIRE_LEFT			42
#define MARIO_ANI_WALKING_FIRE_RIGHT		43
#define MARIO_ANI_WALKING_FIRE_LEFT			44

// Fly
#define MARIO_ANI_FLY_RIGHT					45
#define MARIO_ANI_FLY_LEFT					46
#define MARIO_ANI_BE_FALL_RIGHT				47
#define MARIO_ANI_BE_FALL_LEFT				48

// Jumping
#define MARIO_ANI_JUMPING_FIRE_RIGHT		49
#define MARIO_ANI_JUMPING_FIRE_LEFT			50

// Wagging
#define MARIO_ANI_WAGGING_RIGHT				51
#define MARIO_ANI_WAGGING_LEFT				52

// Crouch
#define MARIO_ANI_BIG_CROUCH_RIGHT			53
#define MARIO_ANI_BIG_CROUCH_LEFT			54
#define MARIO_ANI_TAIL_CROUCH_RIGHT			55
#define MARIO_ANI_TAIL_CROUCH_LEFT			56

#define MARIO_ANI_COLLISION_MUSHROOM		57

// Speed up
#define MARIO_ANI_SPEED_UP_RIGHT			58
#define MARIO_ANI_SPEED_UP_LEFT				59

// Mario Drift Ani
#define MARIO_ANI_SMALL_DRIFT_RIGHT			60
#define MARIO_ANI_SMALL_DRIFT_LEFT			61
#define MARIO_ANI_BIG_DRIFT_RIGHT			62
#define MARIO_ANI_BIG_DRIFT_LEFT			63
#define MARIO_ANI_TAIL_DRIFT_RIGHT			64
#define MARIO_ANI_TAIL_DRIFT_LEFT			65

// Mario speed up Small && Big
#define MARIO_ANI_SPEED_UP_SMALL_RIGHT		66
#define MARIO_ANI_SPEED_UP_SMALL_LEFT		67
#define MARIO_ANI_SPEED_UP_BIG_RIGHT		68
#define MARIO_ANI_SPEED_UP_BIG_LEFT			69

// Mario fly Small && Big
#define MARIO_ANI_FLY_SMALL_RIGHT			71	
#define MARIO_ANI_FLY_SMALL_LEFT			73
#define MARIO_ANI_BE_FALL_SMALL_RIGHT		70	
#define MARIO_ANI_BE_FALL_SMALL_LEFT		72
#define MARIO_ANI_FLY_BIG_RIGHT				75
#define MARIO_ANI_FLY_BIG_LEFT				77
#define MARIO_ANI_BE_FALL_BIG_RIGHT			74
#define MARIO_ANI_BE_FALL_BIG_LEFT			76

// Prepare speed up
#define MARIO_ANI_PREPARE_SPEED_UP_BIG_RIGHT	78			
#define MARIO_ANI_PREPARE_SPEED_UP_BIG_LEFT		79
#define MARIO_ANI_PREPARE_SPEED_UP_SMALL_RIGHT	80
#define MARIO_ANI_PREPARE_SPEED_UP_SMALL_LEFT	81
#define MARIO_ANI_PREPARE_SPEED_UP_TAIL_RIGHT	82
#define MARIO_ANI_PREPARE_SPEED_UP_TAIL_LEFT	83


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
#define MARIO_TAIL_SWING_BBOX_WIDTH_LEFT 9

#define MARIO_CROUCH_BBOX_HEIGHT	18

#define MARIO_UNTOUCHABLE_TIME	2000
#define MARIO_FLY_TIME			4000	
#define MARIO_MOMENTUM_TIME		2000
#define MARIO_KICKING_TIME		200
#define MARIO_SWING_TIME		300


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

	int collisWithMushroom = 0;
	DWORD collisWithMushroom_start;

	int waitToReset;
	DWORD time_to_reset;

	float start_x;
	float start_y;

	int direct;

	// HUD
	int score;
	int dola;
	int live;

	int item;
	int numItem;

	float metter;

	// 
	bool isCrouch;
	int isTurnBack = 0;
	int backupLevel;

	bool isCollisionWithMushroom;

	//
	bool isPrepareSpeedUp;
	bool isSpeedUp;
	bool isComeUp = false;

	// jump max
	bool isMaxJumping;
	int maxjumping;
	DWORD time_maxjumping;

	void StartJumpingMax() { maxjumping = 1; time_maxjumping = GetTickCount(); }

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
	bool isPowerUp;

	bool noCollision;
	bool collision_x;
	bool collision_y;
	bool isCollisionOnAxisY;

	bool isMomentum;

	bool isDecreaseX;
	bool isDecreaseY;
	bool isTurnBackX;
	bool isTurnBackY;

	static Mario* GetInstance(float x, float y);
	Mario(float x = 0.0f, float y = 0.0f);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);

	void SetLevel(int lev) { level = lev; }
	int GetLevel();

	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void StartFlyable() { isFlying = true; flyable_start = GetTickCount(); level = MARIO_LEVEL_FLY; }
	void StartMomentum() { isMomentum = true; momentable = 1; momentable_start = GetTickCount(); }
	void StartKick() { isAllowKick = true; kicking = 1; kicking_start = GetTickCount(); }
	void StartSwing() { isDecreaseX = true; swing = 1; swing_start = GetTickCount(); }
	void StartCollisionWithMushroom() { isCollisionWithMushroom = true; collisWithMushroom = 1; collisWithMushroom_start = GetTickCount(); }
	void StartReset() { waitToReset = 1; time_to_reset = GetTickCount(); }

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

	// HUD
	int GetScore() { return score; }
	int GetDola() { return dola; }
	int GetLive() { return live; }

	int GetItem() { return item; }
	void SetItem(int itm) { item = itm; }

	void LoadItem(int itm);

	bool allowJump;

	int currentAni;	

	// world map
	bool isGoUp;
	bool isGoDown;
	bool isGoLeft;
	bool isGoRight;

	bool isNoWeight;
	bool isCollision = false;

	bool isDrift = false;
	DWORD time_drift;
	int drift;

	void StartDrift() { isDrift = true; drift = 1; time_drift = GetTickCount(); }

	Node* currentNode;
	Node* GetCurentNode() { return currentNode; }
	void SwitchMap();
};

