#pragma once
#include "GameObject.h"

#define MOVINGBRICK_BBOX_WIDTH		48
#define MOVINGBRICK_BBOX_HEIGHT		16

#define MOVINGBRICK_TIME_PREPARE_FALLING	200
#define MOVINGBRICK_TIME_FINISH				4000

class MovingBrick: public GameObject
{
public:
	MovingBrick();
	~MovingBrick();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	DWORD time_falling;
	int falling;
	bool isFalling;

	void StartFalling() { falling = 1; time_falling = GetTickCount(); }
};

