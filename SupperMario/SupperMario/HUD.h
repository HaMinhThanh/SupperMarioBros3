#pragma once

#include "Game.h"
#include "GameObject.h"
#include "mario.h"
#include "Event.h"
#include "Star.h"

class HUD
{
	int time;
	DWORD count;
	int number;
	int dola;
	int score;

	bool isStop;	
	DWORD timeItem;

	Event* event;

	static HUD* instance;
public:
	HUD()
	{
		time = 300;
		count = GetTickCount();
		isStop = false;
		number = 0;
		dola = 0;
		score = 0;
		timeItem = 0;
		//Event = new Event();
	}
	~HUD()
	{
		if (event == NULL)
			delete event;

		event = NULL;
	}

	static HUD* GetInstance();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	void Render();
	
	void TimeCountDown()
	{
		if (GetTickCount() - count >= 1 && time > 0)
		{
			time--;
			count = GetTickCount();
		}
	}

	void SetNumber(int n)
	{
		this->number = n;
		timeItem = GetTickCount();
	}

	int GetTime() { return time; }

	void SetTime(int t) { time = t; }
};

