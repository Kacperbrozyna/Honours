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
	void updateThickness();

private:
	LightShader* shader;

	Blade* blade;
	Blade* blade_mirrored;

	Handle* handle;
	Handle* handle_mirrored;

	Guard* guard;
	Guard* guard_mirrored;

	Pommel* pommel;
	Pommel* pommel_mirrored;

	Light* light;

	int terrainResolution = 128;

	float bladeHeight, bladeWidth, bladeThickness;
	float handleHeight, handleWidth, handleThickness;
	float guardHeight, guardWidth, guardThickness;
	float pommelHeight, pommelWidth, pommelThickness;
};

#endif