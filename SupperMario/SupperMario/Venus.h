#pragma once

#include "GameObject.h"
#include "mario.h"
#include "Fire.h"

#define VENUS_SPEED_Y	0.015f

#define VENUS_ANI_TOP_LEFT		0
#define VENUS_ANI_TOP_RIGHT	1
#define VENUS_ANI_BOT_LEFT	2
#define VENUS_ANI_BOT_RIGHT		3
#define VENUS_ANI_OPEN_TOP_LEFT		4	
#define VENUS_ANI_OPEN_TOP_RIGHT	5
#define	VENUS_ANI_OPEN_BOT_LEFT		6
#define VENUS_ANI_OPEN_BOT_RIGHT	7

#define VENUS_BBOX_WIDTH	16
#define VENUS_BBOX_HEIGHT	32

#define VENUS_STATE_TOP_RIGHT	100
#define VENUS_STATE_TOP_LEFT	200
#define VENUS_STATE_BOT_RIGHT	300
#define VENUS_STATE_BOT_LEFT	400
#define VENUS_STATE_TOP			500	
#define VENUS_STATE_DIE			900	


class Venus: public GameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	float max, min;
	int setFire;
	Venus(float max, float min, int fire);

	virtual void SetState(int state);

	Fire* fire;

	int isFire;

	bool isNormal;

	int idle;
	DWORD idle_start;
	void StartFire() { idle = 1; idle_start = GetTickCount(); }
};

