#pragma once
#include "GameObject.h"

#define BUTTON_BBOX_WIDTH	16
#define BUTTON_BBOX_HEIGHT	16

class Button: public GameObject
{
public:
	Button();
	~Button();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

