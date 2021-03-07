#pragma once
#include "PlaneMesh.h"
#include <vector>

class TerrainMesh :
	public PlaneMesh {
public:
	TerrainMesh( ID3D11Device* device, ID3D11DeviceContext* deviceContext, int resolution);
	~TerrainMesh();

	#define PI 3.14159265

	void Resize( int newResolution );
	void Regenerate( ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	const inline int GetResolution(){ return resolution; }

	const inline float GetHeight() { return height; }
	void SetHeight(float Height) { height = Height; }
	
	const inline float GetWidth() { return width; }
	void SetWidth(float Width) { width = Width; }

	const inline float GetThickness() { return thickness;}

	void SetThickness(float Thickness) { thickness = Thickness; }
	void SetOffsetZ(float pos_OffsetZ) { position_offsetZ = pos_OffsetZ; }
	void SetOffsetY(float pos_OffsetY) { position_offsetY = pos_OffsetY; }
	void setMirrored(bool Mirror) { mirrored = Mirror;}

	void addDamage();

	const inline float GetDynamicHeight() { return dynamic_height; }
	const inline float GetDynamicWidth() { return dynamic_width; }


	int edge_offset;
	bool blade, guard, handle, pommel, inverseCurve, isCurve;
	float pointHeight;
	float curvature_value;
	const float terrainSize = 25.0f;		//What is the width and height of our terrain
	float* heightMap;
	float* offsetMap;

	std::vector<int> meshLayers;
	float length_base, length_top;
	float pommel_point_curvature; 
	int curve_degree;
	bool inverse_pommel_curve;
	float bezierX[3];
	bool bezierCurve, bezierInverse;

private:

	void CreateBuffers( ID3D11Device* device, VertexType* vertices, unsigned long* indices );
	void BuildEdges();

	XMFLOAT3 specific_mesh_Offset(int x, int y);
	float increment_update(float base, float target);

	const float m_UVscale = 10.0f;			//Tile the UV map 10 times across the plane

	float height = 50, width =20, thickness = 2;
	float position_offsetZ, position_offsetY;

	float dynamic_height, dynamic_width;
	bool mirrored;

	float handle_increment, pommel_increment;
	float current_width, target_width;
	int layer_div;
	int incremtn_test;
};
