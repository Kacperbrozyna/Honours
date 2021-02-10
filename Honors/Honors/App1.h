// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"	// include dxframework
#include "LightShader.h"
#include "Blade.h"
#include "Pommel.h"
#include "Handle.h"
#include "Guard.h"

struct blade_variables
{
	Blade* normal_blade;
	Blade* mirrored_blade;
	float bladeHeight, bladeWidth, bladeThickness, bladeTipHeight, Curvature;
	bool inverseCurve, isCurve;
	int edge_offset;
	int fuller_base, fuller_height, fuller_width;
	float fuller_depth;

	void regen(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	{
		normal_blade->Regenerate( device, deviceContext);
		mirrored_blade->Regenerate(device, deviceContext);
	};

	void blade_edge()
	{
		normal_blade->edge_sharpness();
		mirrored_blade->edge_sharpness();
	};

	void blade_fuller()
	{
		normal_blade->fuller();
		mirrored_blade->fuller();
	};
};

struct guard_variables
{
	Guard* normal_guard;
	Guard* mirrored_guard;
	float guardHeight, guardWidth, guardThickness;

	void regen(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	{
		normal_guard->Regenerate(device, deviceContext);
		mirrored_guard->Regenerate(device, deviceContext);
	};
};

struct handle_variables
{
	Handle* normal_handle;
	Handle* mirrored_handle;
	float handleHeight, handleWidth, handleThickness;

	void regen(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	{
		normal_handle->Regenerate(device, deviceContext);
		mirrored_handle->Regenerate(device, deviceContext);
	};
};

struct pommel_variables
{
	Pommel* normal_pommel;
	Pommel* mirrored_pommel;
	float pommelHeight, pommelWidth, pommelThickness;
	bool inverseCurve;

	void regen(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	{
		normal_pommel->Regenerate(device, deviceContext);
		mirrored_pommel->Regenerate(device, deviceContext);
	};
};

class App1 : public BaseApplication
{
public:
	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN);

	bool frame();

protected:
	bool render();
	void gui();
	void initValues();
	void updateHeights();
	void updateWidth();

private:

	LightShader* shader;
	Light* light;
	int terrainResolution = 128;

	blade_variables blade;
	guard_variables guard;
	handle_variables handle;
	pommel_variables pommel;
};

#endif