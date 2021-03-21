#pragma once
#include "TerrainMesh.h"
class Handle : public TerrainMesh
{
public:
	Handle(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int resolution = 128);
	~Handle();

	void handleMeshCurve();

	const inline bool Get_x_dimension_curve() { return x_dimension_curve; }
	void Set_x_dimension_curve(bool x_curve) { x_dimension_curve = x_curve; }

private:
	bool x_dimension_curve;
};

