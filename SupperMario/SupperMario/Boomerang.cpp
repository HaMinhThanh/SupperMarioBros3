#include "Boomerang.h"
#include "mario.h"

Boomerang::Boomerang()
{
	SetAnimationSet(AnimationSets::GetInstance()->Get(46));

	time_boomerang = 0;
	boomerang = 0;
	isBoom = false;
}

Boomerang::~Boomerang()
{

}

void Boomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (boomerang == 1)
	{
		if (GetTickCount() - time_boomerang > 5000)
		{

		}
		else if (GetTickCount() - time_boomerang > 4500)
		{

		}
		else if (GetTickCount() - time_boomerang > 4000)
		{

		}
		else if (GetTickCount() - time_boomerang > 5000)
		{

		}
		else if (GetTickCount() - time_boomerang > 5000)
		{

		}
	}

	GameObject::Update(dt, coObjects);

	x += dx;
	y += dy;

	Mario* mario = Mario::GetInstance(0, 0);

	if (isCollisionWithObject(mario))
	{
		if (mario->untouchable == 0)
		{
			if (mario->level > MARIO_LEVEL_SMALL)
			{
				mario->level -= 1;
				mario->StartUntouchable();
			}
			else
			{
				mario->SetState(MARIO_STATE_DIE);
			}				
		}
	}
}

void Boomerang::Render()
{
	if (direct == 1)
	{
		animation_set->at(0)->Render(x, y);
	}
	else
	{
		animation_set->at(1)->Render(x, y);
	}
}

void Boomerang::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BOOMERANG_BBOX_WIDHT;
	bottom = y + BOOMERANG_BBOX_HEIGHT;
}

void Boomerang::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case BOOMERANG_STATE_RIGHT:
		break;
	case BOOMERANG_STATE_LEFT:
		break;
	case BOOMERANG_STATE_FINISH:
		break;
	}
}
