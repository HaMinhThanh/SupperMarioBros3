#include "HidenWall.h"
#include "Brick.h"

HidenWall::HidenWall()
{
	vx = vy = 0;

	isFinish = false;
}

HidenWall::HidenWall(float x, float y, int type)
{
	isFinish = false;
	SetPosition(x, y);

	this->type = type;
	if (type == 1)
		vy = -0.1f;
	else if (type == 2)
		vy = 0.1f;
	vx = 0;
}

HidenWall::~HidenWall()
{

}

void HidenWall::Update(DWORD dt, vector<LPGAMEOBJECT> coObjects)
{	
	GameObject::Update(dt);

	x += dx;
	y += dy;	

	if (y < -300 || y>250)
		isFinish = true;
}

void HidenWall::Render()
{
	if (isFinish == false)
	{
		animation_set->at(0)->Render(x, y);
	}
}

void HidenWall::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	/*if (isFinish == false)
	{
		left = x;
		top = y;
		right = x + BRICK_BBOX_WIDTH;
		bottom = y + BRICK_BBOX_HEIGHT;
	}*/
}
