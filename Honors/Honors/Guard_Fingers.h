#pragma once
#pragma once
#include "TerrainMesh.h"

class Guard_Fingers : public TerrainMesh
{
public:
	Guard_Fingers(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int resolution = 128);
	~Guard_Fingers();

	void guardMeshCurve();

	const inline bool Get_x_dimension_curve() { return x_dimension_curve; }
	void Set_x_dimension_curve(bool x_curve) { x_dimension_curve = x_curve; }

	const inline bool Get_y_dimension_curve() { return y_dimension_curve; }
	void Set_y_dimension_curve(bool y_curve) { y_dimension_curve = y_curve; }

private:
	bool x_dimension_curve, y_dimension_curve;
};