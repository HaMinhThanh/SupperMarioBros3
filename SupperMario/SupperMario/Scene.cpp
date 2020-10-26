#include "Scene.h"

Scene::Scene(int id, LPCWSTR filePath)
{
	this->id = id;
	this->sceneFilePath = filePath;
}

D3DCOLOR Scene::GetBackColor()
{
	return backColor;
}

