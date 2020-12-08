#pragma once

#include "GameObject.h"

#define	COIN_BBOX_WIDTH 16
#define COIN_BBOX_HEIGHT 16

class Coin: public GameObject
{
public:
	bool isNoCollision;

	int fly;
	DWORD fly_time;

public:
	Coin();
	~Coin();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void coinWithBrickquesion(float x);

	void StartFly() { fly = 1; fly_time = GetTickCount(); }

};

