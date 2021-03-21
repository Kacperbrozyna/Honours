#include "TerrainMesh.h"

TerrainMesh::TerrainMesh( ID3D11Device* device, ID3D11DeviceContext* deviceContext, int lresolution ) :
	PlaneMesh( device, deviceContext, lresolution ) 
{
	Resize( resolution );
	Regenerate( device, deviceContext );
	
	perlin_noise = new Perlin;
}

//Cleanup the heightMap
TerrainMesh::~TerrainMesh() {
	delete[] heightMap;
	heightMap = 0;
}

void TerrainMesh::BuildEdges() 
{
	//Scale everything so that the look is consistent across terrain resolutions
	const float scale =  terrainSize / (float)resolution;

	//looping through out the height map
	for (int j = 0; j < (resolution); j++) 
	{
		for (int i = 0; i < (resolution); i++) 
		{
			//if the point is either edge set the value
			if (j == 0 || i == 0 || i == (resolution - 1) || j == (resolution - 1))
			{
				//setting value to 0
				heightMap[(j * resolution) + i] = 0;
			}
			else
			{ 
				//if offset map has valid values
				if (offsetMap[(j * resolution) + i] >= -1)
				{
					//setting heightmap to offset map
					heightMap[(j * resolution) + i] = offsetMap[(j * resolution) + i];
				}
				else
				{
					//if the values are not valid 
					heightMap[(j * resolution) + i] = thickness * scale;
				}
			}
		}
	}
}

void TerrainMesh::Resize( int newResolution ) {
	resolution = newResolution;
	heightMap = new float[resolution * resolution];
	offsetMap = new float[resolution * resolution];

	if( vertexBuffer != NULL ) {
		vertexBuffer->Release();
	}
	vertexBuffer = NULL;
}

