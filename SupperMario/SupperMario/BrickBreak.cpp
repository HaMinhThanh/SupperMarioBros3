#include "BrickBreak.h"

BrickBreak::BrickBreak(float x, float y)
{
	isRender = false;
	isFinish = false;

	render = 0;
	time_render = 0;

	SetPosition(x, y);

	x1 = x2 = x3 = x4 = x ;
	y1 = y2 = y3 = y4 = y;
}

BrickBreak::~BrickBreak()
{

}

void BrickBreak::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isRender)
	{
		StartRender();
		isRender = false;
	}

	if (render == 1)
	{
		if (GetTickCount() - time_render > 0 && GetTickCount() - time_render <= 50)
		{
			x1 = x - 3;
			x2 = x + 3;
			x3 = x - 3;
			x4 = x + 3;

			y1 = y - 5;
			y2 = y - 5;
			y3 = y - 1;
			y4 = y - 1;
		}
		if (GetTickCount() - time_render > 50 && GetTickCount() - time_render <= 100)
		{
			x1 = x - 7;
			x2 = x + 7;
			x3 = x - 7;
			x4 = x + 7;

			y1 = y - 10;
			y2 = y - 10;
			y3 = y - 3;
			y4 = y - 3;
		}
		if (GetTickCount() - time_render > 100 && GetTickCount() - time_render <= 150)
		{
			x1 = x - 10;
			x2 = x + 10;
			x3 = x - 10;
			x4 = x + 10;

			y1 = y - 15;
			y2 = y - 15;
			y3 = y - 5;
			y4 = y - 5;
		}
		if (GetTickCount() - time_render > 150 && GetTickCount() - time_render <= 200)
		{
			x1 = x - 15;
			x2 = x + 15;
			x3 = x - 15;
			x4 = x + 15;

			y1 = y - 22;
			y2 = y - 22;
			y3 = y - 7;
			y4 = y - 7;
		}
		else if (GetTickCount() - time_render > 250 && GetTickCount() - time_render <= 300)
		{
			x1 = x - 20;
			x2 = x + 20;
			x3 = x - 20;
			x4 = x + 20;

			y1 = y - 25;
			y2 = y - 25;
			y3 = y - 9;
			y4 = y - 9;
		}
		else if (GetTickCount() - time_render > 350 && GetTickCount() - time_render <= 400)
		{
			x1 = x - 25;
			x2 = x + 25;
			x3 = x - 25;
			x4 = x + 25;

			y1 = y - 30;
			y2 = y - 30;
			y3 = y - 11;
			y4 = y - 11;
		}
		else if (GetTickCount() - time_render > 400)
		{
			x1 -= 2;
			x2 += 2;
			x3 -= 2;
			x4 += 2;

			y1 += 2;
			y2 += 2;
			y3 += 2;
			y4 += 2;
		}

		if (GetTickCount() - time_render > 1000)
		{
			time_render = 0;
			render = 0;

			isFinish = true;
		}
	}
}

void BrickBreak::Render()
{
	if (render == 1)
	{
		Sprites* sprite = Sprites::GetInstance();

		sprite->Get(111)->Draw(x1, y1);
		sprite->Get(111)->Draw(x2, y2);
		sprite->Get(111)->Draw(x3, y3);
		sprite->Get(111)->Draw(x4, y4);
	}
}

void BrickBreak::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

}
