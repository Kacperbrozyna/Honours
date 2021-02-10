#include "Blade.h"

#define PI 3.14159265

Blade::Blade(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int resolution) : 
	TerrainMesh(device, deviceContext, resolution)
{
	blade = true;
}

Blade::~Blade()
{
}

void Blade::edge_sharpness()
{
	const float scale = terrainSize / (float)resolution;
	float edge_point_offset;

	edge_point_offset = (GetThickness() * scale) / edge_offset;

	//edge slope for the sides
	for (int j = 0; j < (resolution); j++) 
	{
		for (int i = 0; i < (resolution); i++) 
		{
			if (j <= edge_offset)
			{
				offsetMap[(j * resolution) + i] = (edge_point_offset * j);
			}
			else if (edge_offset >= resolution - j)
			{
				offsetMap[(j * resolution) + i] = (edge_point_offset * (resolution - j));
			}
		}
	}

	//slope for the tip
	for (int j = 0; j < (resolution); j++)
	{
		for (int i = resolution-edge_offset; i < (resolution); i++)
		{
			if (edge_offset >= resolution - i)
			{
				if (j > resolution - i && j < i)
				{
					offsetMap[(j * resolution) + i] = (edge_point_offset * (resolution - i));
				}
			}
		}
	}
} 

void Blade::fuller()
{
	const float scale = terrainSize / (float)resolution;

	if (fuller_width != 0)
	{
		float width_diff = float(fuller_width) /   GetWidth(); //float(resolution);
		float fuller_pos_x = resolution * width_diff;
		float sin_increment = 180 / fuller_pos_x;
		float temp_total_increment;

		for (int i = fuller_base; i < (fuller_height); i++)
		{
			temp_total_increment = 0;

			for (int j = (resolution / 2) - int(fuller_pos_x / 2); j < (resolution/2) + int(fuller_pos_x/2); j++)
			{
				float test = (sin((temp_total_increment * PI / 180))) * fuller_depth;
				offsetMap[(j * resolution) + i] = GetThickness() * scale- test;
				temp_total_increment += sin_increment;
			}
		}
	}
}

void Blade::smooth()
{
}