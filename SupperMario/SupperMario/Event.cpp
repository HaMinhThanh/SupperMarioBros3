#include "Event.h"
#include "Sprites.h"
#include "Mario.h"
#include <string>

void Event::DrawNumber(int max, float x, float y, int _string)
{
	string str = to_string(_string);

	for (int i = 0; i < (max - str.size()); i++)
	{
		Sprites::GetInstance()->Get(SPRITE_ID_ZERO)->Draw(x, y);
		x += 10;
	}
	for (int i = 0; i < str.size(); i++)
	{
		Sprites::GetInstance()->Get(SPRITE_ID_ZERO + ConverToInt(str[i]))->Draw(x, y);
		x += 10;
	}
}

void Event::DrawItem(float x, float y, int type, int string)
{

}

int Event::ConverToInt(char C)
{
	return C - '0';
}
