#pragma once
#include "TerrainMesh.h"
class Pommel : public TerrainMesh
{
public:
	Pommel(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int resolution = 128);
	~Pommel();

};

