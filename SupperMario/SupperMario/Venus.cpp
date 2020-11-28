#include "Venus.h"

Venus::Venus()
{
	fire = new Fire();

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
	

	if (y < VENUS_MIN_Y) 
	{
		vy = VENUS_SPEED_Y;
	}
	else if (y > VENUS_MAX_Y) 
	{
		vy = -VENUS_SPEED_Y;
	}
	else if (y == VENUS_MIN_Y)
	{
		if (untouchable == 0) {
			StartFire();
		}
	}
	else if (y == VENUS_MAX_Y)
	{
		fire->SetPosition(this->x, this->y);
		if (untouchable == 0) {
			StartFire();
			fire->SetPosition(this->x, this->y);
		}
	}

	fire->Update(dt, coObjects);
}

void Venus::Render()
{
	int ani = VENUS_ANI_IDLE_TOP_LEFT;

	int state = GetState();

	if (state == VENUS_STATE_DIE)
	{
		return;
	}
	switch (state)
	{
	case VENUS_STATE_TOP_LEFT:
		ani = VENUS_ANI_OPEN_TOP_LEFT;
		break;
	case VENUS_STATE_TOP_RIGHT:
		ani = VENUS_ANI_OPEN_TOP_RIGHT;
		break;
	case VENUS_STATE_BOT_LEFT:
		ani = VENUS_ANI_OPEN_BOT_LEFT;
		break;
	case VENUS_STATE_BOT_RIGHT:
		ani = VENUS_ANI_OPEN_BOT_RIGHT;
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
	case VENUS_STATE_DIE:
		fire->SetState(FIRE_HIDEN);
		break;
	}
}

