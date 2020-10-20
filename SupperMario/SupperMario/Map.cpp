#include "Map.h"

Map::Map()
{

}

Map::~Map()
{

}

void Map::LoadMap(eType etype)
{

}

void Map::DrawMap(Camera* camera)
{
	int row = (int)(camera->GetYCam()) / texture->GetFrameHeight();
	int column = (int)(camera->GetXCam() / texture->GetFrameWidth());

	float x = -(float)((int)(camera->GetXCam()) % texture->GetFrameHeight());
	float y = -(float)((int)(camera->GetYCam()) % texture->GetFrameHeight());

	for (int i = 0; i < SCREEN_HEIGHT / texture->GetFrameHeight() + 1; i++)
		for (int j = 0; j < SCREEN_WIDTH / texture->GetFrameWidth() + 1; j++)
			if (!(row + i < 0 || row + i >= RowMap || j + column<0 || j + column>ColumnMap))
				sprite->DrawFrame(TileMap[row + i][column + j], x + texture->GetFrameWidth() * j, y + texture->GetFrameHeight() * i + HeightBoard);


}

int Map::GetMapWidth()
{
	return ColumnMap * texture->GetFrameWidth();
}

int Map::GetMapHeight()
{
	return RowMap * texture->GetFrameHeight();
}
