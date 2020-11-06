#include "Coin.h"

Coin::Coin()
{
	isFinish = false;
}

Coin::~Coin()
{

}

void Coin::Render()
{
	if (isFinish) return;
	animation_set->at(0)->Render(x, y);
}

void Coin::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isFinish) {
		left = top = right = bottom = 0;
		return;
	}

	left = x;
	top = y;
	right = x + COIN_BBOX_WIDTH;
	bottom = y + COIN_BBOX_HEIGHT;
}
