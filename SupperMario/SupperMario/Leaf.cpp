#include "Leaf.h"

void Leaf::Render()
{
	animation_set->at(0)->Render(x, y);
}

void Leaf::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + LEAF_BBOX_WIDTH;
	bottom = y + LEAF_BBOX_HEIGHT;
}
