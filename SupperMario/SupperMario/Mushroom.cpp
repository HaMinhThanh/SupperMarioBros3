#include "Mushroom.h"

Mushroom::Mushroom()
{
	isFinish = false();
}

Mushroom::~Mushroom()
{

}

void Mushroom::Render()
{
	if (isFinish) return;
	animation_set->at(0)->Render(x, y);
}

void Mushroom::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isFinish) {
		left = top = right = bottom = 0;
		return;
	}

	left = x;
	top = y;
	right = x + MUSHROOM_BBOX_WIDTH;
	bottom = y + MUSHROOM_BBOX_HEIGHT;
}
