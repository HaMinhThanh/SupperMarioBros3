#pragma once
#include "Sprites.h"
#include "Textures.h"
#include "Sprites.h"
#include"Utils.h"
#include "Define.h"

#define MAP_CELL_SIZE	16

using namespace std;

class Map
{

private:
	int _column_max;
	int _row_max;
	int index;
	int align;
	int TileMap[51][51];

	static Map* _instance;
public:
	static Map* GetInstance();

	Map();
	void SetValueInMap(int row, int column, int index, int align);
	int getTile(int x, int y);
	void LoadMap(wstring filePathTxt);
	void DrawMap(int cam_x, int cam_y);
	int GetColumn() { return _column_max; }
};

