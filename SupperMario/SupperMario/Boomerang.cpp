#include "Boomerang.h"
#include "mario.h"

Boomerang::Boomerang()
{
	SetAnimationSet(AnimationSets::GetInstance()->Get(46));

	time_boomerang = 0;
	boomerang = 0;
	isBoom = false;
	isFinish = true;
}

Boomerang::~Boomerang()
{

}

void Boomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isBoom)
	{
		if (boomerang == 0)
		{
			StartBoomerang();
		}
	}

	if (boomerang == 1)
	{
		if (GetTickCount() - time_boomerang > 5500)
		{
			vx = vy = 0;
			isFinish = true;
			isBoom = false;
			time_boomerang = 0;
			boomerang = 0;
		}
		else if (GetTickCount() - time_boomerang > 3000)
		{
			if (direct == 1)
			{
				vx = -0.04;
				vy = 0.001;
			}
			else
			{
				vx = 0.04f;
				vy = 0.001f;
			}
		}
		else if (GetTickCount() - time_boomerang > 2500)
		{
			if (direct == 1)
			{
				vx = -0.01;
				vy = 0.005;
			}
			else
			{
				vx = 0.01f;
				vy = 0.005;
			}
		}
		else if (GetTickCount() - time_boomerang > 2000)
		{
			if (direct == 1)
			{
				vx = 0.01;
				vy = 0.005;
			}
			else
			{
				vx = -0.01f;
				vy = 0.005;
			}
		}
		else
		{
			if (direct == 1)
			{
				vx = 0.04;
				vy = -0.0001;
			}
			else
			{
				vx = -0.04f;
				vy = -0.0001;
			}
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
	/*if (isFinish)
	{
		return;
	}*/

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
	if (isBoom)
	{
		left = x;
		top = y;
		right = x + BOOMERANG_BBOX_WIDHT;
		bottom = y + BOOMERANG_BBOX_HEIGHT;
	}
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
