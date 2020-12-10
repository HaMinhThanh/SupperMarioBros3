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

	Mario* mario = Mario::GetInstance(0, 0);

	if (mario->numItem == 1) {
		it1 = mario->GetItem();
	}
	else if (mario->numItem == 2) {
		it2 = mario->GetItem();
	}
	else if (mario->numItem == 3) {
		it3 = mario->GetItem();
	}
	else if (mario->numItem > 3) {
		it1 = it2 = it3 = -1;
		mario->numItem -= 3;
	}
}

void HUD::Render()
{
	Game* game = Game::GetInstance();
	Mario* mario = Mario::GetInstance(0,0);

	float cx, cy;
	game->GetCamPos(cx, cy);

	Sprites* sprite = Sprites::GetInstance();
	sprite->Get(50000)->Draw(cx, cy+ game->GetScreenHeight()-HUD_BBOX_HEIGHT);
	sprite->Get(SPRITE_M_CHARACTER)->Draw(cx + 10, cy + game->GetScreenHeight() - 20);

	string str = to_string(mario->GetDola());

	event->DrawNumber(3, cx + 132 , cy + game->GetScreenHeight() - 19, time);	
	event->DrawNumber(7, cx + 58, cy + game->GetScreenHeight() - 19, mario->GetScore());
	event->DrawNumber(1, cx + 40, cy + game->GetScreenHeight() - 19, mario->GetLive());
	event->DrawNumber(1, cx + 42, cy + game->GetScreenHeight() - 27, game->GetCurrentSceneId());
	event->DrawNumber(str.size(), cx + 140, cy + game->GetScreenHeight() - 27, mario->GetDola());

	event->DrawPower(cx + 58, cy + game->GetScreenHeight() - 27, mario->metter);

	if (it1 != -1) {
		event->DrawItem(cx + 171, cy + game->GetScreenHeight() - 28, it1);
	}
	if (it2 != -1) {
		event->DrawItem(cx + 195, cy + game->GetScreenHeight() - 28, it2);
	}
	if (it3 != -1) {
		event->DrawItem(cx + 219, cy + game->GetScreenHeight() - 28, it3);
	}
}