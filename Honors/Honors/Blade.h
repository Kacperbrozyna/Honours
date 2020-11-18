#pragma once
#include "TerrainMesh.h"

class Blade : public TerrainMesh
{
public:
	Blade(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int resolution = 128);
	~Blade();




};

