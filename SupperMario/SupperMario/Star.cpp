#include "Star.h"

Star::Star()
{
	isFinish = false;
}

Star::~Star()
{

}

void Star::Render()
{
	if (isFinish) return;
	animation_set->at(0)->Render(x, y);
}

void Star::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isFinish) {
		left = top = right = bottom = 0;
		return;
	}

	left = x;
	top = y;
	right = x + STAR_BBOX_WIDTH;
	bottom = y + STAR_BBOX_HEIGHT;
}
