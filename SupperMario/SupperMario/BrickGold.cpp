#include "BrickGold.h"


BrickGold::BrickGold(float x, float y, int itm)
{
	isFinish = false;
	SetPosition(x, y);
	item = itm;
}

void BrickGold::Render()
{
	if (isFinish) {
		//animation_set->at(1)->Render(x, y);
		return;
	}
	animation_set->at(0)->Render(x, y);
}

void BrickGold::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isFinish) {
		left = top = right = bottom = 0;
		return;
	}
	left = x;
	top = y;
	right = x + BRICKGOLD_BBOX_WIDTH;
	bottom = y + BRICKGOLD_BBOX_HEIGHT;
}
