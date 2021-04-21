#pragma once
#include "PlaneMesh.h"
#include <vector>
#include "Perlin.h"

enum mesh_Type{BLADE, GUARD, GUARD_FINGERS, GRIP, POMMEL};


class TerrainMesh :
	public PlaneMesh {
public:
	TerrainMesh( ID3D11Device* device, ID3D11DeviceContext* deviceContext, int resolution);
	~TerrainMesh();

	#define PI 3.14159265

	void Resize( int newResolution );
	void Regenerate( ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	const inline int GetResolution(){ return resolution; }

	const inline int Get_edge_offset() { return edge_offset; }
	void Set_edge_offset(int edgeOffset) { edge_offset = edgeOffset; }

	const inline bool Get_inverse_curve() { return inverseCurve; }
	void Set_inverse_curve(bool inverse_curve) { inverseCurve = inverse_curve; }

	const inline bool Get_Iscurve() { return isCurve; }
	void Set_Iscurve(bool is_curve) { isCurve = is_curve; }

	const inline float Get_point_height() { return pointHeight; }
	void Set_point_height(float point_height) { pointHeight = point_height; }

	const inline float Get_curvature_value() { return curvature_value; }
	void Set_curvature_value(float curvatureValue) { curvature_value = curvatureValue; }

	const inline float Get_length_base() { return length_base; }
	void Set_length_base(float lengthBase) { length_base = lengthBase; }

	const inline float Get_length_top() { return length_top; }
	void Set_length_top(float lengthTop) { length_top = lengthTop; }

	const inline float Get_pommel_point_curvature() { return pommel_point_curvature; }
	void Set_pommel_point_curvature(float pommelCurvature) { pommel_point_curvature = pommelCurvature; }

	const inline int Get_curve_degree() { return curve_degree; }
	void Set_curve_degree(int curve_Degree) { curve_degree = curve_Degree; }

	const inline bool Get_inverse_pommel_curve() { return inverse_pommel_curve; }
	void Set_inverse_pommel_curve(bool inverse_pommelCurve) { inverse_pommel_curve = inverse_pommelCurve; }

	const inline bool Get_bezier_curve() { return bezierCurve; }
	void Set_bezier_curve(bool bezier_curve) { bezierCurve = bezier_curve; }

	const inline bool Get_inverse_bezier_curve() { return bezierInverse; }
	void Set_inverse_bezier_curve(bool inverse_bezierCurve) { bezierInverse = inverse_bezierCurve; }

	const inline bool Get_oneSided() { return oneSided; }
	void Set_oneSided(bool one_sided) { oneSided = one_sided; }

	const inline bool Get_fingerGuard() { return fingerGuard; }
	void Set_fingerGuard(bool finger_guard) { fingerGuard = finger_guard; }

	const inline bool Get_Pommel() { return pommel; }
	void Set_pommel(bool pommel_) { pommel = pommel_; }

	const inline bool Get_Symmetrical() { return symmterical; }
	void Set_symmetrical(bool Symmetrical) { symmterical = Symmetrical; }

	const inline bool Get_side_tip() { return side_tip; }
	void Set_side_tip(bool sideTip) { side_tip = sideTip; }

	const inline float GetHeight() { return height; }
	void SetHeight(float Height) { height = Height; }
	
	const inline float GetWidth() { return width; }
	void SetWidth(float Width) { width = Width; }

	const inline float GetThickness() { return thickness;}

	void SetThickness(float Thickness) { thickness = Thickness; }

	void SetOffsetZ(float pos_OffsetZ) { position_offsetZ = pos_OffsetZ; }
	void SetOffsetY(float pos_OffsetY) { position_offsetY = pos_OffsetY; }

	void setMirrored(bool Mirror) { mirrored = Mirror;}

	void addDamage_dents(ID3D11Device* device, ID3D11DeviceContext* deviceContext, float Amplitude, float Frequency);
	void addDamage_scrape(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	const inline float GetDynamicHeight() { return dynamic_height; }
	const inline float GetDynamicWidth() { return dynamic_width; }

	const float terrainSize = 25.0f;		//What is the width and height of our terrain
	float* heightMap;
	float* offsetMap;

	std::vector<int> meshLayers;
	mesh_Type  meshType;
	float bezierX[3];

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
	bool inverseCurve, isCurve, inverse_pommel_curve, bezierCurve, bezierInverse, fingerGuard, pommel, oneSided, symmterical, side_tip;
	float handle_increment, pommel_increment, fingerguard_increment, pointHeight, curvature_value, length_base, length_top, pommel_point_curvature, current_width, target_width;
	int increment, curve_degree, edge_offset;

	
	std::vector<float> loft;
	Perlin* perlin_noise;
};
