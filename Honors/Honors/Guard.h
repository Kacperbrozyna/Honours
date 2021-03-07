#pragma once
#include "TerrainMesh.h"

class Guard : public TerrainMesh
{
public:
	Guard(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int resolution = 128);
	~Guard();

	void guardMeshCurve();
};

