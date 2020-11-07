#include "Leaf.h"

Leaf::Leaf()
{
	isFinish = false();
}

Leaf::~Leaf()
{

}

void Leaf::Render()
{
	if (isFinish) return;
	animation_set->at(0)->Render(x, y);
}

void Leaf::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isFinish) {
		left = top = right = bottom = 0;
		return;
	}

	left = x;
	top = y;
	right = x + LEAF_BBOX_WIDTH;
	bottom = y + LEAF_BBOX_HEIGHT;
}

