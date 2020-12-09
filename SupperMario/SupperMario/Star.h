#pragma once

#include "GameObject.h"

#define STAR_BBOX_WIDTH		16
#define STAR_BBOX_HEIGHT	16	

#define STAR_STATE_FINISH	1
#define STAR_STATE_ACTIVE	2

class Star: public GameObject
{
public:

public:
	int item;

	int fly;
	DWORD fly_start;
	void Start_Fly() { fly = 1; fly_start = GetTickCount(); }

	Star();
	~Star();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

