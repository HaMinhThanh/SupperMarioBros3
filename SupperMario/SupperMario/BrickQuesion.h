#pragma once

#include "GameObject.h"

#define BRICKQUESION_BBOX_WIDTH	16
#define BRICKQUESION_BBOX_HEIGHT	16


class BrickQuesion: public GameObject
{
public:
	BrickQuesion();
	~BrickQuesion();

	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

