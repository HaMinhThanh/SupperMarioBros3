#include "Map.h"
#include "fstream"
#include "Game.h"
#include "Utils.h"

#define SCREEN_HEIGHT	272
#define SCREEN_WIDTH	336

Map* Map::_instance = NULL;

Map* Map::GetInstance()
{
	if (_instance == NULL) _instance = new Map();
	return _instance;
}

Map::Map()
{

}

void Map::SetValueInMap(int row, int column, int index, int align)
{
	this->_column_max = column;
	this->_row_max = row;
	this->index = index;
	this->align = align;
}

int Map::getTile(int i, int j)
{
	return TileMap[i][j];
}

void Map::LoadMap(wstring filePathTxt)
{
	ifstream inFile;
	inFile.open(filePathTxt);

	if (inFile.fail())
	{
		DebugOut(L"[ERROR] Map::Load Map failed\n");
		inFile.close();
		return;
	}

	for (int i = 0; i < _row_max; i++)
	{
		for (int j = 0; j < _column_max; j++)
		{
			inFile >> TileMap[i][j];
		}
	}
	inFile.clear();
}

void Map::DrawMap(int cam_x, int cam_y)
{
	int top = (int)(cam_y) / MAP_CELL_SIZE;

	int left = (int)(cam_x) / MAP_CELL_SIZE;
	int right = (int)(cam_x + SCREEN_HEIGHT) / MAP_CELL_SIZE;

	Sprites* sprites = Sprites::GetInstance();

	for (int row = top; row < _row_max; row++)
	{
		for (int column = left; column < right + 2; column++)	// hệ số +2 sử dụng để fix lỗi logic k vẽ đủ map vì nhất định cell size map phải bằng 32
		{
			if (row <= _row_max && row >= 0 && column < _column_max && column >= 0)
			{
				sprites->Get(getTile(row, column) + index)->Draw(16 * column, 16 * row + align, 255);
			}
		}
	}
	sprites = NULL;
}
