#include "Fire.h"
#include "FireBall.h"
#include "Brick.h"

Fire::Fire()
{
	AnimationSets* animation_sets = AnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(35);

	this->SetAnimationSet(ani_set);
	//StartUnFinish();

	isFinish = false;
}

Fire::~Fire()
{

}

void Fire::Render()
{
	if (isFinish) return;

	animation_set->at(0)->Render(x, y);
}

void Fire::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isFinish) {
		left = top = right = bottom = 0;
		return;
	}

	left = x;
	top = y;
	right = x + FIREBALL_BBOX_WIDTH;
	bottom = y + FIREBALL_BBOX_HEIGHT;
}

void Fire::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	GameObject::Update(dt, coObjects);

	x += dx;
	y += dy;

}

void Fire::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case FIRE_HIDEN:
		isFinish = true;
		vx = vy = 0;
		break;
	case FIRE_TOP:
		vx = 0;
		vy = -0.01;
		break;
	case FIRE_TOP_LEFT:
		vx = -0.02;
		vy = -0.01;
		break;
	case FIRE_TOP_RIGHT:
		vx = 0.02;
		vy = -0.01;
		break;
	case FIRE_BOT_LEFT:
		vx = -0.02;
		vy = 0.01;
		break;
	case FIRE_BOT_RIGHT:
		vx = 0.02;
		vy = 0.01;
		break;
	}
}



