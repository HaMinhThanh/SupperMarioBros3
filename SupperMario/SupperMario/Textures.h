#pragma once

#include <unordered_map>
#include <d3dx9.h>

using namespace std;

class Textures
{
	static Textures* _instance;

	unordered_map<int, LPDIRECT3DTEXTURE9> textures;

public:
	Textures();
	void AddTexture(int id, LPCWSTR filePath, D3DCOLOR transparentColor);
	LPDIRECT3DTEXTURE9 Get(unsigned int i);

	void Clear();

	static Textures* GetInstance();
};

