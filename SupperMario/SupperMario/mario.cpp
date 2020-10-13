#include "Game.h"
#include "Mario.h"
#include "Sprites.h"

#include "Goomba.h"

//Mario::Mario(float x, float y, float vx) :GameObject(x, y)
//{
//	this->vx = vx;
//};


void Mario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	GameObject::Update(dt);

	vy += MARIO_GRAVITY;
	if (y > 100)
	{
		vy = 0;
		y = 100.0f;
	}

	if (vx > 0 && x > 640) x = 640;
	if (vx < 0 && x < 0)x = 0;
	/*x += vx * dt;

	int BackBufferWidth = Game::GetInstance()->GetBackBufferWidth();
	if (x <= 0 || x >= BackBufferWidth - MARIO_WIDTH) {

		vx = -vx;

		if (x <= 0)
		{
			x = 0;
		}
		else if (x >= BackBufferWidth - MARIO_WIDTH)
		{
			x = (float)(BackBufferWidth - MARIO_WIDTH);
		}
	}*/
}

void Mario::Render()
{
	//LPANIMATION ani;
	//
	////ani = Animations::GetInstance()->Get(510);

	//if (vx > 0) 
	//	ani = Animations::GetInstance()->Get(500);
	//else 
	//	ani = Animations::GetInstance()->Get(501);

	//ani->Render(x, y);
	
	int ani;
	if (state == MARIO_STATE_DIE)
		ani = MARIO_ANI_DIE;
	else if (level == MARIO_LEVEL_BIG) {
		if (vx == 0)
		{
			if (nx > 0)ani = MARIO_ANI_BIG_IDLE_RIGHT;
			else ani = MARIO_ANI_BIG_IDLE_LEFT;
		}
		else if (vx > 0)
			ani = MARIO_ANI_BIG_WALKING_RIGHT;
		else ani = MARIO_ANI_BIG_WALKING_LEFT;
	}
	else if (level == MARIO_LEVEL_SMALL)
	{
		if (vx == 0)
		{
			if (nx > 0)ani = MARIO_ANI_SMALL_IDLE_RIGHT;
			else ani = MARIO_ANI_SMALL_IDLE_LEFT;
		}
		else if (vx > 0)
			ani = MARIO_ANI_SMALL_WALKING_RIGHT;
		else ani = MARIO_ANI_SMALL_WALKING_LEFT;
	}
	int alpha = 255;
	if (untouchable) alpha = 128;

	animations[ani]->Render(x, y);

	RenderBoundingBox();
}

void Mario::SetState(int  state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		vx = MARIO_WALKING_SPEED;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		vx = -MARIO_WALKING_SPEED;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (y == 100)
			vy = -MARIO_JUMP_SPEED_Y;
	case MARIO_STATE_IDLE:
		vx = 0;
		break;
	}
}

