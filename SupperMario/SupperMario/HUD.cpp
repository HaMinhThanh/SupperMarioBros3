#include "HUD.h"
#include "Game.h"
#include <string>

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
	sprite->Get(50000)->Draw(cx, cy+ game->GetScreenHeight()-HUD_BBOX_HEIGHT);

	string str = to_string(mario->GetDola());

	event->DrawNumber(3, cx + 132 , cy + game->GetScreenHeight() - 20, time);
	//event->DrawNumber(str.size(), cx + 140, cy + game->GetScreenHeight() - 30, mario->GetDola());
	//event->DrawNumber(7, cx+55, cy + game->GetScreenHeight() - 20, mario->GetX());

	event->DrawNumber(2, cx + 140, cy + game->GetScreenHeight() - 30, 15);
}