#include "Venus.h"

Venus::Venus(float Max, float Min)
{
	fire = new Fire();
	//fire->SetPosition(this->x, this->y);

	max = Max;
	min = Min;

	untouchable = 0;
	SetState(VENUS_STATE_BOT_LEFT);
	vx = 0;
	vy = VENUS_SPEED_Y;
}

void Venus::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + VENUS_BBOX_WIDTH;
	bottom = y + VENUS_BBOX_HEIGHT;
}

void Venus::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	GameObject::Update(dt, coObjects);

	x += dx;
	y += dy;

	if (GetTickCount() - untouchable_start > 750) {
		untouchable = 0;
		untouchable_start = 0;
	}


	if (y >= max)
	{
		vy = -VENUS_SPEED_Y;
	}
	else if (y <= min)
	{
		fire->SetPosition(this->x, this->y);
		int state = GetState();

		if (state == VENUS_STATE_DIE)
		{
			fire->isFinish = true;
		}
		switch (state)
		{
		case VENUS_STATE_TOP_LEFT:
			fire->SetState(FIRE_TOP_LEFT);
			break;
		case VENUS_STATE_TOP_RIGHT:
			fire->SetState(FIRE_TOP_RIGHT);
			break;
		case VENUS_STATE_BOT_LEFT:
			fire->SetState(FIRE_BOT_LEFT);
			break;
		case VENUS_STATE_BOT_RIGHT:
			fire->SetState(FIRE_BOT_RIGHT);
			break;
		}
		vy = VENUS_SPEED_Y;
	}

	fire->Update(dt, coObjects);

	Mario* mario = Mario::GetInstance(0, 0);
	if (mario->isCollisionWithItem(fire))
	{
		if (mario->untouchable == 0)
		{
			if (mario->level > MARIO_LEVEL_SMALL)
			{
				mario->level--;
				mario->StartUntouchable();
			}
			else
				mario->SetState(MARIO_STATE_DIE);
		}

		SetState(FIRE_HIDEN);
	}
}

void Venus::Render()
{
	int ani = VENUS_ANI_TOP_LEFT;

	int state = GetState();

	if (state == VENUS_STATE_DIE)
	{
		return;
	}
	switch (state)
	{
	case VENUS_STATE_TOP_LEFT:
		ani = VENUS_ANI_TOP_LEFT;
		break;
	case VENUS_STATE_TOP_RIGHT:
		ani = VENUS_ANI_TOP_RIGHT;
		break;
	case VENUS_STATE_BOT_LEFT:
		ani = VENUS_ANI_BOT_LEFT;
		break;
	case VENUS_STATE_BOT_RIGHT:
		ani = VENUS_ANI_BOT_RIGHT;
		break;
	}


	animation_set->at(ani)->Render(x, y);
	fire->Render();

	//RenderBoundingBox();
}
void Venus::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case VENUS_STATE_TOP_LEFT:
		//fire->SetState(FIRE_TOP_LEFT);
		break;
	case VENUS_STATE_TOP_RIGHT:
		//fire->SetState(FIRE_TOP_RIGHT);
		break;
	case VENUS_STATE_BOT_LEFT:
		//fire->SetState(FIRE_BOT_LEFT);
		break;
	case VENUS_STATE_BOT_RIGHT:
		//fire->SetState(FIRE_BOT_RIGHT);
		break;
	case VENUS_STATE_DIE:
		//fire->SetState(FIRE_HIDEN);
		break;
	}
}

