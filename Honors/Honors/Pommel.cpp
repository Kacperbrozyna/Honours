#include "Pommel.h"

Pommel::Pommel(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int resolution) :
	TerrainMesh(device, deviceContext, resolution)
{
}

Pommel::~Pommel()
{}