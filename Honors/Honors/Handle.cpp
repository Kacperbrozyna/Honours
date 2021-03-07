#include "Handle.h"

Handle::Handle(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int resolution) :
	TerrainMesh(device, deviceContext, resolution)
{
	handle = true;
}

Handle::~Handle()
{}

void Handle::handleMeshCurve()
{
	float increment = 180 / (float)resolution;
	float temp_increment;
	const float scale = terrainSize / (float)resolution;

	for (int i = 0; i < resolution; i++)
	{
		temp_increment = 0;

		for (int j = 0; j < resolution; j++)
		{
			offsetMap[(j * resolution) + i] = (GetThickness() * scale) + ((sin((temp_increment * PI / 180))) * curvature_value);
	
			temp_increment += increment;
		}
	}
}