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

#define SPRITE_M_CHARACTER	91004
#define SPRITE_L_CHARACTER	91005

#define SPRITE_POWER_BLACK	91012
#define SPRITE_POWER_WHITE	91011
#define SPRITE_P_CHARACTER_BLACK	91014
#define SPRITE_P_CHARACTER_WHITE	91013

class Event
{
public:
	void DrawNumber(int max, float x, float y, int string = 0);
	void DrawItem(float x, float y, int type );
	void DrawPower(float x, float y, int m);
	int ConverToInt(char C);
};

