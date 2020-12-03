#pragma once

#include "Game.h"
#include "GameObject.h"
#include "mario.h"
#include "Event.h"
#include "Star.h"

#define SPRITE_ID_ZERO	90000

#define SPRITE_ID_MUSHROOM	91001
#define	SPRITE_ID_FLOWER	91002
#define SPRITE_ID_STAR		91003

class Event
{
public:
	void DrawNumber(int max, float x, float y, int string = 0);
	void DrawItem(float x, float y, int type = 0, int string = 0);
	int ConverToInt(char C);
};

