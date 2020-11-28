#include "HidenWall.h"
#include "Brick.h"

void HidenWall::Render()
{
	animation_set->at(0)->Render(x, y);
}

void HidenWall::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BRICK_BBOX_WIDTH;
	bottom = y + BRICK_BBOX_HEIGHT;
}
