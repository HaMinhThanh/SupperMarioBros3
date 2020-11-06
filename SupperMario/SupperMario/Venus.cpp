#include "Venus.h"

Venus::Venus()
{
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
	

	if (y <= VENUS_MIN_Y) {
		vy = VENUS_SPEED_Y;
	}
	else if (y >= VENUS_MAX_Y)
		vy = -VENUS_SPEED_Y;


}

void Venus::Render()
{
	int ani = VENUS_ANI_IDLE_TOP_LEFT;

	if (state == VENUS_STATE_DIE)
	{
		return;
	}
	

	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}
void Venus::SetState(int state)
{
	GameObject::SetState(state);

	/*switch (state)
	{
	case VENUS_STATE_TOP_LEFT:
	case VENUS_STATE_TOP_RIGHT:
	case VENUS_STATE_BOT_LEFT:
	case VENUS_STATE_BOT_RIGHT:
	}*/
}

