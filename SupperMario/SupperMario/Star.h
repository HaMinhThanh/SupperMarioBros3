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
	Star();
	~Star();

	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

