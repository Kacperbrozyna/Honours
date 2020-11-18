#include "Handle.h"

Handle::Handle(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int resolution) :
	TerrainMesh(device, deviceContext, resolution)
{
}

Handle::~Handle()
{}