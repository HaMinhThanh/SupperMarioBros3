#include "Node.h"

Node::Node(float x, float y, int l, int t, int r, int b)
{
	this->x = x;
	this->y = y;

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

void Node::Render()
{

}

void Node::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

}


