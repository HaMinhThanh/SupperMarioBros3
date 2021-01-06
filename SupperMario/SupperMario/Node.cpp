#include "Node.h"

Node::Node(float x, float y, int l, int t, int r, int b, int tp)
{
	isFinish = false;

	SetPosition(x, y);

	type = tp;

	if (l == 0)
		this->left = false;
	else
		this->left = true;

	if (t == 0)
		this->top = false;
	else
		this->top = true;

	if (r == 0)
		this->right = false;
	else
		this->right = true;

	if (b == 0)
		this->bottom = false;
	else
		this->bottom = true;
}

Node::~Node()
{

}

void Node::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

}

void Node::Render()
{
	//animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void Node::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + 10;
	bottom = y + 10;
}

void Node::SetDirectNode(float x, float y, int l, int t, int r, int b)
{
	SetPosition(x, y);

	if (l == 0)
		this->left = false;
	else
		this->left = true;

	if (t == 0)
		this->top = false;
	else
		this->top = true;

	if (r == 0)
		this->right = false;
	else
		this->right = true;

	if (b == 0)
		this->bottom = false;
	else
		this->bottom = true;
}

