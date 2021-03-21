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

struct base_variables
{
	float height, width, thickness;
};

struct bezier_variables 
{
	float bezier[3];
	bool bezier_curve, bezier_inverse;

};

struct fuller_variables
{
	int fuller_base, fuller_height, fuller_width;
	float fuller_depth;
};

struct oneSided_variables
{
	bool oneSided;
	float oneSided_width;
};

struct handle_loft_variables 
{
	int base_handle_width, top_handle_width;
	std::vector<int> layers;
};

struct curve_variables
{
	float Curvature;
	bool inverseCurve, isCurve, x_dimension, y_dimension;
};

struct blade_manager
{
	Blade* blade_mesh;
	Blade* mirrored_blade_mesh;

	base_variables blade_base_variables;
	fuller_variables blade_fuller_variables;
	bezier_variables blade_bezier_variables;
	oneSided_variables blade_oneSided_variables;
	
	float bladeTipHeight;
	int edge_offset;
	bool symmetrical;

	void regen(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	{
		blade_mesh->Regenerate( device, deviceContext);
		mirrored_blade_mesh->Regenerate(device, deviceContext);
	};

	void blade_edge()
	{
		blade_mesh->edge_sharpness();
		mirrored_blade_mesh->offsetMap = blade_mesh->offsetMap;
	};

	void blade_fuller()
	{
		blade_mesh->fuller();
		mirrored_blade_mesh->offsetMap = blade_mesh->offsetMap;
	};
};

struct guard_manager
{
	Guard* guard_mesh;
	Guard* mirrored_guard_mesh;

	base_variables guard_base_variables;
	curve_variables guard_curve_variables;
	bezier_variables guard_bezier_variables;
	oneSided_variables guard_oneSided_variables;
	

	void regen(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	{
		guard_mesh->Regenerate(device, deviceContext);
		mirrored_guard_mesh->Regenerate(device, deviceContext);
	};

	void guard_curve()
	{
		guard_mesh->guardMeshCurve();
		mirrored_guard_mesh->offsetMap = guard_mesh->offsetMap;
	}
};

struct handle_manager
{
	Handle* handle_mesh;
	Handle* mirrored_handle_mesh;

	base_variables handle_base_variables;
	curve_variables handle_curve_variables;
	handle_loft_variables loft_variables;

	void regen(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	{
		handle_mesh->Regenerate(device, deviceContext);
		mirrored_handle_mesh->Regenerate(device, deviceContext);
	};

	void handle_curve()
	{
		handle_mesh->handleMeshCurve();
		mirrored_handle_mesh->offsetMap = handle_mesh->offsetMap;
	}
};

struct pommel_manager
{
	Pommel* pommel_mesh;
	Pommel* mirrored_pommel_mesh;

	base_variables pommel_base_variables;
	curve_variables pommel_curve_variables_thickness;
	curve_variables pommel_curve_variables_width;
	oneSided_variables pommel_oneSided_variables;

	float top_width, bottom_width;
	int curve_degree;

	void regen(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	{
		pommel_mesh->Regenerate(device, deviceContext);
		mirrored_pommel_mesh->Regenerate(device, deviceContext);
	};

	void pommel_curve()
	{
		pommel_mesh->pommelMeshCurve();
		mirrored_pommel_mesh->offsetMap = pommel_mesh->offsetMap;
	}
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

	blade_manager blade;
	guard_manager guard;
	handle_manager handle;
	pommel_manager pommel;

	int damage_iterations;
	float Amplitude, Frequency;
	float weaponThickness, lower_weapon_width, lower_weapon_height, lower_weapon_curve, blade_tip, blade_edge;
	bool realistic_variables, detailed_UI, twoHanded;

};

#endif