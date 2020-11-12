#include "BrickColor.h"

#include "Brick.h"


void BrickColor::Render()
{
	//animation_set->at(0)->Render(x, y);
}

void BrickColor::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = w;
	bottom = h;
}

void BrickColor::SetBoundingBox(float w, float h)
{
	this->w = w;
	this->h = h;
}
