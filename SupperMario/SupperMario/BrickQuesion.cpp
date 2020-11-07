#include "BrickQuesion.h"


BrickQuesion::BrickQuesion()
{
	isFinish = false;
}

BrickQuesion::~BrickQuesion()
{

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
