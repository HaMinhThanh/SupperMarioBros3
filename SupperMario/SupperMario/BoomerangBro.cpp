#include "BoomerangBro.h"

BoomerangBro::BoomerangBro()
{
	isFinish = false;

	boomr1 = new Boomerang();
	boomr2 = new Boomerang();
}

BoomerangBro::~BoomerangBro()
{
}

void BoomerangBro::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	GameObject::Update(dt, coObjects);

	x += dx;
	y += dy;

	if (!boomr1->isBoom)
	{
		boomr1->SetPosition(x + 16, y - 10);
		if (vx > 0 || nx > 0)
		{

			boomr1->direct = 1;
			boomr1->isBoom = true;
		}
		else
		{
			boomr1->direct = -1;
			boomr1->isBoom = true;
		}
	}	

	boomr1->Update(dt, coObjects);
	//boomr2->Update(dt, coObjects);
}

void BoomerangBro::Render()
{
	if (vx > 0 || nx > 0)
	{
		animation_set->at(0)->Render(x, y);
	}
	else
	{
		animation_set->at(1)->Render(x, y);
	}

	if (boomr1 != NULL)
	{
		boomr1->Render();
	}

	if (boomr2 != NULL)
	{
		boomr2->Render();
	}
}

void BoomerangBro::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BOOMERANGBRO_BBOX_WIDTH;
	bottom = y + BOOMERANGBRO_BBOX_HEIGHT;
}

void BoomerangBro::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case BOOMERANGBRO_STATE_WALKING_RIGHT:
		break;
	case BOOMERANGBRO_STATE_WALKING_LEFT:
		break;
	case BOOMERANGBRO_STATE_JUMPING:
		break;
	case BOOMERANGBRO_STATE_DIE:
		break;
	}
}
