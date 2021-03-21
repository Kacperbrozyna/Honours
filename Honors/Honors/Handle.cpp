#include "Handle.h"

Handle::Handle(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int resolution) :
	TerrainMesh(device, deviceContext, resolution)
{
	meshType = GRIP;
}

Handle::~Handle()
{}

void Handle::handleMeshCurve()
{
	//setting temporary variables
	float increment = 180 / (float)resolution;
	float temp_increment;
	const float scale = terrainSize / (float)resolution;

	//looping through out map 
	for (int i = 0; i < resolution; i++)
	{
		temp_increment = 0;

		for (int j = 0; j < resolution; j++)
		{
			//adjusting variable at specific point
			if (x_dimension_curve)
			{
				offsetMap[(j * resolution) + i] = (GetThickness() * scale) + ((sin((temp_increment * PI / 180))) * Get_curvature_value());
			}

			//incrementing variable
			temp_increment += increment;
		}
	}
}