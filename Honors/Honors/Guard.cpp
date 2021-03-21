#include "Guard.h"

Guard::Guard(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int resolution) :
	TerrainMesh(device, deviceContext, resolution)
{
	meshType = GUARD;
}

Guard::~Guard()
{
}

void Guard::guardMeshCurve()
{	
	//setting temporary variables
	float increment =  180 / (float)resolution;
	float temp_increment;
	const float scale = terrainSize / (float)resolution;

	//looping through all the points in the mesh
	for (int i = 0; i < resolution; i++)
	{
		temp_increment = 0;

		for (int j = 0; j < resolution; j++)
		{	
			//depending on inverse or not adjusting value
			if (Get_inverse_curve())
			{
				if (x_dimension_curve && y_dimension_curve)
				{
					offsetMap[(j * resolution) + i] = (GetThickness() * scale) - ((((sin((temp_increment * PI / 180))) + sin(((i * increment) * PI / 180))) / 2) * Get_curvature_value());
				}
				else if (x_dimension_curve)
				{
					offsetMap[(j * resolution) + i] = (GetThickness() * scale) - (sin((temp_increment * PI / 180))) * Get_curvature_value();
				}
				else if (y_dimension_curve)
				{
					offsetMap[(j * resolution) + i] = (GetThickness() * scale) - sin(((i * increment) * PI / 180)) * Get_curvature_value();
				}
			}
			else
			{
				if (x_dimension_curve && y_dimension_curve)
				{
					offsetMap[(j * resolution) + i] = (GetThickness() * scale) + ((((sin((temp_increment * PI / 180))) + sin(((i * increment) * PI / 180))) / 2) * Get_curvature_value());
				}
				else if (x_dimension_curve)
				{
					offsetMap[(j * resolution) + i] = (GetThickness() * scale) + (sin((temp_increment * PI / 180))) * Get_curvature_value();
				}
				else if (y_dimension_curve)
				{
					offsetMap[(j * resolution) + i] = (GetThickness() * scale) + sin(((i * increment) * PI / 180)) * Get_curvature_value();
				}
			}

			//incrementing variable
			temp_increment += increment;
		}
	}
}