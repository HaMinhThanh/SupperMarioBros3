#pragma once

#include "GameObject.h"

#define	COIN_BBOX_WIDTH 16
#define COIN_BBOX_HEIGHT 16

class Coin: public GameObject
{
public:

public:
	Coin();
	~Coin();

	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

};

