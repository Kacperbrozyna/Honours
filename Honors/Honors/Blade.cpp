#include "Blade.h"


Blade::Blade(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int resolution) : 
	TerrainMesh(device, deviceContext, resolution)
{
	meshType = BLADE;
}

Blade::~Blade()
{
}

void Blade::edge_sharpness()
{
	const float scale = terrainSize / (float)resolution;
	float edge_point_offset;

	edge_point_offset = (GetThickness() * scale) / Get_edge_offset();

	//create sharpness for the sides of the blade
	//looping through the all the points in the mesh
	for (int j = 0; j < (resolution); j++) 
	{
		for (int i = 0; i < (resolution); i++) 
		{
			//checking if under or over value
			if (j <= Get_edge_offset())
			{
				//applying transform
				offsetMap[(j * resolution) + i] = (edge_point_offset * j);
			}
			else if (Get_edge_offset() >= resolution - j)
			{
				//applying transform
				offsetMap[(j * resolution) + i] = (edge_point_offset * (resolution - j));
			}
			else
			{
				offsetMap[(j * resolution) + i] = GetThickness() * scale;
			}
		}
	}


	if (edge_tip)
	{
		//create sharpness for the tip
		//looping through all the points in the mesh
		for (int j = 0; j < (resolution); j++)
		{
			for (int i = resolution - Get_edge_offset(); i < (resolution); i++)
			{
				//checking if it is in range of values
				if (Get_edge_offset() >= resolution - i)
				{
					if (j > resolution - i && j < i)
					{
						//applying transform
						offsetMap[(j * resolution) + i] = (edge_point_offset * (resolution - i));
					}
				}
			}
		}
	}
} 

void Blade::build_fuller()
{
	const float scale = terrainSize / (float)resolution;
	
	//checking if the width of the fuller is above 0
	if (fuller_width > 0)
	{
	
			//creating and setting temporary variables
			float width_diff = float(fuller_width) / GetWidth();
			float fuller_pos_x = resolution * width_diff;
			float sin_increment = 180 / fuller_pos_x;
			float temp_total_increment;

			//looping through the points in the mesh
			for (int i = fuller_base; i < (fuller_height); i++)
			{
				temp_total_increment = 0;

				//looping through the minimum x value to the max x value of the fuller width, being centered
				for (int j = 0; j < (resolution); j++)
				{
					if (j > (resolution / 2) - int(fuller_pos_x / 2) && j < (resolution / 2) + int(fuller_pos_x / 2))
					{
						//applying offset and incrementing value
						offsetMap[(j * resolution) + i] = GetThickness() * scale - ((sin((temp_total_increment * PI / 180))) * fuller_depth);
						temp_total_increment += sin_increment;
					}
				}
			}

			
		
	}
	
	//if fuller is not enabled, set offset
	if(fuller == false)
	{
		for (int i = 0; i < (resolution); i++)
		{
			for (int j = 0; j < (resolution); j++)
			{
				offsetMap[(j * resolution) + i] = GetThickness() * scale;
			}
		}
	}
}