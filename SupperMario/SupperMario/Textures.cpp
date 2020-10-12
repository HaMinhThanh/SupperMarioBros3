#include <Windows.h>

#include <d3d9.h>
#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "Textures.h"

Textures* Textures::_instance = NULL;

Textures::Textures()
{

}

Textures* Textures::GetInstance()
{
	if (_instance == NULL) _instance = new Textures();
	return _instance;
}

void Textures::AddTexture(int id, LPCWSTR filePath, D3DCOLOR transparentColor)
{
	textures[id] = Game::GetInstance()->LoadTexture(filePath);
}

LPDIRECT3DTEXTURE9 Textures::Get(unsigned int i)
{
	return textures[i];
}
