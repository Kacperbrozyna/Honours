#include "Guard.h"

Guard::Guard(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int resolution) :
	TerrainMesh(device, deviceContext, resolution)
{
	guard = true;
}

Guard::~Guard()
{
}

void Guard::guardMeshCurve()
{	
	float increment =  180 / (float)resolution;
	float temp_increment;
	const float scale = terrainSize / (float)resolution;

	for (int i = 0; i < resolution; i++)
	{
		temp_increment = 0;

		for (int j = 0; j < resolution; j++)
		{	
			if (inverseCurve)
			{
				offsetMap[(j * resolution) + i] = (GetThickness() * scale) - ((sin((temp_increment * PI / 180))) * curvature_value);
			}
			else
			{
				offsetMap[(j * resolution) + i] = (GetThickness() * scale) + ((sin((temp_increment * PI / 180))) * curvature_value);
			}
			
			temp_increment += increment;
		}
	}
}