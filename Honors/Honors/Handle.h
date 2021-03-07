#pragma once
#include "TerrainMesh.h"
class Handle : public TerrainMesh
{
public:
	Handle(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int resolution = 128);
	~Handle();

	void handleMeshCurve();

};

