#include "Guard.h"

Guard::Guard(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int resolution) :
	TerrainMesh(device, deviceContext, resolution)
{
}

Guard::~Guard()
{
}