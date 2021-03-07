#pragma once
#include "TerrainMesh.h"


class Blade : public TerrainMesh
{
public:
	Blade(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int resolution = 128);
	~Blade();

	void edge_sharpness();
	void fuller();

	int fuller_base, fuller_height = GetHeight() / 2;
	int fuller_width, num_fullers;
	float fuller_depth;
};

