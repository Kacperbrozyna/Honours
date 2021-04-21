#pragma once
#include "TerrainMesh.h"


class Blade : public TerrainMesh
{
public:
	Blade(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int resolution = 128);
	~Blade();

	void edge_sharpness();
	void build_fuller();

	const inline bool Get_edge_tip() { return edge_tip; }
	void Set_edge_tip(bool tipEdge) { edge_tip = tipEdge; }

	const inline bool Get_fuller() { return fuller; }
	void Set_fuller(bool blade_fuller) { fuller = blade_fuller; }

	const inline float Get_fuller_depth() { return fuller_depth; }
	void Set_fuller_depth(float fullerDepth) { fuller_depth = fullerDepth; }

	const inline int Get_fuller_base() { return fuller_base; }
	void Set_fuller_base(int fullerBase) { fuller_base = fullerBase; }

	const inline int Get_fuller_width() { return fuller_width; }
	void Set_fuller_width(int fullerWidth) { fuller_width = fullerWidth; }

	const inline int Get_fuller_height() { return fuller_height; }
	void Set_fuller_height(int fullerHeight) { fuller_height = fullerHeight; }

private:
	int fuller_base, fuller_width, fuller_height = GetHeight() / 2;
	float fuller_depth;
	bool edge_tip, fuller;
};

