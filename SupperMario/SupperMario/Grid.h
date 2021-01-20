#pragma once
#include <vector>
#include "Game.h"
#include "GameObject.h"
#include "Utils.h"
#include "Define.h"

#define GRID_ROW_MAX		2
#define GRID_COLUMN_MAX		9

#define GRID_CELL_WIDTH		336//SCREEN_WIDTH
#define GRID_CELL_HEIGHT	272//SCREEN_HEIGHT

using namespace std;

class Grid
{
	vector<LPGAMEOBJECT> cells[GRID_ROW_MAX][GRID_COLUMN_MAX];
public:
	Grid();
	void LoadGrid(vector<LPGAMEOBJECT> ListObj);
	void GetListObject(vector<LPGAMEOBJECT>& ListObj, float cam_x, float cam_y);
	void Insert(LPGAMEOBJECT obj, float x, float y, float w, float h);
	void PushObjectToCell(LPGAMEOBJECT obj, int grid_row, int grid_column);
	void ResetOnCamera(vector<LPGAMEOBJECT>& ListObj);
};

