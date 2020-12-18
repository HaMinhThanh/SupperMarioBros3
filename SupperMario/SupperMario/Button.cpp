#include "Button.h"

Button::Button()
{
	isFinish = false;
}

Button::~Button()
{

}

void Button::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

}

void Button::Render()
{
	if(isFinish)
		animation_set->at(1)->Render(x, y);
	else
		animation_set->at(0)->Render(x, y);
}

void Button::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isFinish = false)
	{
		left = x;
		top = y;
		right = left + BUTTON_BBOX_WIDTH;
		bottom = top + BUTTON_BBOX_HEIGHT;
	}
	else
	{
		left = x;
		top = y + BUTTON_BBOX_WIDTH / 4;
		right = left + BUTTON_BBOX_WIDTH;
		bottom = top + BUTTON_BBOX_HEIGHT;
	}
}