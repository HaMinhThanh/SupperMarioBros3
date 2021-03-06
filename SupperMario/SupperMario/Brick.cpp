#include "Brick.h"

void Brick::Render()
{
	animation_set->at(0)->Render(x, y);
}

void Brick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BRICK_BBOX_WIDTH;
	bottom = y + BRICK_BBOX_HEIGHT;
}
