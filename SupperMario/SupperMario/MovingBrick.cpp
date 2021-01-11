#include "MovingBrick.h"
#include "Brick.h"

MovingBrick::MovingBrick()
{
	isFinish = false;

	vx = 0;
	vy = 0;
}

MovingBrick::~MovingBrick()
{
}

void MovingBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isFalling)
	{
		if (falling == 0)
			StartFalling();

		isFalling = false;
	}

	if (falling == 1)
	{
		if (GetTickCount() - time_falling > MOVINGBRICK_TIME_FINISH)
		{
			isFinish = true;

			falling = 0;
			time_falling = 0;
		}
		else if (GetTickCount() - time_falling > MOVINGBRICK_TIME_PREPARE_FALLING)
		{
			vy = 0.05f;		
		}
	}

	GameObject::Update(dt, coObjects);

	x += dx;
	y += dy;
}

void MovingBrick::Render()
{
	if (isFinish == false)
	{
		animation_set->at(0)->Render(x, y);
	}
}

void MovingBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isFinish == false)
	{
		left = x;
		top = y;
		right = x + MOVINGBRICK_BBOX_WIDTH;
		bottom = y + MOVINGBRICK_BBOX_HEIGHT;
	}
}
