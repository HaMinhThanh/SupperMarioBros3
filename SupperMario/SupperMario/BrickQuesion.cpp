#include "BrickQuesion.h"


BrickQuesion::BrickQuesion(float x, float y, int itm)
{
	isFinish = false;
	SetPosition(x, y);
	item = itm;

	backupX = x;
	backupY = y;

	isMovingUp = false;
	isMovingDown = false;
}

void BrickQuesion::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isMovingDown)
	{
		y -= 1;
	}

	if (isMovingUp)
	{
		y += 1;
	}

	if (y <= backupY - 5)
	{
		isMovingDown = false;
		isMovingUp = true;
	}

	if (y > backupY)
	{
		y = backupY;

		isMovingUp = false;
	}
}

void BrickQuesion::Render()
{
	if (isFinish) { 
		animation_set->at(1)->Render(x, y); 
		return;
	}
	animation_set->at(0)->Render(x, y);
}

void BrickQuesion::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	/*if (isFinish) {
		left = top = right = bottom = 0;
		return;
	}*/

	left = x;
	top = y;
	right = x + BRICKQUESION_BBOX_WIDTH;
	bottom = y + BRICKQUESION_BBOX_HEIGHT;
}
