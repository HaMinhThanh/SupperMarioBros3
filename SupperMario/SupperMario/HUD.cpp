#include "HUD.h"
#include "Game.h"

HUD* HUD::instance = NULL;

HUD* HUD::GetInstance()
{
	if (instance == NULL)
		instance = new HUD();

	return instance;
}

void HUD::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isStop)
	{
		return;
	}

	if (GetTickCount() - count >= 1000)
	{
		if (time > 0)
			time--;
		count = GetTickCount();
	}

	if (timeItem > 0)
	{
		if (GetTickCount() - timeItem >= 10000)
		{
			number = 0;
			timeItem = 0;
		}
	}
}

void HUD::Render()
{
	Game* game = Game::GetInstance();
	Mario* mario = Mario::GetInstance();

	float cx, cy;
	game->GetCamPos(cx, cy);
	Sprites* sprite = Sprites::GetInstance();
	sprite->Get(50000)->Draw(cx, cy+ game->GetScreenHeight());
}