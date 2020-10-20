#pragma once

#include "Textures.h"
#include "Sprites.h"
#include "Camera.h"

class Map
{
private:
	Textures* texture;
	Animation* animation;

	int ColumnTile, RowTile, TotalTiles;

	int ColumnMap, RowMap;

	int HeightBoard;

	int TileMap[500][500];

	char* fileName;

public:
	Map();
	~Map();

	void LoadMap(eType type);
	void ReadMap(char* filename);
	void DrawMap(Camera* camera);

	int GetMapWidth();
	int GetMapHeight();

};