// Set up the heightmap and create or update the appropriate buffers
void TerrainMesh::Regenerate( ID3D11Device * device, ID3D11DeviceContext * deviceContext ) {

	VertexType* vertices;
	unsigned long* indices;
	int index, i, j;
	float positionX, positionY, positionZ, u, v, increment;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	
	//Calculate and store the height values
	BuildEdges();

	// Calculate the number of vertices in the terrain mesh.
	// We share vertices in this mesh, so the vertex count is simply the terrain 'resolution'
	// and the index count is the number of resulting triangles * 3 OR the number of quads * 6
	vertexCount = resolution * resolution;

	indexCount = ( ( resolution - 1 ) * ( resolution - 1 ) ) * 6;
	vertices = new VertexType[vertexCount];
	indices = new unsigned long[indexCount];

	index = 0;

	// UV coords.
	u = 0;
	v = 0;
	increment = m_UVscale / resolution;

	//Scale everything so that the look is consistent across terrain resolutions
	const float scale = terrainSize / (float)resolution;
	
	//checking if the mesh is the Handle and the size of the vector is above 0
	if (meshType == GRIP && meshLayers.size() > 0)
	{
		//setting the variable
		layer_div = resolution / (meshLayers.size() + 1);
	}

	//Set up vertices
	for( j = 0; j < ( resolution ); j++ ) {

		//set the z position
		positionZ = ((float)(j * (width / 100) * scale));

		incremtn_test = 0;

		for( i = 0; i < ( resolution ); i++ ) {

			//set the y position
			positionY = ((float)(i * (height * 0.01) * scale));
			
			//depending if the mesh is mirrored inverse the values
			if (mirrored == true)
			{
				positionX = heightMap[index];
			}
			else
			{
				positionX = -heightMap[index];
			}
			
			//getting offset values depending on the mesh, values and points
			XMFLOAT3 temp_mesh_offset = specific_mesh_Offset(j, i);

			vertices[index].position = XMFLOAT3(positionX + temp_mesh_offset.x, (positionY + position_offsetY) + temp_mesh_offset.y, (positionZ + position_offsetZ) + temp_mesh_offset.z);
			
			//setting textures up with u,v coordinates
			vertices[index].texture = XMFLOAT2( u, v );

			//increment values
			u += increment;
			index++;
		}

		//increment values
		u = 0;
		v += increment;
	}

	//get the distance between the two furthest point to constrain the meshes to each other 
	dynamic_height = (((float)((resolution - 1) * height / 100) * scale)) - (((float)(0 * height / 100) * scale));
	dynamic_width = (((float)((resolution - 1) * width / 100) * scale)) - ((float)(0 * (width / 100) * scale));

	//Set up index list
	index = 0;
	for( j = 0; j < ( resolution - 1 ); j++ ) {
		for( i = 0; i < ( resolution - 1 ); i++ ) {

			if( mirrored == true)
			{
		
				indices[index] = (j * resolution) + i;
				indices[index + 1] = ((j + 1) * resolution) + i;
				indices[index + 2] = ((j + 1) * resolution) + (i + 1);


				indices[index + 3] = (j * resolution) + i;
				indices[index + 4] = ((j + 1) * resolution) + (i + 1);
				indices[index + 5] = (j * resolution) + (i + 1);

				index += 6;
			}

			else
			{
				indices[index] = (j * resolution) + i;
				indices[index + 1] = ((j + 1) * resolution) + (i + 1);
				indices[index + 2] = ((j + 1) * resolution) + i;

				indices[index + 3] = (j * resolution) + i;
				indices[index + 4] = (j * resolution) + (i + 1);
				indices[index + 5] = ((j + 1) * resolution) + (i + 1);
				index += 6;
				
			}
			
		}
	}

	//Set up normals
	for( j = 0; j < ( resolution - 1 ); j++ ) {
		for( i = 0; i < ( resolution - 1 ); i++ ) {
			//Calculate the plane normals
			XMFLOAT3 a, b, c;	//Three corner vertices
			a = vertices[j * resolution + i].position;
			b = vertices[j * resolution + i + 1].position;
			c = vertices[( j + 1 ) * resolution + i].position;

			//Two edges
			XMFLOAT3 ab( c.x - a.x, c.y - a.y, c.z - a.z );
			XMFLOAT3 ac( b.x - a.x, b.y - a.y, b.z - a.z );
			
			//Calculate the cross product
			XMFLOAT3 cross;
			cross.x = ab.y * ac.z - ab.z * ac.y;
			cross.y = ab.z * ac.x - ab.x * ac.z;
			cross.z = ab.x * ac.y - ab.y * ac.x;
			float mag = ( cross.x * cross.x ) + ( cross.y * cross.y ) + ( cross.z * cross.z );
			mag = sqrtf( mag );
			cross.x/= mag;
			cross.y /= mag;
			cross.z /= mag;
			vertices[j * resolution + i].normal = cross;
		}
	}

	//Smooth the normals by averaging the normals from the surrounding planes
	XMFLOAT3 smoothedNormal( 0, 1, 0 );
	for( j = 0; j < resolution; j++ ) {
		for( i = 0; i < resolution; i++ ) {
			smoothedNormal.x = 0;
			smoothedNormal.y = 0;
			smoothedNormal.z = 0;
			float count = 0;
			//Left planes
			if( ( i - 1 ) >= 0 ) {
				//Top planes
				if( ( j ) < ( resolution - 1 ) ) {
					smoothedNormal.x += vertices[j * resolution + ( i - 1 )].normal.x;
					smoothedNormal.y += vertices[j * resolution + ( i - 1 )].normal.y;
					smoothedNormal.z += vertices[j * resolution + ( i - 1 )].normal.z;
					count++;
				}
				//Bottom planes
				if( ( j - 1 ) >= 0 ) {
					smoothedNormal.x += vertices[( j - 1 ) * resolution + ( i - 1 )].normal.x;
					smoothedNormal.y += vertices[( j - 1 ) * resolution + ( i - 1 )].normal.y;
					smoothedNormal.z += vertices[( j - 1 ) * resolution + ( i - 1 )].normal.z;
					count++;
				}
			}
			//right planes
			if( ( i ) <( resolution - 1 ) ) {

				//Top planes
				if( ( j ) < ( resolution - 1 ) ) {
					smoothedNormal.x += vertices[j * resolution + i].normal.x;
					smoothedNormal.y += vertices[j * resolution + i].normal.y;
					smoothedNormal.z += vertices[j * resolution + i].normal.z;
					count++;
				}
				//Bottom planes
				if( ( j - 1 ) >= 0 ) {
					smoothedNormal.x += vertices[( j - 1 ) * resolution + i].normal.x;
					smoothedNormal.y += vertices[( j - 1 ) * resolution + i].normal.y;
					smoothedNormal.z += vertices[( j - 1 ) * resolution + i].normal.z;
					count++;
				}
			}
			smoothedNormal.x /= count;
			smoothedNormal.y /= count;
			smoothedNormal.z /= count;

			float mag = sqrt( ( smoothedNormal.x * smoothedNormal.x ) + ( smoothedNormal.y * smoothedNormal.y ) + ( smoothedNormal.z * smoothedNormal.z ) );
			smoothedNormal.x /= mag;
			smoothedNormal.y /= mag;
			smoothedNormal.z /= mag;

			vertices[j * resolution + i].normal = smoothedNormal;
		}
	}
	//If we've not yet created our dyanmic Vertex and Index buffers, do that now
	if( vertexBuffer == NULL ) {
		CreateBuffers( device, vertices, indices );
	}
	else {
		//If we've already made our buffers, update the information
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		ZeroMemory( &mappedResource, sizeof( D3D11_MAPPED_SUBRESOURCE ) );

		//  Disable GPU access to the vertex buffer data.
		deviceContext->Map( vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
		//  Update the vertex buffer here.
		memcpy( mappedResource.pData, vertices, sizeof( VertexType ) * vertexCount );
		//  Reenable GPU access to the vertex buffer data.
		deviceContext->Unmap( vertexBuffer, 0 );
	}

	// Release the arrays now that the buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;
	delete[] indices;
	indices = 0;
}

//Create the vertex and index buffers that will be passed along to the graphics card for rendering
void TerrainMesh::CreateBuffers( ID3D11Device* device, VertexType* vertices, unsigned long* indices ) {

	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	// Set up the description of the dyanmic vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof( VertexType ) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	// Now create the vertex buffer.
	device->CreateBuffer( &vertexBufferDesc, &vertexData, &vertexBuffer );

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof( unsigned long ) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	device->CreateBuffer( &indexBufferDesc, &indexData, &indexBuffer );
}

XMFLOAT3 TerrainMesh::specific_mesh_Offset(int x, int y)
{
	//if the mesh is the blade
	if (meshType == BLADE)
	{
		//creating temporary variables
		float tipOffset = pointHeight / resolution;
		XMFLOAT3 blade_offset = XMFLOAT3(0,0,0);
	
		//if the tip of the blade is valid
		if (pointHeight > 0)
		{
			//if x is in the first half of the mesh
			if (x < resolution / 2)
			{
				//apply offset with each y creating a bigger offset
				blade_offset.y = ((resolution / 2) * ((tipOffset / 10) * x)) / resolution;
				blade_offset.y = blade_offset.y * y;
			}
			else
			{
				//if the blade is onesided change the offset applied
				if (oneSided)
				{
					blade_offset.y = ((resolution / 2) * (((tipOffset / 10) * resolution) - ((tipOffset / 10) * (resolution - x))) / resolution);
					blade_offset.y = blade_offset.y * y;
				}
				else
				{
					blade_offset.y = ((resolution / 2) * (((tipOffset / 10) * resolution) - ((tipOffset / 10) * (x))) / resolution);
					blade_offset.y = blade_offset.y * y;
				}
			}
		}

		//if the blade mesh has curve enabled
		if (bezierCurve)
		{
			//set a temporary variable
			float temp_t = (float)y / (float)resolution;
			
			//if the blade is not symmetrical 
			if (symmterical == false)
			{
				//set the offset to 3rd degree bezier curve calculation
				blade_offset.z = (3 * temp_t * pow(1 - temp_t, 2) * bezierX[0] + 3 * pow(temp_t, 2) * (1 - temp_t) * bezierX[1] + pow(temp_t, 3) * bezierX[2]);
			}
			else
			{
				//if the blade is onesided
				if (oneSided)
				{
					//and the curve is inversed
					if (bezierInverse)
					{
						//apply the offset one direction
						blade_offset.z = (3 * temp_t * pow(1 - temp_t, 2) * bezierX[0] + 3 * pow(temp_t, 2) * (1 - temp_t) * bezierX[1] + pow(temp_t, 3) * bezierX[2]) * ((x) / ((float)resolution));
					}
					else
					{
						//apply the offset the other direction
						blade_offset.z = (3 * temp_t * pow(1 - temp_t, 2) * bezierX[0] + 3 * pow(temp_t, 2) * (1 - temp_t) * bezierX[1] + pow(temp_t, 3) * bezierX[2]) * ((x - (float)resolution) / ((float)resolution));
					}
				}
				else
				{
					//apply symmetrical offset
					blade_offset.z = (3 * temp_t * pow(1 - temp_t, 2) * bezierX[0] + 3 * pow(temp_t, 2) * (1 - temp_t) * bezierX[1] + pow(temp_t, 3) * bezierX[2]) * ((x - (float)(resolution / 2)) / ((float)resolution / 2));
				}
			}
		}

		//returning the offset;
		return blade_offset;
	}

	//if the mesh is the guard
	else if (meshType == GUARD)
	{
		//creating temp variable
		XMFLOAT3 guard_offset = XMFLOAT3(0, 0, 0);
		float temp_t = -(float)(x - resolution / 2) / (float)(resolution / 2);

		//if the guard has curve enabled
		if (bezierCurve)
		{
			//if x is within the first half of the mesh
			if (x < resolution / 2)
			{
				//if its not onesided
				if (oneSided == false)
				{
					//apply one degree bezier curve near the tip of the guard
					guard_offset.y = (pow(temp_t, 3) * bezierX[2]);
				}
				//if it is onesided
				else
				{
					//needs work ///////////////////////////////////////////////////////
					guard_offset.y = (pow(temp_t, 2.5) * (bezierX[2]*(oneSided_width/5)));
					guard_offset.z = -(pow(temp_t, 1.5) * bezierX[2]) - ((oneSided_width / (resolution/2) * x));
				}
			}
			//if in the other half
			else if (x > resolution/2)
			{
				//if the curve is inversed
				if (bezierInverse)
				{
					//apply one degree bezier offset
					guard_offset.y = (pow(temp_t, 3) * bezierX[2]);
				}
				else
				{	
					//applying negative offset
					guard_offset.y = -(pow(temp_t, 3) * bezierX[2]);
				}
			}
		}

		//returning the offset
		return guard_offset;
	}

	//if the mesh is the handle
	else if (meshType == GRIP)
	{
		XMFLOAT3 handle_offset = XMFLOAT3(0, 0, 0);
		if (meshLayers.size() > 0)
		{
			if (y == 0)
			{
				current_width = length_base;
				target_width = meshLayers.at(0);
				handle_increment = increment_update(current_width, target_width);
			}

			else if (y == resolution - 1)
			{
				current_width = length_top;
				target_width = length_top;
				handle_increment = increment_update(current_width, target_width);
			}

			else
			{
				for (int size = 0; size < meshLayers.size(); size++)
				{
					if (y == layer_div * (size + 1) && y < resolution)
					{
						current_width = target_width;
						incremtn_test = -1;

						if (size == (meshLayers.size() - 1))
						{
							target_width = length_top;
							handle_increment = increment_update(current_width, target_width);
						}
						else
						{
							target_width = meshLayers.at(size + 1);
							handle_increment = increment_update(current_width, target_width);
						}

						break;
					}
				}
			}

			if (x < resolution / 2)
			{
				handle_offset.z = ((-current_width / 2) - ((handle_increment * meshLayers.size()) * incremtn_test)) * -((x-(float)(resolution/2)) / ((float)resolution/2));
			}
			else 
			{
				handle_offset.z = ((current_width / 2) + ((handle_increment * meshLayers.size()) * incremtn_test)) * ((x-(float)(resolution/2)) / ((float)resolution/2));
			}
		}

		else
		{
			current_width = length_base;
			target_width = length_top;

			handle_increment = increment_update(current_width, target_width);

			if (x < resolution / 2)
			{
				handle_offset.z = ((-current_width /2) - (handle_increment * y)) * -((x - (float)(resolution / 2)) / ((float)resolution / 2));
			}
			else if(x > resolution/2)
			{
				handle_offset.z = ((current_width /2) + (handle_increment * y)) * ((x - (float)(resolution / 2)) / ((float)resolution / 2));
			}
		}

		incremtn_test++;
		return handle_offset;
	}

	//if the mesh is the pommel
	else if (meshType == POMMEL)
	{
		XMFLOAT3 pommel_offset = XMFLOAT3 (0,0,0);
		float temp_increment = (float)curve_degree / (float)resolution;

		pommel_increment = increment_update(length_base, length_top);

		if (inverse_pommel_curve == false)
		{
			if (x < resolution / 2)
			{
				pommel_offset.z = (((-length_base/2) - (pommel_increment * y)) - sin(((y * temp_increment) * PI / 180)) * pommel_point_curvature) * -((x - (float)(resolution / 2)) / ((float)resolution / 2));
			}
			else if (x > resolution / 2)
			{
				pommel_offset.z = (((length_base/2) + (pommel_increment*y)) + sin(((y * temp_increment) * PI / 180)) * pommel_point_curvature) * ((x - (float)(resolution / 2)) / ((float)resolution / 2));
			}
		}
		else
		{
			if (x < resolution / 2)
			{
				pommel_offset.z = (((-length_base / 2) - (pommel_increment * y)) + sin(((y * temp_increment) * PI / 180)) * pommel_point_curvature )  * -((x - (float)(resolution / 2)) / ((float)resolution / 2));
			}
			else if (x > resolution / 2)
			{
				pommel_offset.z = (((length_base / 2) + (pommel_increment * y)) - sin(((y * temp_increment) * PI / 180)) * pommel_point_curvature) * ((x - (float)(resolution / 2)) / ((float)resolution / 2));
			}
		}

		return pommel_offset;
	}
	return XMFLOAT3(0,0,0);
} 

//reuasble function to get incremental value
float TerrainMesh::increment_update(float base, float target)
{
	float temp_increment;

	temp_increment = ((((float)target / 2) - ((float)base / 2)) / (resolution));

	return temp_increment;
}

void TerrainMesh::addDamage_scrape(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	//setting variables
	XMFLOAT3 faultVector;
	XMFLOAT3 crossProduct;
	XMFLOAT3 targetVector;

	int	randomX = rand() % resolution;
	int	randomY = rand() % resolution;
	int	randomX2 = rand() % resolution;
	int	randomY2 = rand() % resolution;

	//calculating the fault line vector
	faultVector.x = randomX2 - randomX;
	faultVector.y = randomY2 - randomY;
	faultVector.z = heightMap[(randomY2 * resolution) + randomX2] - heightMap[(randomY * resolution) + randomX];

	//loops all mesh positions 
	for (int j = 0; j < (resolution); j++) {
		for (int i = 0; i < (resolution); i++) {

			//calculates cross product using the fault line vector and the target vector
			targetVector.x = randomX - i;
			targetVector.y = randomY - j;
			targetVector.z = heightMap[(randomY * resolution) + randomX] - heightMap[(j * resolution) + i];

			crossProduct.x = faultVector.y * targetVector.z - faultVector.z * targetVector.y;
			crossProduct.y = faultVector.z * targetVector.x - faultVector.x * targetVector.z;
			crossProduct.z = faultVector.x * targetVector.y - faultVector.y * targetVector.x;

			//moves the point up or down depending on the magnitude
			if (crossProduct.z > -0.1 && crossProduct.z < 0.1 )
			{
				offsetMap[(j * resolution) + i] -= 0.1;
			}
		}
	}

	Regenerate(device, deviceContext);
}

void TerrainMesh::addDamage_dents(ID3D11Device* device, ID3D11DeviceContext* deviceContext, float Amplitude, float Frequency)
{
		//randomises the seeding the 
		perlin_noise->RandomisePermutations(true);

		float offset = rand() % 10000;
		offset /= 33.3f;
		float ampVar;
		float freqVar;
		float height;

		//loops for all the points in the mesh
		for (int j = 0; j < (resolution); j++)
		{
			for (int i = 0; i < (resolution); i++)

			{
				//resetting variables
				ampVar = Amplitude;
				freqVar = Frequency;
				height = 0;

		
				//adds the extra height to the current height of the point in the height map
				
					height = (0.5f - perlin_noise->noise(offset + ((float)i) * freqVar, 0.0f, offset + ((float)j) * freqVar)) * ampVar;

					if (height > 0)
					{
						if (offsetMap[(j * resolution) + i] > 0)
						{
							offsetMap[(j * resolution) + i] -= height;
						}
					}
			}
		}

		//update the height map
		Regenerate(device, deviceContext);
}