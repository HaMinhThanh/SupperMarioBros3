#include "BrickGreen.h"
#include "Brick.h"

void BrickGreen::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

}

void BrickGreen::Render()
{
	animation_set->at(0)->Render(x, y);
}

void BrickGreen::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BRICK_BBOX_WIDTH;
	bottom = y + BRICK_BBOX_HEIGHT;
}
