#pragma once
#include "PlaneMesh.h"
class TerrainMesh :
	public PlaneMesh {
public:
	TerrainMesh( ID3D11Device* device, ID3D11DeviceContext* deviceContext, int resolution);
	~TerrainMesh();

	void Resize( int newResolution );
	void Regenerate( ID3D11Device* device, ID3D11DeviceContext* deviceContext );

	const inline int GetResolution(){ return resolution; }

	const inline int GetHeight() { return height; }
	void SetHeight(float Height) { height = Height; }
	
	const inline int GetWidth() { return width; }
	void SetWidth(float Width) { width = Width; }

	const inline int GetThickness() { return thickness;}
	void SetThickness(float Thickness) { thickness = Thickness; }

	void setMirrored(bool Mirror) { mirrored = Mirror;}

private:
	void CreateBuffers( ID3D11Device* device, VertexType* vertices, unsigned long* indices );
	void BuildEdges();

	const float m_UVscale = 10.0f;			//Tile the UV map 10 times across the plane
	const float terrainSize = 25.0f;		//What is the width and height of our terrain
	float* heightMap;
	float height = 50, width =20, thickness = 2;
	
	bool mirrored;
};
