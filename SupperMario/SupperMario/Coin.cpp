#include "Coin.h"

Coin::Coin()
{
	isFinish = false;
	isNoCollision = false;
	vx = 0;
	vy = 0;

	fly = 0;
}

Coin::~Coin()
{

}

void Coin::Render()
{
	if (isFinish == false)
		animation_set->at(0)->Render(x, y);
}

void Coin::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	/*if (isFinish) {
		left = top = right = bottom = 0;
		return;
	}*/
	if (isFinish == false) {

		left = x;
		top = y;
		right = x + COIN_BBOX_WIDTH;
		bottom = y + COIN_BBOX_HEIGHT;
	}
}

void Coin::coinWithBrickquesion(float max) {
	max = y - 30.0f;
	y -= 0.01f;

	if (y <= max)
	{
		y += 0.1f;
	}
	if (y >= y + 0.1f)
	{
		isFinish = true;
		
	}
}

void Coin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	
	if (isNoCollision) {
		if (fly == 0)
			StartFly();
		isNoCollision = false;
	}

	if (fly == 1) {
		if (GetTickCount() - fly_time < 500) {
			vy = -0.15f;
		}
		else if (GetTickCount() - fly_time >= 500 && GetTickCount() - fly_time < 700) {
			vy = 0.1f;
		}
		else {
			fly = 0;
			fly_time = 0;
			vy = 0;
			isFinish = true;
		}
	}
	
	GameObject::Update(dt, coObjects);

	x += dx;
	y += dy;

}