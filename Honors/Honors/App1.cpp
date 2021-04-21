#include "App1.h"

App1::App1()
{
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in, bool VSYNC, bool FULL_SCREEN)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);

	textureMgr->loadTexture(L"damascus", L"res/Silver.jpg");	
	textureMgr->loadTexture(L"bronze", L"res/Bronze_.jpg");
	textureMgr->loadTexture(L"leather", L"res/Leather_.jpg");

	// Create Mesh object and shader object
	blade.blade_mesh = new Blade(renderer->getDevice(), renderer->getDeviceContext());
	blade.mirrored_blade_mesh = new Blade(renderer->getDevice(), renderer->getDeviceContext());

	guard.guard_mesh = new Guard(renderer->getDevice(), renderer->getDeviceContext());
	guard.mirrored_guard_mesh = new Guard(renderer->getDevice(), renderer->getDeviceContext());

	guard.guard_finger_mesh = new Guard_Fingers(renderer->getDevice(), renderer->getDeviceContext());
	guard.guard_finger_mirrored_mesh = new Guard_Fingers(renderer->getDevice(), renderer->getDeviceContext());

	handle.handle_mesh = new Handle(renderer->getDevice(), renderer->getDeviceContext());
	handle.mirrored_handle_mesh = new Handle(renderer->getDevice(), renderer->getDeviceContext());

	pommel.pommel_mesh = new Pommel(renderer->getDevice(), renderer->getDeviceContext());
	pommel.mirrored_pommel_mesh = new Pommel(renderer->getDevice(), renderer->getDeviceContext());

	shader = new LightShader(renderer->getDevice(), hwnd);
	light = new Light;

	initValues();
}


App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.
	if (shader)
	{
		delete shader;
		shader = 0;
	}
}


bool App1::frame()
{
	bool result;

		//Statements for Blade

	if (!detailed_UI)
	{
		if (twoHanded)
		{
			blade.bladeTipHeight = 2;
			blade.tip_edge = true;
			blade.edge_offset = terrainResolution / 3;
		}
		else
		{
			blade.bladeTipHeight = 2.5;
			blade.tip_edge = false;
			blade.edge_offset = terrainResolution / 2;
		}

		if (rightCurve)
		{
			blade.blade_bezier_variables.bezier_curve = true;
			blade.blade_bezier_variables.bezier[0] = 0;
			blade.blade_bezier_variables.bezier[1] = 0.75;
			blade.blade_bezier_variables.bezier[2] = -5;
		}
		else if (leftCurve)
		{
			blade.blade_bezier_variables.bezier_curve = true;
			blade.blade_bezier_variables.bezier[0] = 0;
			blade.blade_bezier_variables.bezier[1] = -0.75;
			blade.blade_bezier_variables.bezier[2] = 5;
		}
		else
		{
			blade.blade_bezier_variables.bezier_curve = false;
		}

		if (blade.blade_fuller_variables.fuller)
		{
			blade.blade_fuller_variables.fuller_base = 0;
			blade.blade_fuller_variables.fuller_height = terrainResolution * 0.8;
			blade.blade_fuller_variables.fuller_width = blade.blade_base_variables.width * 0.25;
			blade.blade_fuller_variables.fuller_depth = blade.blade_base_variables.thickness * 0.15;
		}
	}

		if (blade.blade_base_variables.height != blade.blade_mesh->GetHeight())
		{
			blade.mirrored_blade_mesh->SetHeight(blade.blade_base_variables.height);
			blade.blade_mesh->SetHeight(blade.blade_base_variables.height);
			blade.blade_fuller();
			blade.blade_edge();
			updateHeights();
		}

		if (blade.blade_base_variables.thickness != blade.blade_mesh->GetThickness())
		{
			blade.blade_mesh->SetThickness(blade.blade_base_variables.thickness);
			blade.mirrored_blade_mesh->SetThickness(blade.blade_base_variables.thickness);
			blade.blade_fuller();
			blade.blade_edge();
			blade.regen(renderer->getDevice(), renderer->getDeviceContext());
		}

		if (blade.blade_base_variables.width != blade.blade_mesh->GetWidth())
		{
			blade.blade_mesh->SetWidth(blade.blade_base_variables.width);
			blade.mirrored_blade_mesh->SetWidth(blade.blade_base_variables.width);
			blade.blade_fuller();
			blade.blade_edge();
			updateWidth();
		}

		if (blade.bladeTipHeight != blade.blade_mesh->Get_point_height())
		{
			blade.blade_mesh->Set_point_height(blade.bladeTipHeight);
			blade.mirrored_blade_mesh->Set_point_height(blade.bladeTipHeight);
			blade.regen(renderer->getDevice(), renderer->getDeviceContext());
		}

		if (blade.blade_bezier_variables.bezier_curve != blade.blade_mesh->Get_bezier_curve())
		{
			blade.blade_mesh->Set_bezier_curve(blade.blade_bezier_variables.bezier_curve);
			blade.mirrored_blade_mesh->Set_bezier_curve(blade.blade_bezier_variables.bezier_curve);
			blade.regen(renderer->getDevice(), renderer->getDeviceContext());
		}

		if (blade.edge_offset != blade.blade_mesh->Get_edge_offset())
		{
			blade.blade_mesh->Set_edge_offset(blade.edge_offset);
			blade.mirrored_blade_mesh->Set_edge_offset(blade.edge_offset);

			blade.blade_edge();
			blade.regen(renderer->getDevice(), renderer->getDeviceContext());
		}

		if (blade.blade_fuller_variables.fuller != blade.blade_mesh->Get_fuller())
		{
			blade.blade_mesh->Set_fuller(blade.blade_fuller_variables.fuller);
			blade.mirrored_blade_mesh->Set_fuller(blade.blade_fuller_variables.fuller);
			if (blade.blade_fuller_variables.fuller == false)
			{
				blade.blade_fuller();
				blade.blade_edge();
			}
			else
			{
				blade.blade_edge();
				blade.blade_fuller();
			}
			blade.regen(renderer->getDevice(), renderer->getDeviceContext());
		}

	if (blade.blade_fuller_variables.fuller)
	{
		
		if (blade.blade_fuller_variables.fuller_base != blade.blade_mesh->Get_fuller_base())
		{
			blade.blade_mesh->Set_fuller_base(blade.blade_fuller_variables.fuller_base);
			blade.mirrored_blade_mesh->Set_fuller_base(blade.blade_fuller_variables.fuller_base);
			blade.blade_edge();
			blade.blade_fuller();
			blade.regen(renderer->getDevice(), renderer->getDeviceContext());
		}

		if (blade.blade_fuller_variables.fuller_height != blade.blade_mesh->Get_fuller_height())
		{
			blade.blade_mesh->Set_fuller_height(blade.blade_fuller_variables.fuller_height);
			blade.mirrored_blade_mesh->Set_fuller_height(blade.blade_fuller_variables.fuller_height);
			blade.blade_edge();
			blade.blade_fuller();
			blade.regen(renderer->getDevice(), renderer->getDeviceContext());
		}

		if (blade.blade_fuller_variables.fuller_width != blade.blade_mesh->Get_fuller_width())
		{
			blade.blade_mesh->Set_fuller_width(blade.blade_fuller_variables.fuller_width);
			blade.mirrored_blade_mesh->Set_fuller_width(blade.blade_fuller_variables.fuller_width);
			blade.blade_edge();
			blade.blade_fuller();
			blade.regen(renderer->getDevice(), renderer->getDeviceContext());
		}

		if (blade.blade_fuller_variables.fuller_depth != blade.blade_mesh->Get_fuller_depth())
		{
			blade.blade_mesh->Set_fuller_depth(blade.blade_fuller_variables.fuller_depth);
			blade.mirrored_blade_mesh->Set_fuller_depth(blade.blade_fuller_variables.fuller_depth);
			blade.blade_edge();
			blade.blade_fuller();
			blade.regen(renderer->getDevice(), renderer->getDeviceContext());
		}
	}

		for (int x = 0; x < 3; x++)
		{
			if (blade.blade_bezier_variables.bezier[x] != blade.blade_mesh->bezierX[x])
			{
				blade.blade_mesh->bezierX[x] = blade.blade_bezier_variables.bezier[x];
				blade.mirrored_blade_mesh->bezierX[x] = blade.blade_bezier_variables.bezier[x];
				blade.regen(renderer->getDevice(), renderer->getDeviceContext());
			}
		}

		if (blade.symmetrical != blade.blade_mesh->Get_Symmetrical())
		{
			blade.blade_mesh->Set_symmetrical(blade.symmetrical);
			blade.mirrored_blade_mesh->Set_symmetrical(blade.symmetrical);

			blade.regen(renderer->getDevice(), renderer->getDeviceContext());
		}

		if (blade.blade_oneSided_variables.oneSided != blade.blade_mesh->Get_oneSided())
		{
			blade.blade_mesh->Set_oneSided(blade.blade_oneSided_variables.oneSided);
			blade.mirrored_blade_mesh->Set_oneSided(blade.blade_oneSided_variables.oneSided);
			blade.regen(renderer->getDevice(), renderer->getDeviceContext());
		}

		if (blade.blade_bezier_variables.bezier_inverse != blade.blade_mesh->Get_inverse_bezier_curve())
		{
			blade.blade_mesh->Set_inverse_bezier_curve(blade.blade_bezier_variables.bezier_inverse);
			blade.mirrored_blade_mesh->Set_inverse_bezier_curve(blade.blade_bezier_variables.bezier_inverse);
			blade.regen(renderer->getDevice(), renderer->getDeviceContext());
		}

		if (blade.sideTip != blade.blade_mesh->Get_side_tip())
		{
			blade.blade_mesh->Set_side_tip(blade.sideTip);
			blade.mirrored_blade_mesh->Set_side_tip(blade.sideTip);
			blade.regen(renderer->getDevice(), renderer->getDeviceContext());
		}

		if (blade.tip_edge != blade.blade_mesh->Get_edge_tip())
		{
			blade.blade_mesh->Set_edge_tip(blade.tip_edge);
			blade.mirrored_blade_mesh->Set_edge_tip(blade.tip_edge);
			blade.regen(renderer->getDevice(), renderer->getDeviceContext());
		}

		//Statements for Guard

		if (guard.guard_base_variables.height != guard.guard_mesh->GetHeight())
		{
			guard.mirrored_guard_mesh->SetHeight(guard.guard_base_variables.height);
			guard.guard_mesh->SetHeight(guard.guard_base_variables.height);

			updateHeights();
		}

		if (guard.guard_base_variables.thickness != guard.guard_mesh->GetThickness())
		{
			guard.guard_mesh->SetThickness(guard.guard_base_variables.thickness);
			guard.mirrored_guard_mesh->SetThickness(guard.guard_base_variables.thickness);
			guard.guard_curve();
			guard.regen(renderer->getDevice(), renderer->getDeviceContext());
		}

		if (guard.guard_base_variables.width != guard.guard_mesh->GetWidth())
		{
			guard.guard_mesh->SetWidth(guard.guard_base_variables.width);
			guard.mirrored_guard_mesh->SetWidth(guard.guard_base_variables.width);

			updateWidth();
		}

		if (guard.guard_bezier_variables.bezier_curve != guard.guard_mesh->Get_bezier_curve())
		{
			guard.guard_mesh->Set_bezier_curve(guard.guard_bezier_variables.bezier_curve);
			guard.mirrored_guard_mesh->Set_bezier_curve(guard.guard_bezier_variables.bezier_curve);
			guard.regen(renderer->getDevice(), renderer->getDeviceContext());
		}

		if (guard.guard_bezier_variables.bezier_inverse != guard.guard_mesh->Get_inverse_bezier_curve())
		{
			guard.guard_mesh->Set_inverse_bezier_curve(guard.guard_bezier_variables.bezier_inverse);
			guard.mirrored_guard_mesh->Set_inverse_bezier_curve(guard.guard_bezier_variables.bezier_inverse);
			guard.regen(renderer->getDevice(), renderer->getDeviceContext());
		}

		if (guard.guard_bezier_variables.bezier[2] != guard.guard_mesh->bezierX[2])
		{
			guard.guard_mesh->bezierX[2] = guard.guard_bezier_variables.bezier[2];
			guard.mirrored_guard_mesh->bezierX[2] = guard.guard_bezier_variables.bezier[2];
			guard.regen(renderer->getDevice(), renderer->getDeviceContext());
		}

		if (guard.guard_curve_variables.Curvature != guard.guard_mesh->Get_curvature_value())
		{
			guard.guard_mesh->Set_curvature_value(guard.guard_curve_variables.Curvature);
			guard.mirrored_guard_mesh->Set_curvature_value(guard.guard_curve_variables.Curvature);

			guard.guard_curve();
			guard.regen(renderer->getDevice(), renderer->getDeviceContext());
		}

		if (guard.guard_curve_variables.inverseCurve != guard.guard_mesh->Get_inverse_curve())
		{
			guard.guard_mesh->Set_inverse_curve(guard.guard_curve_variables.inverseCurve);
			guard.mirrored_guard_mesh->Set_inverse_curve(guard.guard_curve_variables.inverseCurve);
			guard.guard_curve();
			guard.regen(renderer->getDevice(), renderer->getDeviceContext());
		}

		if (guard.guard_curve_variables.x_dimension != guard.guard_mesh->Get_x_dimension_curve())
		{
			guard.guard_mesh->Set_x_dimension_curve(guard.guard_curve_variables.x_dimension);
			guard.mirrored_guard_mesh->Set_x_dimension_curve(guard.guard_curve_variables.x_dimension);
			guard.guard_curve();
			guard.regen(renderer->getDevice(), renderer->getDeviceContext());
		}

		if (guard.guard_curve_variables.y_dimension != guard.guard_mesh->Get_y_dimension_curve())
		{
			guard.guard_mesh->Set_y_dimension_curve(guard.guard_curve_variables.y_dimension);
			guard.mirrored_guard_mesh->Set_y_dimension_curve(guard.guard_curve_variables.y_dimension);
			guard.guard_curve();
			guard.regen(renderer->getDevice(), renderer->getDeviceContext());
		}

		//statments for Finger Guard
		if (guard.fingerGuard != guard.guard_finger_mesh->Get_fingerGuard())
		{
			guard.guard_finger_mesh->Set_fingerGuard(guard.fingerGuard);
			guard.guard_finger_mirrored_mesh->Set_fingerGuard(guard.fingerGuard);
			guard.finger_regen(renderer->getDevice(), renderer->getDeviceContext());
		}

		if (guard.fingerGuard)
		{
			if (guard.guard_finger_base_variables.height != guard.guard_finger_mesh->GetHeight())
			{
				guard.guard_finger_mirrored_mesh->SetHeight(guard.guard_finger_base_variables.height);
				guard.guard_finger_mesh->SetHeight(guard.guard_finger_base_variables.height);

				updateHeights();
			}

			if (guard.guard_finger_base_variables.thickness != guard.guard_finger_mesh->GetThickness())
			{
				guard.guard_finger_mesh->SetThickness(guard.guard_finger_base_variables.thickness);
				guard.guard_finger_mirrored_mesh->SetThickness(guard.guard_finger_base_variables.thickness);
				guard.finger_guard_curve();
				guard.finger_regen(renderer->getDevice(), renderer->getDeviceContext());
			}

			if (guard.guard_finger_base_variables.width != guard.guard_finger_mesh->GetWidth())
			{
				guard.guard_finger_mesh->SetWidth(guard.guard_finger_base_variables.width);
				guard.guard_finger_mirrored_mesh->SetWidth(guard.guard_finger_base_variables.width);

				updateWidth();
			}

			if (fingerGuard_offset.x != guard.finger_guard_offset.x)
			{
				fingerGuard_offset.x = guard.finger_guard_offset.x;
				updateWidth();
			}

			if (fingerGuard_offset.y != guard.finger_guard_offset.y)
			{
				fingerGuard_offset.y = guard.finger_guard_offset.y;
				updateHeights();
			}

			if (guard.guard_finger_bezier_variables.bezier_curve != guard.guard_finger_mesh->Get_bezier_curve())
			{
				guard.guard_finger_mesh->Set_bezier_curve(guard.guard_finger_bezier_variables.bezier_curve);
				guard.guard_finger_mirrored_mesh->Set_bezier_curve(guard.guard_finger_bezier_variables.bezier_curve);
				guard.finger_regen(renderer->getDevice(), renderer->getDeviceContext());
			}

			for (int gx = 0; gx < 3; gx++)
			{
				if (guard.guard_finger_bezier_variables.bezier[gx] != guard.guard_finger_mesh->bezierX[gx])
				{
					guard.guard_finger_mesh->bezierX[gx] = guard.guard_finger_bezier_variables.bezier[gx];
					guard.guard_finger_mirrored_mesh->bezierX[gx] = guard.guard_finger_bezier_variables.bezier[gx];
					guard.finger_regen(renderer->getDevice(), renderer->getDeviceContext());
				}
			}

			if (guard.guard_finger_curve_variables.Curvature != guard.guard_finger_mesh->Get_curvature_value())
			{
				guard.guard_finger_mesh->Set_curvature_value(guard.guard_finger_curve_variables.Curvature);
				guard.guard_finger_mirrored_mesh->Set_curvature_value(guard.guard_finger_curve_variables.Curvature);

				guard.finger_guard_curve();
				guard.finger_regen(renderer->getDevice(), renderer->getDeviceContext());
			}

			if (guard.guard_finger_curve_variables.inverseCurve != guard.guard_finger_mesh->Get_inverse_curve())
			{
				guard.guard_finger_mesh->Set_inverse_curve(guard.guard_finger_curve_variables.inverseCurve);
				guard.guard_finger_mirrored_mesh->Set_inverse_curve(guard.guard_finger_curve_variables.inverseCurve);
				guard.finger_guard_curve();
				guard.finger_regen(renderer->getDevice(), renderer->getDeviceContext());
			}

			if (guard.guard_finger_curve_variables.x_dimension != guard.guard_finger_mesh->Get_x_dimension_curve())
			{
				guard.guard_finger_mesh->Set_x_dimension_curve(guard.guard_finger_curve_variables.x_dimension);
				guard.guard_finger_mirrored_mesh->Set_x_dimension_curve(guard.guard_finger_curve_variables.x_dimension);
				guard.finger_guard_curve();
				guard.finger_regen(renderer->getDevice(), renderer->getDeviceContext());
			}

			if (guard.guard_finger_curve_variables.y_dimension != guard.guard_finger_mesh->Get_y_dimension_curve())
			{
				guard.guard_finger_mesh->Set_y_dimension_curve(guard.guard_finger_curve_variables.y_dimension);
				guard.guard_finger_mirrored_mesh->Set_y_dimension_curve(guard.guard_finger_curve_variables.y_dimension);
				guard.finger_guard_curve();
				guard.finger_regen(renderer->getDevice(), renderer->getDeviceContext());
			}

			if (guard.guard_finger_loft_variables.base_width != guard.guard_finger_mesh->Get_length_base())
			{
				guard.guard_finger_mesh->Set_length_base(guard.guard_finger_loft_variables.base_width);
				guard.guard_finger_mirrored_mesh->Set_length_base(guard.guard_finger_loft_variables.base_width);
				guard.finger_regen(renderer->getDevice(), renderer->getDeviceContext());
			}

			if (guard.guard_finger_loft_variables.top_width != guard.guard_finger_mesh->Get_length_top())
			{
				guard.guard_finger_mesh->Set_length_top(guard.guard_finger_loft_variables.top_width);
				guard.guard_finger_mirrored_mesh->Set_length_top(guard.guard_finger_loft_variables.top_width);
				guard.finger_regen(renderer->getDevice(), renderer->getDeviceContext());
			}
		}
		//Statements for Handle

		if (handle.handle_base_variables.height != handle.handle_mesh->GetHeight())
		{
			handle.mirrored_handle_mesh->SetHeight(handle.handle_base_variables.height);
			handle.handle_mesh->SetHeight(handle.handle_base_variables.height);

			if (realistic_variables)
			{
				guard.guard_finger_base_variables.height = handle.handle_base_variables.height;
			}

			updateHeights();
		}

		if (handle.handle_base_variables.thickness != handle.handle_mesh->GetThickness())
		{
			handle.handle_mesh->SetThickness(handle.handle_base_variables.thickness);
			handle.mirrored_handle_mesh->SetThickness(handle.handle_base_variables.thickness);
			handle.handle_curve();
			handle.regen(renderer->getDevice(), renderer->getDeviceContext());
		}

		if (handle.handle_base_variables.width != handle.handle_mesh->GetWidth())
		{
			handle.handle_mesh->SetWidth(handle.handle_base_variables.width);
			handle.mirrored_handle_mesh->SetWidth(handle.handle_base_variables.width);

			updateWidth();
		}

		if (handle.handle_loft_variables.base_width != handle.handle_mesh->Get_length_base())
		{
			handle.handle_mesh->Set_length_base(handle.handle_loft_variables.base_width);
			handle.mirrored_handle_mesh->Set_length_base(handle.handle_loft_variables.base_width);
			handle.regen(renderer->getDevice(), renderer->getDeviceContext());
		}

		if (handle.handle_loft_variables.top_width != handle.handle_mesh->Get_length_top())
		{
			handle.handle_mesh->Set_length_top(handle.handle_loft_variables.top_width);
			handle.mirrored_handle_mesh->Set_length_top(handle.handle_loft_variables.top_width);
			handle.regen(renderer->getDevice(), renderer->getDeviceContext());
		}

		for (int s = 0; s < handle.handle_loft_variables.layers.size(); s++)
		{
			if (handle.handle_loft_variables.layers.at(s) != handle.handle_mesh->meshLayers.at(s))
			{
				handle.handle_mesh->meshLayers.at(s) = handle.handle_loft_variables.layers.at(s);
				handle.mirrored_handle_mesh->meshLayers.at(s) = handle.handle_loft_variables.layers.at(s);
				handle.regen(renderer->getDevice(), renderer->getDeviceContext());
			}
		}

		if (handle.handle_curve_variables.Curvature != handle.handle_mesh->Get_curvature_value())
		{
			handle.handle_mesh->Set_curvature_value(handle.handle_curve_variables.Curvature);
			handle.mirrored_handle_mesh->Set_curvature_value(handle.handle_curve_variables.Curvature);

			handle.handle_curve();
			handle.regen(renderer->getDevice(), renderer->getDeviceContext());
		}

		if (handle.handle_curve_variables.x_dimension != handle.handle_mesh->Get_x_dimension_curve())
		{
			handle.handle_mesh->Set_x_dimension_curve(handle.handle_curve_variables.x_dimension);
			handle.mirrored_handle_mesh->Set_x_dimension_curve(handle.handle_curve_variables.x_dimension);
			handle.handle_curve();
			handle.regen(renderer->getDevice(), renderer->getDeviceContext());
		}

		//Statements for Pommel
		if (pommel.pommel != pommel.pommel_mesh->Get_Pommel())
		{
			pommel.pommel_mesh->Set_pommel(pommel.pommel);
			pommel.mirrored_pommel_mesh->Set_pommel(pommel.pommel);
			pommel.regen(renderer->getDevice(), renderer->getDeviceContext());
		}

		if (pommel.pommel)
		{
			if (pommel.pommel_base_variables.height != pommel.pommel_mesh->GetHeight())
			{
				pommel.mirrored_pommel_mesh->SetHeight(pommel.pommel_base_variables.height);
				pommel.pommel_mesh->SetHeight(pommel.pommel_base_variables.height);

				updateHeights();
			}

			if (pommel.pommel_base_variables.thickness != pommel.pommel_mesh->GetThickness())
			{
				pommel.pommel_mesh->SetThickness(pommel.pommel_base_variables.thickness);
				pommel.mirrored_pommel_mesh->SetThickness(pommel.pommel_base_variables.thickness);
				pommel.pommel_curve();
				pommel.regen(renderer->getDevice(), renderer->getDeviceContext());
			}

			if (pommel.pommel_base_variables.width != pommel.pommel_mesh->GetWidth())
			{
				pommel.pommel_mesh->SetWidth(pommel.pommel_base_variables.width);
				pommel.mirrored_pommel_mesh->SetWidth(pommel.pommel_base_variables.width);

				updateWidth();
			}

			if (pommel.pommel_curve_variables_thickness.Curvature != pommel.pommel_mesh->Get_curvature_value())
			{
				pommel.pommel_mesh->Set_curvature_value(pommel.pommel_curve_variables_thickness.Curvature);
				pommel.mirrored_pommel_mesh->Set_curvature_value(pommel.pommel_curve_variables_thickness.Curvature);

				pommel.pommel_curve();
				pommel.regen(renderer->getDevice(), renderer->getDeviceContext());
			}


			if (pommel.pommel_curve_variables_thickness.inverseCurve != pommel.pommel_mesh->Get_inverse_curve())
			{
				pommel.pommel_mesh->Set_inverse_curve(pommel.pommel_curve_variables_thickness.inverseCurve);
				pommel.mirrored_pommel_mesh->Set_inverse_curve(pommel.pommel_curve_variables_thickness.inverseCurve);
				pommel.pommel_curve();
				pommel.regen(renderer->getDevice(), renderer->getDeviceContext());
			}


			if (pommel.pommel_curve_variables_width.Curvature != pommel.pommel_mesh->Get_pommel_point_curvature())
			{
				pommel.pommel_mesh->Set_pommel_point_curvature(pommel.pommel_curve_variables_width.Curvature);
				pommel.mirrored_pommel_mesh->Set_pommel_point_curvature(pommel.pommel_curve_variables_width.Curvature);
				pommel.regen(renderer->getDevice(), renderer->getDeviceContext());
			}

			if (pommel.pommel_curve_variables_width.inverseCurve != pommel.pommel_mesh->Get_inverse_pommel_curve())
			{
				pommel.pommel_mesh->Set_inverse_pommel_curve(pommel.pommel_curve_variables_width.inverseCurve);
				pommel.mirrored_pommel_mesh->Set_inverse_pommel_curve(pommel.pommel_curve_variables_width.inverseCurve);
				pommel.regen(renderer->getDevice(), renderer->getDeviceContext());
			}

			if (pommel.pommel_loft_variables.base_width != pommel.pommel_mesh->Get_length_base())
			{
				pommel.pommel_mesh->Set_length_base(pommel.pommel_loft_variables.base_width);
				pommel.mirrored_pommel_mesh->Set_length_base(pommel.pommel_loft_variables.base_width);
				pommel.regen(renderer->getDevice(), renderer->getDeviceContext());
			}

			if (pommel.pommel_loft_variables.top_width != pommel.pommel_mesh->Get_length_top())
			{
				pommel.pommel_mesh->Set_length_top(pommel.pommel_loft_variables.top_width);
				pommel.mirrored_pommel_mesh->Set_length_top(pommel.pommel_loft_variables.top_width);
				pommel.regen(renderer->getDevice(), renderer->getDeviceContext());
			}

			if (pommel.curve_degree != pommel.pommel_mesh->Get_curve_degree())
			{
				pommel.pommel_mesh->Set_curve_degree(pommel.curve_degree);
				pommel.mirrored_pommel_mesh->Set_curve_degree(pommel.curve_degree);
				pommel.regen(renderer->getDevice(), renderer->getDeviceContext());
			}

			if (pommel.pommel_curve_variables_thickness.x_dimension != pommel.pommel_mesh->Get_x_dimension_curve())
			{
				pommel.pommel_mesh->Set_x_dimension_curve(pommel.pommel_curve_variables_thickness.x_dimension);
				pommel.mirrored_pommel_mesh->Set_x_dimension_curve(pommel.pommel_curve_variables_thickness.x_dimension);

				if (!detailed_UI)
				{
					pommel.pommel_mesh->Set_y_dimension_curve(pommel.pommel_curve_variables_thickness.x_dimension);
					pommel.mirrored_pommel_mesh->Set_y_dimension_curve(pommel.pommel_curve_variables_thickness.x_dimension);
					pommel.pommel_curve_variables_thickness.y_dimension = pommel.pommel_curve_variables_thickness.x_dimension;
				}
			
				pommel.pommel_curve();
				pommel.regen(renderer->getDevice(), renderer->getDeviceContext());
			}

			if (pommel.pommel_curve_variables_thickness.y_dimension != pommel.pommel_mesh->Get_y_dimension_curve())
			{
				pommel.pommel_mesh->Set_y_dimension_curve(pommel.pommel_curve_variables_thickness.y_dimension);
				pommel.mirrored_pommel_mesh->Set_y_dimension_curve(pommel.pommel_curve_variables_thickness.y_dimension);
				pommel.pommel_curve();
				pommel.regen(renderer->getDevice(), renderer->getDeviceContext());
			}
		}

	result = BaseApplication::frame();
	if (!result)
	{
		return false;
	}
	// Render the graphics.
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool App1::render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	worldMatrix = renderer->getWorldMatrix();
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();

	// Send geometry data, set shader parameters, render object with shader
	blade.blade_mesh->sendData(renderer->getDeviceContext());
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"damascus"), light);
	shader->render(renderer->getDeviceContext(), blade.blade_mesh->getIndexCount());

	blade.mirrored_blade_mesh->sendData(renderer->getDeviceContext());
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"damascus"), light);
	shader->render(renderer->getDeviceContext(), blade.mirrored_blade_mesh->getIndexCount());

	guard.guard_mesh->sendData(renderer->getDeviceContext());
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"bronze"), light);
	shader->render(renderer->getDeviceContext(), guard.guard_mesh->getIndexCount());

	guard.mirrored_guard_mesh->sendData(renderer->getDeviceContext());
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"bronze"), light);
	shader->render(renderer->getDeviceContext(), guard.mirrored_guard_mesh->getIndexCount());

	if (guard.fingerGuard)
	{
		guard.guard_finger_mesh->sendData(renderer->getDeviceContext());
		shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"bronze"), light);
		shader->render(renderer->getDeviceContext(), guard.guard_finger_mesh->getIndexCount());

		guard.guard_finger_mirrored_mesh->sendData(renderer->getDeviceContext());
		shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"bronze"), light);
		shader->render(renderer->getDeviceContext(), guard.guard_finger_mirrored_mesh->getIndexCount());
	}

	handle.handle_mesh->sendData(renderer->getDeviceContext());
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"leather"), light);
	shader->render(renderer->getDeviceContext(), handle.handle_mesh->getIndexCount());

	handle.mirrored_handle_mesh->sendData(renderer->getDeviceContext());
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"leather"), light);
	shader->render(renderer->getDeviceContext(), handle.mirrored_handle_mesh->getIndexCount());

	if (pommel.pommel)
	{
		pommel.pommel_mesh->sendData(renderer->getDeviceContext());
		shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"bronze"), light);
		shader->render(renderer->getDeviceContext(), pommel.pommel_mesh->getIndexCount());

		pommel.mirrored_pommel_mesh->sendData(renderer->getDeviceContext());
		shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"bronze"), light);
		shader->render(renderer->getDeviceContext(), pommel.mirrored_pommel_mesh->getIndexCount());
	}
	
	// Render GUI
	gui();

	// Swap the buffers
	renderer->endScene();

	return true;
}

void App1::gui()
{
	// Force turn off unnecessary shader stages.
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->HSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->DSSetShader(NULL, NULL, 0);

	// Build UI
	ImGui::Text("FPS: %.2f", timer->getFPS());
	ImGui::Text("Camera Pos: (%.2f, %.2f, %.2f)", camera->getPosition().x, camera->getPosition().y, camera->getPosition().z);

	ImGui::NewLine();

	ImGui::Checkbox("Wireframe mode", &wireframeToggle);
	ImGui::SliderInt("Weapon Resolution", &terrainResolution, 100, 1000);
	if (ImGui::Button("Regenerate Terrain")) {
		if (terrainResolution != blade.blade_mesh->GetResolution()) {
			blade.blade_mesh->Resize(terrainResolution);
			blade.mirrored_blade_mesh->Resize(terrainResolution);
			guard.guard_mesh->Resize(terrainResolution);
			guard.mirrored_guard_mesh->Resize(terrainResolution);
			guard.guard_finger_mesh->Resize(terrainResolution);
			guard.guard_finger_mirrored_mesh->Resize(terrainResolution);
			handle.handle_mesh->Resize(terrainResolution);
			handle.mirrored_handle_mesh->Resize(terrainResolution);
			pommel.pommel_mesh->Resize(terrainResolution);
			pommel.mirrored_pommel_mesh->Resize(terrainResolution);
		}

		blade.regen(renderer->getDevice(), renderer->getDeviceContext());
		guard.regen(renderer->getDevice(), renderer->getDeviceContext());
		guard.finger_regen(renderer->getDevice(), renderer->getDeviceContext());
		handle.regen(renderer->getDevice(), renderer->getDeviceContext());
		pommel.regen(renderer->getDevice(), renderer->getDeviceContext());
	}

	if (ImGui::Button("Reset Meshes"))
	{
		resetMeshes();
	}

	ImGui::NewLine();

	ImGui::Checkbox("Elaborate UI", &detailed_UI);
	if (detailed_UI)
	{
		ImGui::Checkbox("Realistic Parameters", &realistic_variables);
	}

	if (detailed_UI)
	{
	ImGui::NewLine();
	ImGui::Separator();
	ImGui::Text("Weapon Examples");
	ImGui::NewLine();

		if (ImGui::Button("Weapon Example Katana Min"))
		{
			if (detailed_UI)
			{
				terrainResolution = 128;
				if (terrainResolution != blade.blade_mesh->GetResolution()) {
					blade.blade_mesh->Resize(terrainResolution);
					blade.mirrored_blade_mesh->Resize(terrainResolution);
					guard.guard_mesh->Resize(terrainResolution);
					guard.mirrored_guard_mesh->Resize(terrainResolution);
					guard.guard_finger_mesh->Resize(terrainResolution);
					guard.guard_finger_mirrored_mesh->Resize(terrainResolution);
					handle.handle_mesh->Resize(terrainResolution);
					handle.mirrored_handle_mesh->Resize(terrainResolution);
					pommel.pommel_mesh->Resize(terrainResolution);
					pommel.mirrored_pommel_mesh->Resize(terrainResolution);
				}

				blade.blade_mesh->SetHeight(280);
				blade.mirrored_blade_mesh->SetHeight(280);
				blade.blade_base_variables.height = 280;

				blade.blade_mesh->SetWidth(20);
				blade.mirrored_blade_mesh->SetWidth(20);
				blade.blade_base_variables.width = 20;

				blade.blade_mesh->SetThickness(9);
				blade.mirrored_blade_mesh->SetThickness(9);
				blade.blade_base_variables.thickness = 9;

				guard.guard_mesh->SetHeight(5);
				guard.mirrored_guard_mesh->SetHeight(5);
				guard.guard_base_variables.height = 5;

				guard.guard_mesh->SetWidth(37.5);
				guard.mirrored_guard_mesh->SetWidth(37.5);
				guard.guard_base_variables.width = 37.5;

				guard.guard_mesh->SetThickness(0.5);
				guard.mirrored_guard_mesh->SetThickness(0.5);
				guard.guard_base_variables.thickness = 0.5;

				handle.handle_mesh->SetHeight(77.5);
				handle.mirrored_handle_mesh->SetHeight(77.5);
				handle.handle_base_variables.height = 77.5;

				handle.handle_mesh->SetWidth(20);
				handle.mirrored_handle_mesh->SetWidth(20);
				handle.handle_base_variables.width = 20;

				handle.handle_mesh->SetThickness(2);
				handle.mirrored_handle_mesh->SetThickness(2);
				handle.handle_base_variables.thickness = 2;

				pommel.pommel = false;

				blade.blade_mesh->Set_point_height(0.75);
				blade.mirrored_blade_mesh->Set_point_height(0.75);
				blade.bladeTipHeight = 0.75;

				blade.blade_mesh->Set_side_tip(true);
				blade.mirrored_blade_mesh->Set_side_tip(true);
				blade.sideTip = true;

				blade.blade_mesh->Set_edge_offset(64);
				blade.mirrored_blade_mesh->Set_edge_offset(64);
				blade.edge_offset = 64;

				blade.blade_mesh->Set_bezier_curve(true);
				blade.mirrored_blade_mesh->Set_bezier_curve(true);
				blade.blade_bezier_variables.bezier_curve = true;

				blade.blade_mesh->Set_symmetrical(false);
				blade.mirrored_blade_mesh->Set_symmetrical(false);
				blade.symmetrical = false;

				blade.blade_mesh->bezierX[0] = 0;
				blade.blade_mesh->bezierX[1] = 0;
				blade.blade_mesh->bezierX[2] = -5;
				blade.mirrored_blade_mesh->bezierX[0] = 0;
				blade.mirrored_blade_mesh->bezierX[1] = 0;
				blade.mirrored_blade_mesh->bezierX[2] = -5;
				blade.blade_bezier_variables.bezier[0] = 0;
				blade.blade_bezier_variables.bezier[1] = 0;
				blade.blade_bezier_variables.bezier[2] = -5;

				blade.blade_mesh->Set_fuller_base(0);
				blade.blade_mesh->Set_fuller_depth(0);
				blade.blade_mesh->Set_fuller_height(0);
				blade.blade_mesh->Set_fuller_width(0);
				blade.mirrored_blade_mesh->Set_fuller_base(0);
				blade.mirrored_blade_mesh->Set_fuller_depth(0);
				blade.mirrored_blade_mesh->Set_fuller_height(0);
				blade.mirrored_blade_mesh->Set_fuller_width(0);
				blade.blade_fuller_variables.fuller_base = 0;
				blade.blade_fuller_variables.fuller_depth = 0;
				blade.blade_fuller_variables.fuller_height = 0;
				blade.blade_fuller_variables.fuller_width = 0;

				guard.guard_mesh->Set_bezier_curve(false);
				guard.mirrored_guard_mesh->Set_bezier_curve(false);
				guard.guard_bezier_variables.bezier_curve = false;

				guard.guard_mesh->Set_x_dimension_curve(true);
				guard.guard_mesh->Set_y_dimension_curve(false);
				guard.mirrored_guard_mesh->Set_x_dimension_curve(true);
				guard.mirrored_guard_mesh->Set_y_dimension_curve(false);
				guard.guard_curve_variables.x_dimension = true;
				guard.guard_curve_variables.y_dimension = false;
				guard.guard_mesh->Set_x_dimension_curve(4.5);
				guard.mirrored_guard_mesh->Set_x_dimension_curve(4.5);
				guard.guard_curve_variables.Curvature = 4.5;

				handle.handle_mesh->Set_x_dimension_curve(true);
				handle.mirrored_handle_mesh->Set_x_dimension_curve(true);
				handle.handle_curve_variables.x_dimension = true;

				handle.handle_mesh->Set_curvature_value(1.5);
				handle.mirrored_handle_mesh->Set_curvature_value(1.5);
				handle.handle_curve_variables.Curvature = 1.5;

				handle.handle_mesh->Set_length_base(0);
				handle.mirrored_handle_mesh->Set_length_base(0);
				while (handle.handle_loft_variables.layers.size() > 0)
				{
					handle.handle_loft_variables.layers.pop_back();
					handle.handle_mesh->meshLayers.pop_back();
					handle.mirrored_handle_mesh->meshLayers.pop_back();
				}
				handle.handle_mesh->Set_length_top(0);
				handle.mirrored_handle_mesh->Set_length_top(0);
				handle.handle_loft_variables.base_width = 0;
				handle.handle_loft_variables.top_width = 0;

				pommel.pommel_mesh->Set_x_dimension_curve(true);
				pommel.mirrored_pommel_mesh->Set_x_dimension_curve(true);
				pommel.pommel_curve_variables_thickness.x_dimension = true;

				pommel.pommel_mesh->Set_curvature_value(2);
				pommel.mirrored_pommel_mesh->Set_curvature_value(2);
				pommel.pommel_curve_variables_thickness.Curvature = 2;

				pommel.pommel_mesh->Set_y_dimension_curve(false);
				pommel.mirrored_pommel_mesh->Set_y_dimension_curve(false);
				pommel.pommel_curve_variables_thickness.y_dimension = false;

				pommel.pommel_mesh->Set_pommel_point_curvature(0);
				pommel.mirrored_pommel_mesh->Set_pommel_point_curvature(0);
				pommel.pommel_curve_variables_width.Curvature = 0;

				pommel.pommel_mesh->Set_inverse_curve(false);
				pommel.pommel_mesh->Set_Iscurve(false);
				pommel.pommel_curve_variables_width.isCurve = false;
				pommel.pommel_curve_variables_width.inverseCurve = false;
				pommel.mirrored_pommel_mesh->Set_inverse_curve(false);
				pommel.mirrored_pommel_mesh->Set_Iscurve(false);

				pommel.mirrored_pommel_mesh->Set_length_base(0);
				pommel.mirrored_pommel_mesh->Set_length_top(0);
				pommel.pommel_mesh->Set_length_base(0);
				pommel.pommel_mesh->Set_length_top(0);
				pommel.pommel_loft_variables.top_width = 0;
				pommel.pommel_loft_variables.base_width = 0;

				pommel.pommel_mesh->Set_curve_degree(0);
				pommel.mirrored_pommel_mesh->Set_curve_degree(0);
				pommel.curve_degree = 0;

				blade.blade_edge();
				guard.guard_curve();
				handle.handle_curve();

				updateHeights();
				updateWidth();
			}
		}

		ImGui::SameLine();
		if (ImGui::Button("Weapon Example Katana Max"))
		{
			if (detailed_UI)
			{
				terrainResolution = 750;
				if (terrainResolution != blade.blade_mesh->GetResolution()) {
					blade.blade_mesh->Resize(terrainResolution);
					blade.mirrored_blade_mesh->Resize(terrainResolution);
					guard.guard_mesh->Resize(terrainResolution);
					guard.mirrored_guard_mesh->Resize(terrainResolution);
					guard.guard_finger_mesh->Resize(terrainResolution);
					guard.guard_finger_mirrored_mesh->Resize(terrainResolution);
					handle.handle_mesh->Resize(terrainResolution);
					handle.mirrored_handle_mesh->Resize(terrainResolution);
					pommel.pommel_mesh->Resize(terrainResolution);
					pommel.mirrored_pommel_mesh->Resize(terrainResolution);
				}

				blade.blade_mesh->SetHeight(280);
				blade.mirrored_blade_mesh->SetHeight(280);
				blade.blade_base_variables.height = 280;

				blade.blade_mesh->SetWidth(20);
				blade.mirrored_blade_mesh->SetWidth(20);
				blade.blade_base_variables.width = 20;

				blade.blade_mesh->SetThickness(51.5);
				blade.mirrored_blade_mesh->SetThickness(51.5);
				blade.blade_base_variables.thickness = 51.5;

				guard.guard_mesh->SetHeight(5);
				guard.mirrored_guard_mesh->SetHeight(5);
				guard.guard_base_variables.height = 5;

				guard.guard_mesh->SetWidth(37.5);
				guard.mirrored_guard_mesh->SetWidth(37.5);
				guard.guard_base_variables.width = 37.5;

				guard.guard_mesh->SetThickness(0.5);
				guard.mirrored_guard_mesh->SetThickness(0.5);
				guard.guard_base_variables.thickness = 0.5;

				handle.handle_mesh->SetHeight(77.5);
				handle.mirrored_handle_mesh->SetHeight(77.5);
				handle.handle_base_variables.height = 77.5;

				handle.handle_mesh->SetWidth(20);
				handle.mirrored_handle_mesh->SetWidth(20);
				handle.handle_base_variables.width = 20;

				handle.handle_mesh->SetThickness(2);
				handle.mirrored_handle_mesh->SetThickness(2);
				handle.handle_base_variables.thickness = 2;

				pommel.pommel = false;

				blade.blade_mesh->Set_point_height(0.15);
				blade.mirrored_blade_mesh->Set_point_height(0.15);
				blade.bladeTipHeight = 0.15;

				blade.blade_mesh->Set_side_tip(true);
				blade.mirrored_blade_mesh->Set_side_tip(true);
				blade.sideTip = true;

				blade.blade_mesh->Set_edge_offset(360);
				blade.mirrored_blade_mesh->Set_edge_offset(360);
				blade.edge_offset = 360;

				blade.blade_mesh->Set_bezier_curve(true);
				blade.mirrored_blade_mesh->Set_bezier_curve(true);
				blade.blade_bezier_variables.bezier_curve = true;

				blade.blade_mesh->Set_symmetrical(false);
				blade.mirrored_blade_mesh->Set_symmetrical(false);
				blade.symmetrical = false;

				blade.blade_mesh->bezierX[0] = 0;
				blade.blade_mesh->bezierX[1] = 0.5;
				blade.blade_mesh->bezierX[2] = -5;
				blade.mirrored_blade_mesh->bezierX[0] = 0;
				blade.mirrored_blade_mesh->bezierX[1] = 0.5;
				blade.mirrored_blade_mesh->bezierX[2] = -5;
				blade.blade_bezier_variables.bezier[0] = 0;
				blade.blade_bezier_variables.bezier[1] = 0.5;
				blade.blade_bezier_variables.bezier[2] = -5;

				blade.blade_mesh->Set_fuller_base(0);
				blade.blade_mesh->Set_fuller_depth(0);
				blade.blade_mesh->Set_fuller_height(0);
				blade.blade_mesh->Set_fuller_width(0);
				blade.mirrored_blade_mesh->Set_fuller_base(0);
				blade.mirrored_blade_mesh->Set_fuller_depth(0);
				blade.mirrored_blade_mesh->Set_fuller_height(0);
				blade.mirrored_blade_mesh->Set_fuller_width(0);
				blade.blade_fuller_variables.fuller_base = 0;
				blade.blade_fuller_variables.fuller_depth = 0;
				blade.blade_fuller_variables.fuller_height = 0;
				blade.blade_fuller_variables.fuller_width = 0;

				guard.guard_mesh->Set_bezier_curve(false);
				guard.mirrored_guard_mesh->Set_bezier_curve(false);
				guard.guard_bezier_variables.bezier_curve = false;

				guard.guard_mesh->Set_x_dimension_curve(true);
				guard.guard_mesh->Set_y_dimension_curve(false);
				guard.mirrored_guard_mesh->Set_x_dimension_curve(true);
				guard.mirrored_guard_mesh->Set_y_dimension_curve(false);
				guard.guard_curve_variables.x_dimension = true;
				guard.guard_curve_variables.y_dimension = false;
				guard.guard_mesh->Set_x_dimension_curve(3.5);
				guard.mirrored_guard_mesh->Set_x_dimension_curve(3.5);
				guard.guard_curve_variables.Curvature = 3.5;

				handle.handle_mesh->Set_x_dimension_curve(true);
				handle.mirrored_handle_mesh->Set_x_dimension_curve(true);
				handle.handle_curve_variables.x_dimension = true;

				handle.handle_mesh->Set_curvature_value(2);
				handle.mirrored_handle_mesh->Set_curvature_value(2);
				handle.handle_curve_variables.Curvature = 2;

				handle.handle_mesh->Set_length_base(0);
				handle.mirrored_handle_mesh->Set_length_base(0);
				while (handle.handle_loft_variables.layers.size() > 0)
				{
					handle.handle_loft_variables.layers.pop_back();
					handle.handle_mesh->meshLayers.pop_back();
					handle.mirrored_handle_mesh->meshLayers.pop_back();
				}
				handle.handle_mesh->Set_length_top(0);
				handle.mirrored_handle_mesh->Set_length_top(0);
				handle.handle_loft_variables.base_width = 0;
				handle.handle_loft_variables.top_width = 0;

				pommel.pommel_mesh->Set_x_dimension_curve(true);
				pommel.mirrored_pommel_mesh->Set_x_dimension_curve(true);
				pommel.pommel_curve_variables_thickness.x_dimension = true;

				pommel.pommel_mesh->Set_curvature_value(2);
				pommel.mirrored_pommel_mesh->Set_curvature_value(2);
				pommel.pommel_curve_variables_thickness.Curvature = 2;

				pommel.pommel_mesh->Set_y_dimension_curve(false);
				pommel.mirrored_pommel_mesh->Set_y_dimension_curve(false);
				pommel.pommel_curve_variables_thickness.y_dimension = false;

				pommel.pommel_mesh->Set_pommel_point_curvature(0);
				pommel.mirrored_pommel_mesh->Set_pommel_point_curvature(0);
				pommel.pommel_curve_variables_width.Curvature = 0;

				pommel.pommel_mesh->Set_inverse_curve(false);
				pommel.pommel_mesh->Set_Iscurve(false);
				pommel.pommel_curve_variables_width.isCurve = false;
				pommel.pommel_curve_variables_width.inverseCurve = false;
				pommel.mirrored_pommel_mesh->Set_inverse_curve(false);
				pommel.mirrored_pommel_mesh->Set_Iscurve(false);

				pommel.mirrored_pommel_mesh->Set_length_base(0);
				pommel.mirrored_pommel_mesh->Set_length_top(0);
				pommel.pommel_mesh->Set_length_base(0);
				pommel.pommel_mesh->Set_length_top(0);
				pommel.pommel_loft_variables.top_width = 0;
				pommel.pommel_loft_variables.base_width = 0;

				pommel.pommel_mesh->Set_curve_degree(0);
				pommel.mirrored_pommel_mesh->Set_curve_degree(0);
				pommel.curve_degree = 0;

				blade.blade_edge();
				guard.guard_curve();
				handle.handle_curve();

				updateHeights();
				updateWidth();
			}
		}

		if (ImGui::Button("Weapon Example Gladius Min"))
		{
			terrainResolution = 128;
			if (terrainResolution != blade.blade_mesh->GetResolution()) {
				blade.blade_mesh->Resize(terrainResolution);
				blade.mirrored_blade_mesh->Resize(terrainResolution);
				guard.guard_mesh->Resize(terrainResolution);
				guard.mirrored_guard_mesh->Resize(terrainResolution);
				guard.guard_finger_mesh->Resize(terrainResolution);
				guard.guard_finger_mirrored_mesh->Resize(terrainResolution);
				handle.handle_mesh->Resize(terrainResolution);
				handle.mirrored_handle_mesh->Resize(terrainResolution);
				pommel.pommel_mesh->Resize(terrainResolution);
				pommel.mirrored_pommel_mesh->Resize(terrainResolution);
			}

			if (detailed_UI)
			{
				blade.blade_mesh->SetHeight(250);
				blade.mirrored_blade_mesh->SetHeight(250);
				blade.blade_base_variables.height = 250;

				blade.blade_mesh->SetWidth(50);
				blade.mirrored_blade_mesh->SetWidth(50);
				blade.blade_base_variables.width = 50;

				blade.blade_mesh->SetThickness(12.5);
				blade.mirrored_blade_mesh->SetThickness(12.5);
				blade.blade_base_variables.thickness = 12.5;

				guard.guard_mesh->SetHeight(25);
				guard.mirrored_guard_mesh->SetHeight(25);
				guard.guard_base_variables.height = 25;

				guard.guard_mesh->SetWidth(80);
				guard.mirrored_guard_mesh->SetWidth(80);
				guard.guard_base_variables.width = 80;

				guard.guard_mesh->SetThickness(12.5);
				guard.mirrored_guard_mesh->SetThickness(12.5);
				guard.guard_base_variables.thickness = 12.5;

				handle.handle_mesh->SetHeight(80);
				handle.mirrored_handle_mesh->SetHeight(80);
				handle.handle_base_variables.height = 80;

				handle.handle_mesh->SetWidth(27.5);
				handle.mirrored_handle_mesh->SetWidth(27.5);
				handle.handle_base_variables.width = 27.5;

				handle.handle_mesh->SetThickness(2.75);
				handle.mirrored_handle_mesh->SetThickness(2.75);
				handle.handle_base_variables.thickness = 2.75;

				pommel.pommel_mesh->SetHeight(27.5);
				pommel.mirrored_pommel_mesh->SetHeight(27.5);
				pommel.pommel_base_variables.height = 27.5;

				pommel.pommel_mesh->SetWidth(15);
				pommel.mirrored_pommel_mesh->SetWidth(15);
				pommel.pommel_base_variables.width = 15;

				pommel.pommel_mesh->SetThickness(0.1);
				pommel.mirrored_pommel_mesh->SetThickness(0.1);
				pommel.pommel_base_variables.thickness = 0.1;

				blade.blade_mesh->Set_point_height(5);
				blade.mirrored_blade_mesh->Set_point_height(5);
				blade.bladeTipHeight = 5;

				blade.blade_mesh->Set_side_tip(false);
				blade.mirrored_blade_mesh->Set_side_tip(false);
				blade.sideTip = false;

				blade.blade_mesh->Set_edge_offset(terrainResolution / 2);
				blade.mirrored_blade_mesh->Set_edge_offset(terrainResolution / 2);
				blade.edge_offset = terrainResolution / 2;

				blade.blade_mesh->Set_bezier_curve(false);
				blade.mirrored_blade_mesh->Set_bezier_curve(false);
				blade.blade_bezier_variables.bezier_curve = false;

				blade.blade_mesh->Set_fuller_base(0);
				blade.blade_mesh->Set_fuller_depth(0);
				blade.blade_mesh->Set_fuller_height(0);
				blade.blade_mesh->Set_fuller_width(0);
				blade.mirrored_blade_mesh->Set_fuller_base(0);
				blade.mirrored_blade_mesh->Set_fuller_depth(0);
				blade.mirrored_blade_mesh->Set_fuller_height(0);
				blade.mirrored_blade_mesh->Set_fuller_width(0);
				blade.blade_fuller_variables.fuller_base = 0;
				blade.blade_fuller_variables.fuller_depth = 0;
				blade.blade_fuller_variables.fuller_height = 0;
				blade.blade_fuller_variables.fuller_width = 0;

				guard.guard_mesh->Set_bezier_curve(false);
				guard.mirrored_guard_mesh->Set_bezier_curve(false);
				guard.guard_bezier_variables.bezier_curve = false;

				guard.guard_mesh->bezierX[2] = 0.5;
				guard.mirrored_guard_mesh->bezierX[2] = 0.5;
				guard.guard_bezier_variables.bezier[2] = 0.5;

				guard.guard_mesh->Set_x_dimension_curve(false);
				guard.guard_mesh->Set_y_dimension_curve(false);
				guard.mirrored_guard_mesh->Set_x_dimension_curve(false);
				guard.mirrored_guard_mesh->Set_y_dimension_curve(false);
				guard.guard_curve_variables.x_dimension = false;
				guard.guard_curve_variables.y_dimension = false;
				guard.guard_mesh->Set_x_dimension_curve(0);
				guard.mirrored_guard_mesh->Set_x_dimension_curve(0);
				guard.guard_curve_variables.Curvature = 0;

				handle.handle_mesh->Set_x_dimension_curve(true);
				handle.mirrored_handle_mesh->Set_x_dimension_curve(true);
				handle.handle_curve_variables.x_dimension = true;

				handle.handle_mesh->Set_curvature_value(2);
				handle.mirrored_handle_mesh->Set_curvature_value(2);
				handle.handle_curve_variables.Curvature = 2;

				handle.handle_mesh->Set_length_base(0);
				handle.mirrored_handle_mesh->Set_length_base(0);
				handle.handle_mesh->Set_length_top(0);
				handle.mirrored_handle_mesh->Set_length_top(0);
				handle.handle_loft_variables.base_width = 0;
				handle.handle_loft_variables.top_width = 0;

				while (handle.handle_loft_variables.layers.size() > 0)
				{
					handle.handle_loft_variables.layers.pop_back();
					handle.handle_mesh->meshLayers.pop_back();
					handle.mirrored_handle_mesh->meshLayers.pop_back();
				}

				pommel.pommel = true;

				pommel.pommel_mesh->Set_x_dimension_curve(true);
				pommel.mirrored_pommel_mesh->Set_x_dimension_curve(true);
				pommel.pommel_curve_variables_thickness.x_dimension = true;

				pommel.pommel_mesh->Set_curvature_value(5);
				pommel.mirrored_pommel_mesh->Set_curvature_value(5);
				pommel.pommel_curve_variables_thickness.Curvature = 5;

				pommel.pommel_mesh->Set_y_dimension_curve(true);
				pommel.mirrored_pommel_mesh->Set_y_dimension_curve(true);
				pommel.pommel_curve_variables_thickness.y_dimension = true;

				pommel.pommel_mesh->Set_pommel_point_curvature(2.4);
				pommel.mirrored_pommel_mesh->Set_pommel_point_curvature(2.4);
				pommel.pommel_curve_variables_width.Curvature = 2.4;

				pommel.pommel_mesh->Set_inverse_curve(false);
				pommel.pommel_mesh->Set_Iscurve(false);
				pommel.pommel_curve_variables_width.isCurve = false;
				pommel.pommel_curve_variables_width.inverseCurve = false;
				pommel.mirrored_pommel_mesh->Set_inverse_curve(false);
				pommel.mirrored_pommel_mesh->Set_Iscurve(false);

				pommel.mirrored_pommel_mesh->Set_length_base(3.5);
				pommel.mirrored_pommel_mesh->Set_length_top(5);
				pommel.pommel_mesh->Set_length_base(3.5);
				pommel.pommel_mesh->Set_length_top(5);
				pommel.pommel_loft_variables.top_width = 5;
				pommel.pommel_loft_variables.base_width = 3.5;

				pommel.pommel_mesh->Set_curve_degree(160);
				pommel.mirrored_pommel_mesh->Set_curve_degree(160);
				pommel.curve_degree = 148;

				blade.blade_edge();
				guard.guard_curve();
				handle.handle_curve();
				pommel.pommel_curve();

				updateHeights();
				updateWidth();
			}
		}

		ImGui::SameLine();
		if (ImGui::Button("Weapon Example Gladius Max"))
		{
			if (detailed_UI)
			{
				terrainResolution = 750;
				if (terrainResolution != blade.blade_mesh->GetResolution()) {
					blade.blade_mesh->Resize(terrainResolution);
					blade.mirrored_blade_mesh->Resize(terrainResolution);
					guard.guard_mesh->Resize(terrainResolution);
					guard.mirrored_guard_mesh->Resize(terrainResolution);
					guard.guard_finger_mesh->Resize(terrainResolution);
					guard.guard_finger_mirrored_mesh->Resize(terrainResolution);
					handle.handle_mesh->Resize(terrainResolution);
					handle.mirrored_handle_mesh->Resize(terrainResolution);
					pommel.pommel_mesh->Resize(terrainResolution);
					pommel.mirrored_pommel_mesh->Resize(terrainResolution);
				}

				if (detailed_UI)
				{
					blade.blade_mesh->SetHeight(250);
					blade.mirrored_blade_mesh->SetHeight(250);
					blade.blade_base_variables.height = 250;

					blade.blade_mesh->SetWidth(50);
					blade.mirrored_blade_mesh->SetWidth(50);
					blade.blade_base_variables.width = 50;

					blade.blade_mesh->SetThickness(70);
					blade.mirrored_blade_mesh->SetThickness(70);
					blade.blade_base_variables.thickness = 70;

					guard.guard_mesh->SetHeight(25);
					guard.mirrored_guard_mesh->SetHeight(25);
					guard.guard_base_variables.height = 25;

					guard.guard_mesh->SetWidth(80);
					guard.mirrored_guard_mesh->SetWidth(80);
					guard.guard_base_variables.width = 80;

					guard.guard_mesh->SetThickness(0.5);
					guard.mirrored_guard_mesh->SetThickness(0.5);
					guard.guard_base_variables.thickness = 0.5;

					handle.handle_mesh->SetHeight(80);
					handle.mirrored_handle_mesh->SetHeight(80);
					handle.handle_base_variables.height = 80;

					handle.handle_mesh->SetWidth(22.5);
					handle.mirrored_handle_mesh->SetWidth(22.5);
					handle.handle_base_variables.width = 22.5;

					handle.handle_mesh->SetThickness(2.75);
					handle.mirrored_handle_mesh->SetThickness(2.75);
					handle.handle_base_variables.thickness = 2.75;

					pommel.pommel_mesh->SetHeight(27.5);
					pommel.mirrored_pommel_mesh->SetHeight(27.5);
					pommel.pommel_base_variables.height = 27.5;

					pommel.pommel_mesh->SetWidth(25);
					pommel.mirrored_pommel_mesh->SetWidth(25);
					pommel.pommel_base_variables.width = 25;

					pommel.pommel_mesh->SetThickness(0.1);
					pommel.mirrored_pommel_mesh->SetThickness(0.1);
					pommel.pommel_base_variables.thickness = 0.1;

					blade.blade_mesh->Set_point_height(0.7);
					blade.mirrored_blade_mesh->Set_point_height(0.7);
					blade.bladeTipHeight = 0.7;

					blade.blade_mesh->Set_side_tip(false);
					blade.mirrored_blade_mesh->Set_side_tip(false);
					blade.sideTip = false;

					blade.blade_mesh->Set_edge_offset(350);
					blade.mirrored_blade_mesh->Set_edge_offset(350);
					blade.edge_offset = 350;

					blade.blade_mesh->Set_bezier_curve(false);
					blade.mirrored_blade_mesh->Set_bezier_curve(false);
					blade.blade_bezier_variables.bezier_curve = false;

					blade.blade_mesh->Set_symmetrical(false);
					blade.mirrored_blade_mesh->Set_symmetrical(false);
					blade.symmetrical = false;

					blade.blade_mesh->bezierX[0] = 0;
					blade.blade_mesh->bezierX[1] = 0;
					blade.blade_mesh->bezierX[2] = 0;
					blade.mirrored_blade_mesh->bezierX[0] = 0;
					blade.mirrored_blade_mesh->bezierX[1] =0;
					blade.mirrored_blade_mesh->bezierX[2] = 0;
					blade.blade_bezier_variables.bezier[0] = 0;
					blade.blade_bezier_variables.bezier[1] = 0;
					blade.blade_bezier_variables.bezier[2] = 0;

					blade.blade_mesh->Set_fuller_base(0);
					blade.blade_mesh->Set_fuller_depth(0);
					blade.blade_mesh->Set_fuller_height(0);
					blade.blade_mesh->Set_fuller_width(0);
					blade.mirrored_blade_mesh->Set_fuller_base(0);
					blade.mirrored_blade_mesh->Set_fuller_depth(0);
					blade.mirrored_blade_mesh->Set_fuller_height(0);
					blade.mirrored_blade_mesh->Set_fuller_width(0);
					blade.blade_fuller_variables.fuller_base = 0;
					blade.blade_fuller_variables.fuller_depth = 0;
					blade.blade_fuller_variables.fuller_height = 0;
					blade.blade_fuller_variables.fuller_width = 0;

					guard.guard_mesh->Set_bezier_curve(true);
					guard.mirrored_guard_mesh->Set_bezier_curve(true);
					guard.guard_bezier_variables.bezier_curve = true;

					guard.guard_mesh->bezierX[2] = 0.75;
					guard.mirrored_guard_mesh->bezierX[2] = 0.75;
					guard.guard_bezier_variables.bezier[2] = 0.75;

					guard.guard_mesh->Set_x_dimension_curve(true);
					guard.guard_mesh->Set_y_dimension_curve(false);
					guard.mirrored_guard_mesh->Set_x_dimension_curve(true);
					guard.mirrored_guard_mesh->Set_y_dimension_curve(false);
					guard.guard_curve_variables.x_dimension = true;
					guard.guard_curve_variables.y_dimension = false;
					guard.guard_mesh->Set_x_dimension_curve(2.5);
					guard.mirrored_guard_mesh->Set_x_dimension_curve(2.5);
					guard.guard_curve_variables.Curvature = 2.5;

					handle.handle_mesh->Set_x_dimension_curve(true);
					handle.mirrored_handle_mesh->Set_x_dimension_curve(true);
					handle.handle_curve_variables.x_dimension = true;

					handle.handle_mesh->Set_curvature_value(2);
					handle.mirrored_handle_mesh->Set_curvature_value(2);
					handle.handle_curve_variables.Curvature = 2;

					handle.handle_mesh->Set_length_base(0);
					handle.mirrored_handle_mesh->Set_length_base(0);
					handle.handle_mesh->Set_length_top(0);
					handle.mirrored_handle_mesh->Set_length_top(0);
					handle.handle_loft_variables.base_width = 0;
					handle.handle_loft_variables.top_width = 0;

					while (handle.handle_loft_variables.layers.size() > 31)
					{
						handle.handle_loft_variables.layers.pop_back();
						handle.handle_mesh->meshLayers.pop_back();
						handle.mirrored_handle_mesh->meshLayers.pop_back();
					}

					while (handle.handle_loft_variables.layers.size() < 31)
					{
						handle.handle_loft_variables.layers.push_back(0);
						handle.handle_mesh->meshLayers.push_back(0);
						handle.mirrored_handle_mesh->meshLayers.push_back(0);
					}

					for (int i = 0; i < handle.handle_loft_variables.layers.size(); i++)
					{
						if (i == 0 || i == 30)
						{
							handle.handle_loft_variables.layers.at(i) = 5;
							handle.handle_mesh->meshLayers.at(i) = 5;
							handle.mirrored_handle_mesh->meshLayers.at(i) = 5;
						}
						else if(i == 6 || i == 8 || i == 14 || i == 16 || i == 22 || i == 24)
						{
							handle.handle_loft_variables.layers.at(i) = 3;
							handle.handle_mesh->meshLayers.at(i) = 3;
							handle.mirrored_handle_mesh->meshLayers.at(i) = 3;
						}
						else if (i == 7 || i == 15 || i ==23)
						{
							handle.handle_loft_variables.layers.at(i) = 5;
							handle.handle_mesh->meshLayers.at(i) = 5;
							handle.mirrored_handle_mesh->meshLayers.at(i) = 5;
						}
						else if(i == 3 || i == 27)
						{
							handle.handle_loft_variables.layers.at(i) = -1;
							handle.handle_mesh->meshLayers.at(i) = -1;
							handle.mirrored_handle_mesh->meshLayers.at(i) = -1;
						}
						else if(i == 11 || i == 19)
						{
							handle.handle_loft_variables.layers.at(i) = -2;
							handle.handle_mesh->meshLayers.at(i) = -2;
							handle.mirrored_handle_mesh->meshLayers.at(i) = -2;
						}
						else
						{
							handle.handle_loft_variables.layers.at(i) = 0;
							handle.handle_mesh->meshLayers.at(i) = 0;
							handle.mirrored_handle_mesh->meshLayers.at(i) = 0;
						}
					}

					pommel.pommel_mesh->Set_x_dimension_curve(true);
					pommel.mirrored_pommel_mesh->Set_x_dimension_curve(true);
					pommel.pommel_curve_variables_thickness.x_dimension = true;

					pommel.pommel_mesh->Set_curvature_value(5);
					pommel.mirrored_pommel_mesh->Set_curvature_value(5);
					pommel.pommel_curve_variables_thickness.Curvature = 5;

					pommel.pommel_mesh->Set_y_dimension_curve(true);
					pommel.mirrored_pommel_mesh->Set_y_dimension_curve(true);
					pommel.pommel_curve_variables_thickness.y_dimension = true;

					pommel.pommel_mesh->Set_pommel_point_curvature(2.4);
					pommel.mirrored_pommel_mesh->Set_pommel_point_curvature(2.4);
					pommel.pommel_curve_variables_width.Curvature = 2.4;

					pommel.pommel_mesh->Set_inverse_curve(false);
					pommel.pommel_mesh->Set_Iscurve(false);
					pommel.pommel_curve_variables_width.isCurve = false;
					pommel.pommel_curve_variables_width.inverseCurve = false;
					pommel.mirrored_pommel_mesh->Set_inverse_curve(false);
					pommel.mirrored_pommel_mesh->Set_Iscurve(false);

					pommel.mirrored_pommel_mesh->Set_length_base(3.5);
					pommel.mirrored_pommel_mesh->Set_length_top(5);
					pommel.pommel_mesh->Set_length_base(3.5);
					pommel.pommel_mesh->Set_length_top(5);
					pommel.pommel_loft_variables.top_width = 5;
					pommel.pommel_loft_variables.base_width = 3.5;

					pommel.pommel_mesh->Set_curve_degree(160);
					pommel.mirrored_pommel_mesh->Set_curve_degree(160);
					pommel.curve_degree = 148;

					blade.blade_edge();
					guard.guard_curve();
					handle.handle_curve();
					pommel.pommel_curve();

					updateHeights();
					updateWidth();
				}
			}
		}

		if (ImGui::Button("Weapon Example 3 Min"))
		{
			if (detailed_UI)
			{

			}
		}

		ImGui::SameLine();
		if (ImGui::Button("Weapon Example 3 Max"))
		{
			if (detailed_UI)
			{

			}
		} 

		if (ImGui::Button("Weapon Example 4 Min"))
		{
			if (detailed_UI)
			{

			}
		}  

		ImGui::SameLine();
		if (ImGui::Button("Weapon Example 4 Max"))
		{
			if (detailed_UI)
			{

			}
		}

		if (ImGui::Button("Weapon Example 5 Min"))
		{
			if (detailed_UI)
			{

			}
		}

		ImGui::SameLine();
		if (ImGui::Button("Weapon Example 5 Max"))
		{
			if (detailed_UI)
			{

			}
		}
	}

	if (!detailed_UI)
	{
		ImGui::NewLine();
		ImGui::Separator();
		ImGui::Text("Base Variables");
		ImGui::NewLine();

		ImGui::Checkbox("Two Handed Weapon", &twoHanded);

		ImGui::NewLine();

		ImGui::SliderFloat("Blade Height", &blade.blade_base_variables.height, 25, 300);
		ImGui::SliderFloat("Blade Width", &blade.blade_base_variables.width, 5, 75);
		ImGui::SliderFloat("Blade Thickness", &blade.blade_base_variables.thickness, 0.5, 150);

		ImGui::NewLine();

		ImGui::SliderFloat("Guard Height", &guard.guard_base_variables.height, 5, 20);
		ImGui::SliderFloat("Guard Width", &guard.guard_base_variables.width, 0, 250);
		ImGui::SliderFloat("Guard Thickness", &guard.guard_base_variables.thickness, 0.5, 150);

		ImGui::NewLine();

		ImGui::SliderFloat("Handle Height", &handle.handle_base_variables.height, 25, 175);
		ImGui::SliderFloat("Handle Width", &handle.handle_base_variables.width, 5, 75);
		ImGui::SliderFloat("Handle Thickness", &handle.handle_base_variables.thickness, 0.5, 150);

		ImGui::NewLine();

		ImGui::Checkbox("Pommel", &pommel.pommel);
		if (pommel.pommel)
		{
			ImGui::SliderFloat("Pommel Height", &pommel.pommel_base_variables.height, 1, 150);
			ImGui::SliderFloat("Pommel Width", &pommel.pommel_base_variables.width, 0.1, 100);
			ImGui::SliderFloat("Pommel Thickness", &pommel.pommel_base_variables.thickness, 0.1, 150);
		}

		ImGui::NewLine();
		ImGui::Separator();
		ImGui::Text("Specific Variables");
		ImGui::NewLine();

		if (rightCurve == false)
		{
			ImGui::Checkbox("Blade Curve Left", &leftCurve);
		}
		if (leftCurve == false)
		{
			ImGui::Checkbox("Blade Curve Right", &rightCurve);
		}

		if (rightCurve == false && leftCurve == false)
		{
			ImGui::Checkbox("Blade Fuller", &blade.blade_fuller_variables.fuller);
		}

		ImGui::NewLine();

		ImGui::Checkbox("Guard Curve", &guard.guard_curve_variables.x_dimension);
		if (guard.guard_curve_variables.x_dimension == true)
		{
			ImGui::Checkbox("Inverse Guard Curve", &guard.guard_curve_variables.inverseCurve);
			ImGui::SliderFloat("Guard Curve Value", &guard.guard_curve_variables.Curvature, 0, 5);
		}

		ImGui::NewLine();

		ImGui::Checkbox("Handle Curve", &handle.handle_curve_variables.x_dimension);
		if (handle.handle_curve_variables.x_dimension == true)
		{
			ImGui::SliderFloat("Handle Curve Value", &handle.handle_curve_variables.Curvature, 0, 5);
		}
		ImGui::SliderFloat("Base Handle Width", &handle.handle_loft_variables.base_width, 0, 5);
		ImGui::SliderFloat("Top Handle Width", &handle.handle_loft_variables.top_width, 0, 5);

		ImGui::NewLine();

		if (pommel.pommel)
		{
			ImGui::Checkbox("Pommel Curve", &pommel.pommel_curve_variables_thickness.x_dimension);
			if (pommel.pommel_curve_variables_thickness.x_dimension == true)
			{
				ImGui::Checkbox("Inverse Curve Pommel", &pommel.pommel_curve_variables_thickness.inverseCurve);
				ImGui::SliderFloat("Pommel Curve Value", &pommel.pommel_curve_variables_thickness.Curvature, 0, 5);
			}
		}

		ImGui::NewLine();
		ImGui::Separator();
		ImGui::Text("Finshing Touches");
		ImGui::NewLine();
		if (ImGui::Button("Add damage"))
		{
			for (int i = 0; i < 25; i++)
			{
				blade.blade_mesh->addDamage_scrape(renderer->getDevice(), renderer->getDeviceContext());
				blade.mirrored_blade_mesh->addDamage_scrape(renderer->getDevice(), renderer->getDeviceContext());
				guard.guard_finger_mesh->addDamage_scrape(renderer->getDevice(), renderer->getDeviceContext());
				guard.guard_finger_mirrored_mesh->addDamage_scrape(renderer->getDevice(), renderer->getDeviceContext());
				guard.guard_mesh->addDamage_scrape(renderer->getDevice(), renderer->getDeviceContext());
				guard.mirrored_guard_mesh->addDamage_scrape(renderer->getDevice(), renderer->getDeviceContext());
				handle.handle_mesh->addDamage_scrape(renderer->getDevice(), renderer->getDeviceContext());
				handle.mirrored_handle_mesh->addDamage_scrape(renderer->getDevice(), renderer->getDeviceContext());
				pommel.pommel_mesh->addDamage_scrape(renderer->getDevice(), renderer->getDeviceContext());
				pommel.mirrored_pommel_mesh->addDamage_scrape(renderer->getDevice(), renderer->getDeviceContext());
			}

			blade.blade_mesh->addDamage_dents(renderer->getDevice(), renderer->getDeviceContext(), 0.25, 0.025);
			blade.mirrored_blade_mesh->addDamage_dents(renderer->getDevice(), renderer->getDeviceContext(), 0.25, 0.025);
			guard.guard_finger_mesh->addDamage_dents(renderer->getDevice(), renderer->getDeviceContext(), 0.25, 0.025);
			guard.guard_finger_mirrored_mesh->addDamage_dents(renderer->getDevice(), renderer->getDeviceContext(), 0.25, 0.025);
			guard.guard_mesh->addDamage_dents(renderer->getDevice(), renderer->getDeviceContext(), 0.25, 0.025);
			guard.mirrored_guard_mesh->addDamage_dents(renderer->getDevice(), renderer->getDeviceContext(), 0.25, 0.025);
			handle.handle_mesh->addDamage_dents(renderer->getDevice(), renderer->getDeviceContext(), 0.25, 0.025);
			handle.mirrored_handle_mesh->addDamage_dents(renderer->getDevice(), renderer->getDeviceContext(), 0.25, 0.025);
			pommel.pommel_mesh->addDamage_dents(renderer->getDevice(), renderer->getDeviceContext(), 0.25, 0.025);
			pommel.mirrored_pommel_mesh->addDamage_dents(renderer->getDevice(), renderer->getDeviceContext(), 0.25, 0.025);
		}
	}

	else
	{
		if (!realistic_variables)
		{
			///UNREALISTIC PARAMETERS
			ImGui::NewLine();
			ImGui::Separator();
			ImGui::Text("Base Variables");
			ImGui::NewLine();

			ImGui::SliderFloat("Blade Height", &blade.blade_base_variables.height, 25, 300);
			ImGui::SliderFloat("Blade Width", &blade.blade_base_variables.width, 5, 75);
			ImGui::SliderFloat("Blade Thickness", &blade.blade_base_variables.thickness, 0.5, 150);

			ImGui::NewLine();

			ImGui::SliderFloat("Guard Height", &guard.guard_base_variables.height, 5, 200);
			ImGui::SliderFloat("Guard Width", &guard.guard_base_variables.width, 0, 250);
			ImGui::SliderFloat("Guard Thickness", &guard.guard_base_variables.thickness, 0.5, 150);

			ImGui::NewLine();

			ImGui::SliderFloat("Handle Height", &handle.handle_base_variables.height, 25, 175);
			ImGui::SliderFloat("Handle Width", &handle.handle_base_variables.width, 5, 75);
			ImGui::SliderFloat("Handle Thickness", &handle.handle_base_variables.thickness, 0.5, 150);

			ImGui::NewLine();

			ImGui::Checkbox("Pommel", &pommel.pommel);
			if (pommel.pommel)
			{
				ImGui::SliderFloat("Pommel Height", &pommel.pommel_base_variables.height, 1, 150);
				ImGui::SliderFloat("Pommel Width", &pommel.pommel_base_variables.width, 0.1, 100);
				ImGui::SliderFloat("Pommel Thickness", &pommel.pommel_base_variables.thickness, 0.1, 150);
			}

			ImGui::NewLine();
			ImGui::Checkbox("Finger Guard", &guard.fingerGuard);

			if (guard.fingerGuard == true)
			{

				ImGui::SliderFloat("Finger Guard Height", &guard.guard_finger_base_variables.height, 0, 200);
				ImGui::SliderFloat("Finger Guard Width", &guard.guard_finger_base_variables.width, 0, 250);
				ImGui::SliderFloat("Finger Guard Thickness", &guard.guard_finger_base_variables.thickness, 0.5, 150);
				ImGui::SliderFloat("Finger Guard Offset X", &guard.finger_guard_offset.x, -25, 50);
				ImGui::SliderFloat("Finger Guard Offset Y", &guard.finger_guard_offset.y, -25, 50);
			}
			
			ImGui::NewLine();
			ImGui::Separator();
			ImGui::Text("Blade Specific Variables");
			ImGui::NewLine();

			ImGui::SliderFloat("Tip Height", &blade.bladeTipHeight, 0, 5);
			ImGui::Checkbox("Tip on the Edge", &blade.sideTip);
			ImGui::NewLine();

			ImGui::SliderInt("Blade Edge", &blade.edge_offset, 0, terrainResolution / 2);

			if (blade.edge_offset > 0)
			{
				ImGui::Checkbox("Tip Edge", &blade.tip_edge);
			}

			ImGui::NewLine();

			ImGui::Checkbox("Blade Curve", &blade.blade_bezier_variables.bezier_curve);

			if (blade.blade_bezier_variables.bezier_curve == true)
			{
				ImGui::Checkbox("Symmetrical Blade", &blade.symmetrical);

				if (blade.symmetrical)
				{
					ImGui::Checkbox("OneSided Blade", &blade.blade_oneSided_variables.oneSided);

					if (blade.blade_oneSided_variables.oneSided)
					{
						ImGui::Checkbox("Inverse Curve", &blade.blade_bezier_variables.bezier_inverse);
					}
				}

				for (int x = 0; x < 3; x++)
				{
					ImGui::PushID(x);
					ImGui::SliderFloat("control point X", &blade.blade_bezier_variables.bezier[x], -5, 5);
					ImGui::SameLine();
					ImGui::Text("%i", x);
					ImGui::PopID();
				}
			}

			ImGui::NewLine();

			ImGui::Checkbox("Blade Fuller", &blade.blade_fuller_variables.fuller);
			if (blade.blade_fuller_variables.fuller == true)
			{
				ImGui::SliderInt("Fuller Base", &blade.blade_fuller_variables.fuller_base, 0, blade.blade_mesh->GetHeight() * 0.5);
				ImGui::SliderInt("Fuller Height", &blade.blade_fuller_variables.fuller_height, terrainResolution * 0.51, terrainResolution);
				ImGui::SliderInt("Fuller Width", &blade.blade_fuller_variables.fuller_width, 1, blade.blade_mesh->GetWidth());
				ImGui::SliderFloat("Fuller Depth", &blade.blade_fuller_variables.fuller_depth, 0.1, blade.blade_mesh->GetThickness() * 0.25);
			}

			ImGui::NewLine();
			ImGui::Separator();
			ImGui::Text("Guard Specific Variables");
			ImGui::NewLine();

			ImGui::Checkbox("Guard Bezier Curve", &guard.guard_bezier_variables.bezier_curve);
			if (guard.guard_bezier_variables.bezier_curve == true)
			{
				ImGui::Checkbox("Guard Curve Inverse", &guard.guard_bezier_variables.bezier_inverse);
				ImGui::SliderFloat("control point Y", &guard.guard_bezier_variables.bezier[2], -15, 15);
			}

			ImGui::NewLine();

			ImGui::Checkbox("Guard Curve in X Dimension", &guard.guard_curve_variables.x_dimension);
			ImGui::SameLine();
			ImGui::Checkbox("Guard Curve in Y Dimension", &guard.guard_curve_variables.y_dimension);

			if (guard.guard_curve_variables.x_dimension || guard.guard_curve_variables.y_dimension)
			{
				ImGui::SliderFloat("guard Curve", &guard.guard_curve_variables.Curvature, 0, 5);
				ImGui::Checkbox("Inverse guard Curvature", &guard.guard_curve_variables.inverseCurve);
			}

			if (guard.fingerGuard == true)
			{

			ImGui::NewLine();
			ImGui::Separator();
			ImGui::Text("Finger Guard Specific Variables");
			ImGui::NewLine();
		
				ImGui::Checkbox("Finger Guard Bezier Curve", &guard.guard_finger_bezier_variables.bezier_curve);
				if (guard.guard_finger_bezier_variables.bezier_curve == true)
				{
					for (int gx = 0; gx < 3; gx++)
					{
						ImGui::PushID(gx);
						ImGui::SliderFloat("control point", &guard.guard_finger_bezier_variables.bezier[gx], -15, 15);
						ImGui::SameLine();
						ImGui::Text("%i", gx);
						ImGui::PopID();
					}
				}

				ImGui::NewLine();


				ImGui::SliderFloat("Base Finger Guard Width", &guard.guard_finger_loft_variables.base_width, 0, 5);
				ImGui::SliderFloat("Top Finger Guard Width", &guard.guard_finger_loft_variables.top_width, 0, 5);

				ImGui::NewLine();

				ImGui::Checkbox("Finger Guard Curve in X Dimension", &guard.guard_finger_curve_variables.x_dimension);
				ImGui::SameLine();
				ImGui::Checkbox("Finger Guard Curve in Y Dimension", &guard.guard_finger_curve_variables.y_dimension);

				if (guard.guard_finger_curve_variables.x_dimension || guard.guard_finger_curve_variables.y_dimension)
				{
					ImGui::SliderFloat("Finger Guard Curve", &guard.guard_finger_curve_variables.Curvature, 0, 5);
					ImGui::Checkbox("Inverse Finger Guard Curvature", &guard.guard_finger_curve_variables.inverseCurve);
				}
			}
	
			ImGui::NewLine();
			ImGui::Separator();
			ImGui::Text("Handle Specific Variables");
			ImGui::NewLine();

			ImGui::Checkbox("Handle Curve in x Dimension", &handle.handle_curve_variables.x_dimension);

			if (handle.handle_curve_variables.x_dimension)
			{
				ImGui::SliderFloat("handle Curve", &handle.handle_curve_variables.Curvature, 0, 5);
			}

			ImGui::NewLine();

			ImGui::SliderFloat("Base Handle Width", &handle.handle_loft_variables.base_width, 0, 5);
			ImGui::SliderFloat("Top Handle Width", &handle.handle_loft_variables.top_width, 0, 5);

			ImGui::NewLine();

			ImGui::Text("Layers %i", handle.handle_loft_variables.layers.size());

			if (ImGui::ArrowButton("layers ", 0))
			{
				if (handle.handle_loft_variables.layers.size() > 0)
				{
					handle.handle_loft_variables.layers.pop_back();
					handle.handle_mesh->meshLayers.pop_back();
					handle.mirrored_handle_mesh->meshLayers.pop_back();
				}
			}

			ImGui::SameLine();

			if (ImGui::ArrowButton("LOFT layers", 1))
			{
				if (handle.handle_loft_variables.layers.size() < (terrainResolution / 2))
				{
					handle.handle_loft_variables.layers.push_back(0);
					handle.handle_mesh->meshLayers.push_back(0);
					handle.mirrored_handle_mesh->meshLayers.push_back(0);
				}
			}

			for (int s = 0; s < handle.handle_loft_variables.layers.size(); s++)
			{
				ImGui::PushID(s);
				ImGui::SliderInt("Layer", &handle.handle_loft_variables.layers.at(s), -5, 5);
				ImGui::SameLine();
				ImGui::Text("%i", s);
				ImGui::PopID();
			}

			if (pommel.pommel)
			{
				ImGui::NewLine();
				ImGui::Separator();
				ImGui::Text("Pommel Specific Variables");
				ImGui::NewLine();


				ImGui::Checkbox("Pommel Curve in X Dimension", &pommel.pommel_curve_variables_thickness.x_dimension);
				ImGui::SameLine();
				ImGui::Checkbox("Pommel Curve in Y Dimension", &pommel.pommel_curve_variables_thickness.y_dimension);

				if (pommel.pommel_curve_variables_thickness.y_dimension || pommel.pommel_curve_variables_thickness.x_dimension)
				{
					ImGui::SliderFloat("pommel Curve", &pommel.pommel_curve_variables_thickness.Curvature, 0, 5);
					ImGui::Checkbox("Inverse pommel Curvature", &pommel.pommel_curve_variables_thickness.inverseCurve);
				}

				ImGui::NewLine();

				
				ImGui::Checkbox("Pommel Curve Inverse", &pommel.pommel_curve_variables_width.inverseCurve);
				ImGui::SliderFloat("Pommel base width", &pommel.pommel_loft_variables.base_width, 0, 5);
				ImGui::SliderFloat("Pommel top width", &pommel.pommel_loft_variables.top_width, 0, 5);

				ImGui::NewLine();

				ImGui::SliderFloat("Pommel Curve Amplitude", &pommel.pommel_curve_variables_width.Curvature, 0, 5);
				if (pommel.pommel_curve_variables_width.Curvature > 0)
				{
					ImGui::SliderInt("Curve Degree", &pommel.curve_degree, 0, 720);
				}
			}
			ImGui::NewLine();
			ImGui::Separator();
			ImGui::Text("Finishing Touches");
			ImGui::NewLine();

			ImGui::SliderInt("Damage Iterations", &damage_iterations, 0, 50);
			if (ImGui::Button("Add Blade scrape damage"))
			{
				for (int i = 0; i < damage_iterations; i++)
				{
					blade.blade_mesh->addDamage_scrape(renderer->getDevice(), renderer->getDeviceContext());
					blade.mirrored_blade_mesh->addDamage_scrape(renderer->getDevice(), renderer->getDeviceContext());
				}
			}

			if (ImGui::Button("Add Guard scrape damage"))
			{
				for (int i = 0; i < damage_iterations; i++)
				{
					guard.guard_mesh->addDamage_scrape(renderer->getDevice(), renderer->getDeviceContext());
					guard.mirrored_guard_mesh->addDamage_scrape(renderer->getDevice(), renderer->getDeviceContext());
				}
			}

			if (guard.fingerGuard)
			{
				if (ImGui::Button("Add Finger Guard scrape damage"))
				{
					for (int i = 0; i < damage_iterations; i++)
					{
						guard.guard_finger_mesh->addDamage_scrape(renderer->getDevice(), renderer->getDeviceContext());
						guard.guard_finger_mirrored_mesh->addDamage_scrape(renderer->getDevice(), renderer->getDeviceContext());
					}
				}
			}

			if (ImGui::Button("Add Handle scrape damage"))
			{
				for (int i = 0; i < damage_iterations; i++)
				{
					handle.handle_mesh->addDamage_scrape(renderer->getDevice(), renderer->getDeviceContext());
					handle.mirrored_handle_mesh->addDamage_scrape(renderer->getDevice(), renderer->getDeviceContext());
				}
			}

			if (pommel.pommel)
			{
				if (ImGui::Button("Add Pommel scrape damage"))
				{
					for (int i = 0; i < damage_iterations; i++)
					{
						pommel.pommel_mesh->addDamage_scrape(renderer->getDevice(), renderer->getDeviceContext());
						pommel.mirrored_pommel_mesh->addDamage_scrape(renderer->getDevice(), renderer->getDeviceContext());
					}
				}
			}

			ImGui::NewLine();

			ImGui::SliderFloat("Damage Amplitude", &Amplitude, 0, 1);
			ImGui::SliderFloat("Damage Frequency", &Frequency, 0, 0.1);

			if (ImGui::Button("Add Blade dent damage"))
			{
				blade.blade_mesh->addDamage_dents(renderer->getDevice(), renderer->getDeviceContext(), Amplitude, Frequency);
				blade.mirrored_blade_mesh->addDamage_dents(renderer->getDevice(), renderer->getDeviceContext(), Amplitude, Frequency);
			}

			if (ImGui::Button("Add Guard dent damage"))
			{
				guard.guard_mesh->addDamage_dents(renderer->getDevice(), renderer->getDeviceContext(), Amplitude, Frequency);
				guard.mirrored_guard_mesh->addDamage_dents(renderer->getDevice(), renderer->getDeviceContext(), Amplitude, Frequency);
			}

			if (guard.fingerGuard)
			{
				if (ImGui::Button("Add Finger Guard dent damage"))
				{
					guard.guard_finger_mesh->addDamage_dents(renderer->getDevice(), renderer->getDeviceContext(), Amplitude, Frequency);
					guard.guard_finger_mirrored_mesh->addDamage_dents(renderer->getDevice(), renderer->getDeviceContext(), Amplitude, Frequency);
				}
			}

			if (ImGui::Button("Add Handle dent damage"))
			{
				handle.handle_mesh->addDamage_dents(renderer->getDevice(), renderer->getDeviceContext(), Amplitude, Frequency);
				handle.mirrored_handle_mesh->addDamage_dents(renderer->getDevice(), renderer->getDeviceContext(), Amplitude, Frequency);
			}

			if (pommel.pommel)
			{
				if (ImGui::Button("Add Pommel dent damage"))
				{
					pommel.pommel_mesh->addDamage_dents(renderer->getDevice(), renderer->getDeviceContext(), Amplitude, Frequency);
					pommel.mirrored_pommel_mesh->addDamage_dents(renderer->getDevice(), renderer->getDeviceContext(), Amplitude, Frequency);
				}
			}
			///UNREALISTIC PARAMETERS
		}
		else
		{

		///REALISTIC PARAMETERS
		ImGui::NewLine();
		ImGui::Separator();
		ImGui::Text("Base Variables");
		ImGui::NewLine();

		ImGui::SliderFloat("Blade Height", &blade.blade_base_variables.height, 25, 300);
		ImGui::SliderFloat("Blade Width", &blade.blade_base_variables.width, 5, 75);
		ImGui::SliderFloat("Blade Thickness", &blade.blade_base_variables.thickness, 0.5, 15);

		ImGui::NewLine();

		ImGui::SliderFloat("Guard Height", &guard.guard_base_variables.height, 5, 20);
		ImGui::SliderFloat("Guard Width", &guard.guard_base_variables.width, blade.blade_base_variables.width, 250);
		if (guard.guard_base_variables.width < blade.blade_base_variables.width)
		{
			guard.guard_base_variables.width = blade.blade_base_variables.width;
		}

		ImGui::SliderFloat("Guard Thickness", &guard.guard_base_variables.thickness, blade.blade_base_variables.thickness, 150);
		if (guard.guard_base_variables.thickness < blade.blade_base_variables.thickness)
		{
			guard.guard_base_variables.thickness = blade.blade_base_variables.thickness;
		}

		ImGui::NewLine();

		ImGui::SliderFloat("Handle Height", &handle.handle_base_variables.height, 50, 100);
		ImGui::SliderFloat("Handle Width", &handle.handle_base_variables.width, 20, 30);
		ImGui::SliderFloat("Handle Thickness", &handle.handle_base_variables.thickness, 1, 5);

		ImGui::NewLine();

		ImGui::Checkbox("Pommel", &pommel.pommel);
		if (pommel.pommel)
		{
			ImGui::SliderFloat("Pommel Height", &pommel.pommel_base_variables.height, 10, 20);
			ImGui::SliderFloat("Pommel Width", &pommel.pommel_base_variables.width, handle.handle_base_variables.width, 250);
			ImGui::SliderFloat("Pommel Thickness", &pommel.pommel_base_variables.thickness, handle.handle_base_variables.thickness, 150);
			if (pommel.pommel_base_variables.thickness < handle.handle_base_variables.thickness)
			{
				pommel.pommel_base_variables.thickness = handle.handle_base_variables.thickness;
			}
		}

		ImGui::NewLine();
		ImGui::Checkbox("Finger Guard", &guard.fingerGuard);

		if (guard.fingerGuard == true)
		{
			ImGui::SliderFloat("Finger Guard Width", &guard.guard_finger_base_variables.width, 2.5, 10);
			ImGui::SliderFloat("Finger Guard Thickness", &guard.guard_finger_base_variables.thickness, 0.5, 150);
			ImGui::SliderFloat("Finger Guard Offset X", &guard.finger_guard_offset.x, -25, 50);
			ImGui::SliderFloat("Finger Guard Offset Y", &guard.finger_guard_offset.y, -25, 50);
		}

		ImGui::NewLine();
		ImGui::Separator();
		ImGui::Text("Blade Specific Variables");
		ImGui::NewLine();

		ImGui::SliderFloat("Tip Height", &blade.bladeTipHeight, 0, 5);
		ImGui::Checkbox("Tip on the Edge", &blade.sideTip);

		ImGui::NewLine();

		ImGui::SliderInt("Blade Edge", &blade.edge_offset, 0, terrainResolution / 2);

		if (blade.edge_offset > 0)
		{
			ImGui::Checkbox("Tip Edge", &blade.tip_edge);
		}

		ImGui::NewLine();

		ImGui::Checkbox("Blade Curve", &blade.blade_bezier_variables.bezier_curve);

		if (blade.blade_bezier_variables.bezier_curve == true)
		{
			ImGui::Checkbox("Symmetrical Blade", &blade.symmetrical);

			if (blade.symmetrical)
			{
				ImGui::Checkbox("OneSided Blade", &blade.blade_oneSided_variables.oneSided);

				if (blade.blade_oneSided_variables.oneSided)
				{
					ImGui::Checkbox("Inverse Curve", &blade.blade_bezier_variables.bezier_inverse);
				}
			}

			for (int x = 0; x < 3; x++)
			{
				ImGui::PushID(x);
				ImGui::SliderFloat("control point X", &blade.blade_bezier_variables.bezier[x], -5, 5);
				ImGui::SameLine();
				ImGui::Text("%i", x);
				ImGui::PopID();
			}
		}

		ImGui::NewLine();

		ImGui::Checkbox("Blade Fuller", &blade.blade_fuller_variables.fuller);
		if (blade.blade_fuller_variables.fuller == true)
		{
			ImGui::SliderInt("Fuller Base", &blade.blade_fuller_variables.fuller_base, 0, blade.blade_mesh->GetHeight() * 0.5);
			ImGui::SliderInt("Fuller Height", &blade.blade_fuller_variables.fuller_height, terrainResolution * 0.51, terrainResolution);
			ImGui::SliderInt("Fuller Width", &blade.blade_fuller_variables.fuller_width, 1, blade.blade_mesh->GetWidth());
			ImGui::SliderFloat("Fuller Depth", &blade.blade_fuller_variables.fuller_depth, 0.1, blade.blade_mesh->GetThickness() * 0.25);
		}

		ImGui::NewLine();


		ImGui::NewLine();
		ImGui::Separator();
		ImGui::Text("Guard Specific Variables");
		ImGui::NewLine();

		ImGui::Checkbox("Guard Bezier Curve", &guard.guard_bezier_variables.bezier_curve);
		if (guard.guard_bezier_variables.bezier_curve == true)
		{
			ImGui::Checkbox("Guard Curve Inverse", &guard.guard_bezier_variables.bezier_inverse);
			ImGui::SliderFloat("control point Y", &guard.guard_bezier_variables.bezier[2], -15, 15);
		}

		ImGui::NewLine();

		ImGui::Checkbox("Guard Curve in X Dimension", &guard.guard_curve_variables.x_dimension);
		ImGui::SameLine();
		ImGui::Checkbox("Guard Curve in Y Dimension", &guard.guard_curve_variables.y_dimension);

		if (guard.guard_curve_variables.x_dimension || guard.guard_curve_variables.y_dimension)
		{
			ImGui::SliderFloat("guard Curve", &guard.guard_curve_variables.Curvature, 0, 5);
			ImGui::Checkbox("Inverse guard Curvature", &guard.guard_curve_variables.inverseCurve);
		}

		if (guard.fingerGuard == true)
		{

			ImGui::NewLine();
			ImGui::Separator();
			ImGui::Text("Finger Guard Specific Variables");
			ImGui::NewLine();

			ImGui::Checkbox("Finger Guard Bezier Curve", &guard.guard_finger_bezier_variables.bezier_curve);
			if (guard.guard_finger_bezier_variables.bezier_curve == true)
			{
				for (int gx = 0; gx < 3; gx++)
				{
				ImGui::PushID(gx);
				ImGui::SliderFloat("control point", &guard.guard_finger_bezier_variables.bezier[gx], -15, 15);
				ImGui::SameLine();
				ImGui::Text("%i", gx);
				ImGui::PopID();
				}
			}

			ImGui::NewLine();

			ImGui::SliderFloat("Base Finger Guard Width", &guard.guard_finger_loft_variables.base_width, 0, 5);
			ImGui::SliderFloat("Top Finger Guard Width", &guard.guard_finger_loft_variables.top_width, 0, 5);

			ImGui::NewLine();

			ImGui::Checkbox("Finger Guard Curve in X Dimension", &guard.guard_finger_curve_variables.x_dimension);
			ImGui::SameLine();
			ImGui::Checkbox("Finger Guard Curve in Y Dimension", &guard.guard_finger_curve_variables.y_dimension);

			if (guard.guard_finger_curve_variables.x_dimension || guard.guard_finger_curve_variables.y_dimension)
			{
				ImGui::SliderFloat("Finger Guard Curve", &guard.guard_finger_curve_variables.Curvature, 0, 5);
				ImGui::Checkbox("Inverse Finger Guard Curvature", &guard.guard_finger_curve_variables.inverseCurve);
			}
		}

		ImGui::NewLine();
		ImGui::Separator();
		ImGui::Text("Handle Specific Variables");
		ImGui::NewLine();

		ImGui::Checkbox("Handle Curve in x Dimension", &handle.handle_curve_variables.x_dimension);

		if (handle.handle_curve_variables.x_dimension)
		{
			ImGui::SliderFloat("handle Curve", &handle.handle_curve_variables.Curvature, 0, 5);
		}

		ImGui::NewLine();

		ImGui::SliderFloat("Base Handle Width", &handle.handle_loft_variables.base_width, 0, 5);
		ImGui::SliderFloat("Top Handle Width", &handle.handle_loft_variables.top_width, 0, 5);

		ImGui::NewLine();

		ImGui::Text("Layers %i", handle.handle_loft_variables.layers.size());

		if (ImGui::ArrowButton("layers ", 0))
		{
			if (handle.handle_loft_variables.layers.size() > 0)
			{
				handle.handle_loft_variables.layers.pop_back();
				handle.handle_mesh->meshLayers.pop_back();
				handle.mirrored_handle_mesh->meshLayers.pop_back();
			}
		}

		ImGui::SameLine();

		if (ImGui::ArrowButton("LOFT layers", 1))
		{
			if (handle.handle_loft_variables.layers.size() < (terrainResolution / 2))
			{
				handle.handle_loft_variables.layers.push_back(0);
				handle.handle_mesh->meshLayers.push_back(0);
				handle.mirrored_handle_mesh->meshLayers.push_back(0);
			}
		}

		for (int s = 0; s < handle.handle_loft_variables.layers.size(); s++)
		{
			ImGui::PushID(s);
			ImGui::SliderInt("Layer", &handle.handle_loft_variables.layers.at(s), -5, 5);
			ImGui::SameLine();
			ImGui::Text("%i", s);
			ImGui::PopID();
		}


		ImGui::NewLine();
		ImGui::Separator();
		ImGui::Text("Pommel Specific Variables");
		ImGui::NewLine();


		ImGui::Checkbox("Pommel Curve in X Dimension", &pommel.pommel_curve_variables_thickness.x_dimension);
		ImGui::SameLine();
		ImGui::Checkbox("Pommel Curve in Y Dimension", &pommel.pommel_curve_variables_thickness.y_dimension);

		if (pommel.pommel_curve_variables_thickness.y_dimension || pommel.pommel_curve_variables_thickness.x_dimension)
		{
			ImGui::SliderFloat("pommel Curve", &pommel.pommel_curve_variables_thickness.Curvature, 0, 5);
			ImGui::Checkbox("Inverse pommel Curvature", &pommel.pommel_curve_variables_thickness.inverseCurve);
		}

		ImGui::NewLine();

	
		ImGui::Checkbox("Pommel Curve Inverse", &pommel.pommel_curve_variables_width.inverseCurve);
		ImGui::SliderFloat("Pommel base width", &pommel.pommel_loft_variables.base_width, 0, 5);
		ImGui::SliderFloat("Pommel top width", &pommel.pommel_loft_variables.top_width, 0, 5);

		ImGui::NewLine();

		ImGui::SliderFloat("Pommel Curve Amplitude", &pommel.pommel_curve_variables_width.Curvature, 0, 5);
		if (pommel.pommel_curve_variables_width.Curvature > 0)
		{
			ImGui::SliderInt("Curve Degree", &pommel.curve_degree, 0, 180);
		}

		ImGui::NewLine();
		ImGui::Separator();
		ImGui::Text("Finishing Touches");
		ImGui::NewLine();

		ImGui::SliderInt("Damage Iterations", &damage_iterations, 0, 50);
		if (ImGui::Button("Add Blade scrape damage"))
		{
			for (int i = 0; i < damage_iterations; i++)
			{
				blade.blade_mesh->addDamage_scrape(renderer->getDevice(), renderer->getDeviceContext());
				blade.mirrored_blade_mesh->addDamage_scrape(renderer->getDevice(), renderer->getDeviceContext());
			}
		}

		if (ImGui::Button("Add Guard scrape damage"))
		{
			for (int i = 0; i < damage_iterations; i++)
			{
				guard.guard_mesh->addDamage_scrape(renderer->getDevice(), renderer->getDeviceContext());
				guard.mirrored_guard_mesh->addDamage_scrape(renderer->getDevice(), renderer->getDeviceContext());
			}
		}
	
		if (guard.fingerGuard)
		{
			if (ImGui::Button("Add Finger Guard scrape damage"))
			{
				for (int i = 0; i < damage_iterations; i++)
				{
					guard.guard_finger_mesh->addDamage_scrape(renderer->getDevice(), renderer->getDeviceContext());
					guard.mirrored_guard_mesh->addDamage_scrape(renderer->getDevice(), renderer->getDeviceContext());
				}
			}
		}

		if (ImGui::Button("Add Handle scrape damage"))
		{
			for (int i = 0; i < damage_iterations; i++)
			{
				handle.handle_mesh->addDamage_scrape(renderer->getDevice(), renderer->getDeviceContext());
				handle.mirrored_handle_mesh->addDamage_scrape(renderer->getDevice(), renderer->getDeviceContext());
			}
		}
		
		if (pommel.pommel)
		{
			if (ImGui::Button("Add Pommel scrape damage"))
			{
				for (int i = 0; i < damage_iterations; i++)
				{
					pommel.pommel_mesh->addDamage_scrape(renderer->getDevice(), renderer->getDeviceContext());
					pommel.mirrored_pommel_mesh->addDamage_scrape(renderer->getDevice(), renderer->getDeviceContext());
				}
			}
		}

		ImGui::NewLine();

		ImGui::SliderFloat("Damage Amplitude", &Amplitude, 0, 1);
		ImGui::SliderFloat("Damage Frequency", &Frequency, 0, 0.1);

		if (ImGui::Button("Add Blade dent damage"))
		{
			blade.blade_mesh->addDamage_dents(renderer->getDevice(), renderer->getDeviceContext(), Amplitude, Frequency);
			blade.mirrored_blade_mesh->addDamage_dents(renderer->getDevice(), renderer->getDeviceContext(), Amplitude, Frequency);
		}

		if (guard.fingerGuard)
		{
			if (ImGui::Button("Add Finger Guard dent damage"))
			{
				guard.guard_finger_mesh->addDamage_dents(renderer->getDevice(), renderer->getDeviceContext(), Amplitude, Frequency);
				guard.guard_finger_mirrored_mesh->addDamage_dents(renderer->getDevice(), renderer->getDeviceContext(), Amplitude, Frequency);
			}
		}

		if (ImGui::Button("Add Guard dent damage"))
		{
			guard.guard_mesh->addDamage_dents(renderer->getDevice(), renderer->getDeviceContext(), Amplitude, Frequency);
			guard.mirrored_guard_mesh->addDamage_dents(renderer->getDevice(), renderer->getDeviceContext(), Amplitude, Frequency);
		}

		if (ImGui::Button("Add Handle dent damage"))
		{
			handle.handle_mesh->addDamage_dents(renderer->getDevice(), renderer->getDeviceContext(), Amplitude, Frequency);
			handle.mirrored_handle_mesh->addDamage_dents(renderer->getDevice(), renderer->getDeviceContext(), Amplitude, Frequency);
		}

		if (pommel.pommel)
		{
			if (ImGui::Button("Add Pommel dent damage"))
			{
				pommel.pommel_mesh->addDamage_dents(renderer->getDevice(), renderer->getDeviceContext(), Amplitude, Frequency);
				pommel.mirrored_pommel_mesh->addDamage_dents(renderer->getDevice(), renderer->getDeviceContext(), Amplitude, Frequency);
			}
		}

		////REALISTIC PARAMETERS
		}
	}
	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void App1::initValues()
{
	fingerGuard_offset.x = 0;
	fingerGuard_offset.y = 0;
	pommel.pommel = true;

	light->setAmbientColour(0.25f, 0.25f, 0.25f, 1.0f);
	light->setDiffuseColour(0.75f, 0.75f, 0.75f, 1.0f);
	light->setDirection(1.0f, -0.0f, 0.0f);

	camera->setPosition(-70.0f, 30.0f, 0.0f);
	camera->setRotation(0.0f, 90.0f, 2.5f);

	terrainResolution = blade.blade_mesh->GetResolution();

	/////////////////////////////////////////////

	blade.mirrored_blade_mesh->setMirrored(true);
	blade.blade_base_variables.height = blade.blade_mesh->GetHeight();
	blade.blade_base_variables.thickness = blade.blade_mesh->GetThickness();
	blade.blade_base_variables.width = blade.blade_mesh->GetWidth();
	
	guard.mirrored_guard_mesh->setMirrored(true);
	guard.guard_base_variables.height = guard.guard_mesh->GetHeight();
	guard.guard_base_variables.thickness = guard.guard_mesh->GetThickness();
	guard.guard_base_variables.width = guard.guard_mesh->GetWidth();

	guard.guard_finger_mirrored_mesh->setMirrored(true);
	guard.guard_finger_base_variables.height = guard.guard_mesh->GetHeight();
	guard.guard_finger_base_variables.thickness = guard.guard_mesh->GetThickness();
	guard.guard_finger_base_variables.width = guard.guard_mesh->GetWidth();

	handle.handle_mesh->setMirrored(true);
	handle.handle_base_variables.height = handle.handle_mesh ->GetHeight();
	handle.handle_base_variables.thickness = handle.handle_mesh->GetThickness();
	handle.handle_base_variables.width = handle.handle_mesh->GetWidth();

	pommel.pommel_mesh->setMirrored(true);
	pommel.pommel_base_variables.height = pommel.pommel_mesh->GetHeight();
	pommel.pommel_base_variables.thickness = pommel.pommel_mesh->GetThickness();
	pommel.pommel_base_variables.width = pommel.pommel_mesh->GetWidth();

	/////////////////////////////////////////////

	blade.blade_mesh->Resize(terrainResolution);
	blade.mirrored_blade_mesh->Resize(terrainResolution);
	guard.guard_mesh->Resize(terrainResolution);
	guard.mirrored_guard_mesh->Resize(terrainResolution);
	guard.guard_finger_mesh->Resize(terrainResolution);
	guard.guard_finger_mirrored_mesh->Resize(terrainResolution);
	handle.handle_mesh->Resize(terrainResolution);
	handle.mirrored_handle_mesh->Resize(terrainResolution);
	pommel.pommel_mesh->Resize(terrainResolution);
	pommel.mirrored_pommel_mesh->Resize(terrainResolution);

	updateHeights();
	updateWidth();
	resetMeshes();
}

void App1::updateHeights()
{
	//Regenarate from bottom mesh to top mesh
	//setting position offset from bottom mesh to top mesh

	//pommel first
	pommel.regen(renderer->getDevice(), renderer->getDeviceContext());

	//handle second
	handle.handle_mesh->SetOffsetY(pommel.pommel_mesh->GetDynamicHeight());
	handle.mirrored_handle_mesh->SetOffsetY(pommel.pommel_mesh->GetDynamicHeight());
	handle.regen(renderer->getDevice(), renderer->getDeviceContext());

	guard.guard_finger_mesh->SetOffsetY(pommel.pommel_mesh->GetDynamicHeight() + fingerGuard_offset.y);
	guard.guard_finger_mirrored_mesh->SetOffsetY(pommel.pommel_mesh->GetDynamicHeight() + fingerGuard_offset.y);
	guard.finger_regen(renderer->getDevice(), renderer->getDeviceContext());

	//guard third
	guard.guard_mesh->SetOffsetY(pommel.pommel_mesh->GetDynamicHeight() + handle.handle_mesh->GetDynamicHeight());
	guard.mirrored_guard_mesh->SetOffsetY(pommel.pommel_mesh->GetDynamicHeight() + handle.handle_mesh->GetDynamicHeight());
	guard.regen(renderer->getDevice(), renderer->getDeviceContext());

	//blade last
	blade.blade_mesh->SetOffsetY(pommel.pommel_mesh->GetDynamicHeight() + handle.handle_mesh->GetDynamicHeight() + guard.guard_mesh->GetDynamicHeight());
	blade.mirrored_blade_mesh->SetOffsetY(pommel.pommel_mesh->GetDynamicHeight() + handle.handle_mesh->GetDynamicHeight() + guard.guard_mesh->GetDynamicHeight());
	blade.regen(renderer->getDevice(), renderer->getDeviceContext());
}

void App1::updateWidth()
{
	//regenerating pommel mesh
	
	if (pommel.pommel)
	{
		pommel.regen(renderer->getDevice(), renderer->getDeviceContext());
	
		//regenerating handle mesh
		handle.handle_mesh->SetOffsetZ(pommel.pommel_mesh->GetDynamicWidth() / 2 - handle.handle_mesh->GetDynamicWidth() / 2);
		handle.mirrored_handle_mesh->SetOffsetZ(pommel.pommel_mesh->GetDynamicWidth() / 2 - handle.handle_mesh->GetDynamicWidth() / 2);
		handle.regen(renderer->getDevice(), renderer->getDeviceContext());

		//regenerating guard mesh
		guard.guard_mesh->SetOffsetZ((pommel.pommel_mesh->GetDynamicWidth() / 2 - guard.guard_mesh->GetDynamicWidth() / 2));
		guard.mirrored_guard_mesh->SetOffsetZ(pommel.pommel_mesh->GetDynamicWidth() / 2 - guard.guard_mesh->GetDynamicWidth() / 2);
		guard.regen(renderer->getDevice(), renderer->getDeviceContext());

		guard.guard_finger_mesh->SetOffsetZ(fingerGuard_offset.x);
		guard.guard_finger_mirrored_mesh->SetOffsetZ(fingerGuard_offset.x);
		guard.finger_regen(renderer->getDevice(), renderer->getDeviceContext());

		//regenerating blade mesh
		blade.blade_mesh->SetOffsetZ(pommel.pommel_mesh->GetDynamicWidth() / 2 - blade.blade_mesh->GetDynamicWidth() / 2);
		blade.mirrored_blade_mesh->SetOffsetZ(pommel.pommel_mesh->GetDynamicWidth() / 2 - blade.blade_mesh->GetDynamicWidth() / 2);
		blade.regen(renderer->getDevice(), renderer->getDeviceContext());
	}
	else
	{
		//regenerating handle mesh
		handle.handle_mesh->SetOffsetZ(0);
		handle.mirrored_handle_mesh->SetOffsetZ(0);
		handle.regen(renderer->getDevice(), renderer->getDeviceContext());

		//regenerating guard mesh
		guard.guard_mesh->SetOffsetZ((handle.handle_mesh->GetDynamicWidth() / 2 - guard.guard_mesh->GetDynamicWidth() / 2));
		guard.mirrored_guard_mesh->SetOffsetZ(handle.handle_mesh->GetDynamicWidth() / 2 - guard.guard_mesh->GetDynamicWidth() / 2);
		guard.regen(renderer->getDevice(), renderer->getDeviceContext());

		guard.guard_finger_mesh->SetOffsetZ(fingerGuard_offset.x);
		guard.guard_finger_mirrored_mesh->SetOffsetZ(fingerGuard_offset.x);
		guard.finger_regen(renderer->getDevice(), renderer->getDeviceContext());

		//regenerating blade mesh
		blade.blade_mesh->SetOffsetZ(handle.handle_mesh->GetDynamicWidth() / 2 - blade.blade_mesh->GetDynamicWidth() / 2);
		blade.mirrored_blade_mesh->SetOffsetZ(handle.handle_mesh->GetDynamicWidth() / 2 - blade.blade_mesh->GetDynamicWidth() / 2);
		blade.regen(renderer->getDevice(), renderer->getDeviceContext());
	}
}

void App1::resetMeshes()
{
	if (detailed_UI)
	{
		blade.blade_mesh->SetHeight(50);
		blade.mirrored_blade_mesh->SetHeight(50);
		blade.blade_base_variables.height = 50;

		blade.blade_mesh->SetWidth(20);
		blade.mirrored_blade_mesh->SetWidth(20);
		blade.blade_base_variables.width = 20;

		blade.blade_mesh->SetThickness(2);
		blade.mirrored_blade_mesh->SetThickness(2);
		blade.blade_base_variables.thickness = 2;

		guard.guard_mesh->SetHeight(50);
		guard.mirrored_guard_mesh->SetHeight(50);
		guard.guard_base_variables.height = 50;

		guard.guard_mesh->SetWidth(20);
		guard.mirrored_guard_mesh->SetWidth(20);
		guard.guard_base_variables.width = 20;

		guard.guard_mesh->SetThickness(2);
		guard.mirrored_guard_mesh->SetThickness(2);
		guard.guard_base_variables.thickness=2;

		guard.fingerGuard = false;
		guard.guard_finger_mesh->SetHeight(50);
		guard.guard_finger_mirrored_mesh->SetHeight(50);
		guard.guard_finger_base_variables.height = 50;

		guard.guard_finger_mesh->SetWidth(20);
		guard.guard_finger_mirrored_mesh->SetWidth(20);
		guard.guard_finger_base_variables.width = 20;

		guard.guard_finger_mesh->SetThickness(2);
		guard.guard_finger_mirrored_mesh->SetThickness(2);
		guard.guard_finger_base_variables.thickness = 2;

		guard.finger_guard_offset = XMFLOAT2(0, 0);
		guard.guard_finger_mesh->SetOffsetY(0);
		guard.mirrored_guard_mesh->SetOffsetY(0);
		guard.guard_finger_mesh->SetOffsetZ(0);
		guard.guard_finger_mirrored_mesh->SetOffsetZ(0);


		handle.handle_mesh->SetHeight(50);
		handle.mirrored_handle_mesh->SetHeight(50);
		handle.handle_base_variables.height = 50;

		handle.handle_mesh->SetWidth(20);
		handle.mirrored_handle_mesh->SetWidth(20);
		handle.handle_base_variables.width = 20;

		handle.handle_mesh->SetThickness(2);
		handle.mirrored_handle_mesh->SetThickness(2);
		handle.handle_base_variables.thickness = 2;

		pommel.pommel_mesh->SetHeight(50);
		pommel.mirrored_pommel_mesh->SetHeight(50);
		pommel.pommel_base_variables.height = 50;

		pommel.pommel_mesh->SetWidth(20);
		pommel.mirrored_pommel_mesh->SetWidth(20);
		pommel.pommel_base_variables.width = 20;

		pommel.pommel = true;
		pommel.pommel_mesh->SetThickness(2);
		pommel.mirrored_pommel_mesh->SetThickness(2);
		pommel.pommel_base_variables.thickness = 2;

		blade.blade_mesh->Set_point_height(0);
		blade.mirrored_blade_mesh->Set_point_height(0);
		blade.bladeTipHeight = 0;

		blade.blade_mesh->Set_side_tip(false);
		blade.mirrored_blade_mesh->Set_side_tip(false);
		blade.sideTip = false;

		blade.blade_mesh->Set_edge_offset(0);
		blade.mirrored_blade_mesh->Set_edge_offset(0);
		blade.edge_offset = 0;

		blade.blade_mesh->Set_bezier_curve(false);
		blade.mirrored_blade_mesh->Set_bezier_curve(false);
		blade.blade_bezier_variables.bezier_curve = false;

		blade.blade_mesh->Set_fuller_base(0);
		blade.blade_mesh->Set_fuller_depth(0);
		blade.blade_mesh->Set_fuller_height(0);
		blade.blade_mesh->Set_fuller_width(0);
		blade.mirrored_blade_mesh->Set_fuller_base(0);
		blade.mirrored_blade_mesh->Set_fuller_depth(0);
		blade.mirrored_blade_mesh->Set_fuller_height(0);
		blade.mirrored_blade_mesh->Set_fuller_width(0);
		blade.blade_fuller_variables.fuller = false;
		blade.blade_fuller_variables.fuller_base = 0;
		blade.blade_fuller_variables.fuller_depth = 0;
		blade.blade_fuller_variables.fuller_height = 0;
		blade.blade_fuller_variables.fuller_width = 0;

		blade.blade_mesh->bezierX[0] = 0;
		blade.blade_mesh->bezierX[1] = 0;
		blade.blade_mesh->bezierX[2] = 0;
		blade.mirrored_blade_mesh->bezierX[0] = 0;
		blade.mirrored_blade_mesh->bezierX[1] = 0;
		blade.mirrored_blade_mesh->bezierX[2] = 0;
		blade.blade_bezier_variables.bezier[0] = 0;
		blade.blade_bezier_variables.bezier[1] = 0;
		blade.blade_bezier_variables.bezier[2] = 0;

		guard.fingerGuard = false;

		guard.guard_mesh->Set_bezier_curve(false);
		guard.mirrored_guard_mesh->Set_bezier_curve(false);
		guard.guard_bezier_variables.bezier_curve = false;

		guard.guard_mesh->Set_inverse_curve(false);
		guard.guard_mesh->Set_inverse_curve(false);
		guard.mirrored_guard_mesh->Set_inverse_curve(false);
		guard.mirrored_guard_mesh->Set_inverse_curve(false);
		guard.guard_curve_variables.inverseCurve = false;
		guard.guard_curve_variables.inverseCurve = false;


		guard.guard_mesh->Set_x_dimension_curve(false);
		guard.guard_mesh->Set_y_dimension_curve(false);
		guard.mirrored_guard_mesh->Set_x_dimension_curve(false);
		guard.mirrored_guard_mesh->Set_y_dimension_curve(false);
		guard.guard_curve_variables.x_dimension = false;
		guard.guard_curve_variables.y_dimension = false;

		guard.guard_mesh->Set_x_dimension_curve(0);
		guard.mirrored_guard_mesh->Set_x_dimension_curve(0);
		guard.guard_curve_variables.Curvature = 0;

		handle.handle_mesh->Set_x_dimension_curve(false);
		handle.mirrored_handle_mesh->Set_x_dimension_curve(false);
		handle.handle_curve_variables.x_dimension = false;

		handle.handle_mesh->Set_curvature_value(0);
		handle.mirrored_handle_mesh->Set_curvature_value(0);
		handle.handle_curve_variables.Curvature = 0;

		handle.handle_mesh->Set_length_base(0);
		handle.mirrored_handle_mesh->Set_length_base(0);
		handle.mirrored_handle_mesh->Set_length_top(0);
		handle.handle_loft_variables.base_width = 0;
		handle.handle_loft_variables.top_width = 0;

		while(handle.handle_loft_variables.layers.size() > 0)
		{
			handle.handle_loft_variables.layers.pop_back();
			handle.handle_mesh->meshLayers.pop_back();
			handle.mirrored_handle_mesh->meshLayers.pop_back();
		}
		handle.handle_mesh->Set_length_top(0);
		

		pommel.pommel = true;

		pommel.pommel_mesh->Set_x_dimension_curve(false);
		pommel.mirrored_pommel_mesh->Set_x_dimension_curve(false);
		pommel.pommel_curve_variables_thickness.x_dimension = false;

		pommel.pommel_mesh->Set_y_dimension_curve(false);
		pommel.mirrored_pommel_mesh->Set_y_dimension_curve(false);
		pommel.pommel_curve_variables_thickness.y_dimension = false;

		pommel.pommel_mesh->Set_pommel_point_curvature(0);
		pommel.mirrored_pommel_mesh->Set_pommel_point_curvature(0);
		pommel.pommel_curve_variables_width.Curvature = 0;

		pommel.pommel_mesh->Set_inverse_curve(false);
		pommel.pommel_mesh->Set_Iscurve(false);
		pommel.pommel_curve_variables_width.isCurve = false;
		pommel.pommel_curve_variables_width.inverseCurve = false;
		pommel.mirrored_pommel_mesh->Set_inverse_curve(false);
		pommel.mirrored_pommel_mesh->Set_Iscurve(false);

		pommel.mirrored_pommel_mesh->Set_length_base(0);
		pommel.mirrored_pommel_mesh->Set_length_top(0);
		pommel.pommel_mesh->Set_length_base(0);
		pommel.pommel_mesh->Set_length_top(0);
		pommel.pommel_loft_variables.top_width = 0;
		pommel.pommel_loft_variables.base_width = 0;

		pommel.pommel_mesh->Set_curve_degree(0);
		pommel.mirrored_pommel_mesh->Set_curve_degree(0);
		pommel.curve_degree = 0;
		
	}
	else
	{
	blade.blade_mesh->SetHeight(50);
	blade.mirrored_blade_mesh->SetHeight(50);
	blade.blade_base_variables.height = 50;

	blade.blade_mesh->SetWidth(20);
	blade.mirrored_blade_mesh->SetWidth(20);
	blade.blade_base_variables.width = 20;

	blade.blade_mesh->SetThickness(2);
	blade.mirrored_blade_mesh->SetThickness(2);
	blade.blade_base_variables.thickness = 2;


	guard.guard_mesh->SetHeight(50);
	guard.mirrored_guard_mesh->SetHeight(50);
	guard.guard_base_variables.height = 50;

	guard.guard_mesh->SetWidth(20);
	guard.mirrored_guard_mesh->SetWidth(20);
	guard.guard_base_variables.width = 20;

	guard.guard_mesh->SetThickness(2);
	guard.mirrored_guard_mesh->SetThickness(2);
	guard.guard_base_variables.thickness = 2;

	handle.handle_mesh->SetHeight(50);
	handle.mirrored_handle_mesh->SetHeight(50);
	handle.handle_base_variables.height = 50;

	handle.handle_mesh->SetWidth(20);
	handle.mirrored_handle_mesh->SetWidth(20);
	handle.handle_base_variables.width = 20;

	handle.handle_mesh->SetThickness(2);
	handle.mirrored_handle_mesh->SetThickness(2);
	handle.handle_base_variables.thickness = 2;

	pommel.pommel_mesh->SetHeight(50);
	pommel.mirrored_pommel_mesh->SetHeight(50);
	pommel.pommel_base_variables.height = 50;

	pommel.pommel_mesh->SetWidth(20);
	pommel.mirrored_pommel_mesh->SetWidth(20);
	pommel.pommel_base_variables.width = 20;

	pommel.pommel = true;
	pommel.pommel_mesh->SetThickness(2);
	pommel.mirrored_pommel_mesh->SetThickness(2);
	pommel.pommel_base_variables.thickness = 2;

	leftCurve = false;
	rightCurve = false;
	blade.blade_fuller_variables.fuller = false;

	guard.guard_mesh->Set_x_dimension_curve(false);
	guard.guard_mesh->Set_y_dimension_curve(false);
	guard.mirrored_guard_mesh->Set_x_dimension_curve(false);
	guard.mirrored_guard_mesh->Set_y_dimension_curve(false);
	guard.guard_curve_variables.x_dimension = false;
	guard.guard_curve_variables.y_dimension = false;

	guard.guard_mesh->Set_inverse_curve(false);
	guard.guard_mesh->Set_inverse_curve(false);
	guard.mirrored_guard_mesh->Set_inverse_curve(false);
	guard.mirrored_guard_mesh->Set_inverse_curve(false);
	guard.guard_curve_variables.inverseCurve = false;
	guard.guard_curve_variables.inverseCurve = false;

	guard.guard_mesh->Set_x_dimension_curve(0);
	guard.mirrored_guard_mesh->Set_x_dimension_curve(0);
	guard.guard_curve_variables.Curvature = 0;

	handle.handle_mesh->Set_x_dimension_curve(false);
	handle.mirrored_handle_mesh->Set_x_dimension_curve(false);
	handle.handle_curve_variables.x_dimension = false;

	handle.handle_mesh->Set_curvature_value(0);
	handle.mirrored_handle_mesh->Set_curvature_value(0);
	handle.handle_curve_variables.Curvature = 0;

	handle.handle_mesh->Set_length_base(0);
	handle.mirrored_handle_mesh->Set_length_base(0);
	handle.mirrored_handle_mesh->Set_length_top(0);
	handle.handle_loft_variables.base_width = 0;
	handle.handle_loft_variables.top_width = 0;

	pommel.pommel_mesh->Set_x_dimension_curve(false);
	pommel.mirrored_pommel_mesh->Set_x_dimension_curve(false);
	pommel.pommel_curve_variables_thickness.x_dimension = false;

	pommel.pommel_mesh->Set_y_dimension_curve(false);
	pommel.mirrored_pommel_mesh->Set_y_dimension_curve(false);
	pommel.pommel_curve_variables_thickness.y_dimension = false;

	pommel.pommel_mesh->Set_curvature_value(0);
	pommel.mirrored_pommel_mesh->Set_curvature_value(0);
	pommel.pommel_curve_variables_thickness.Curvature = 0;

	pommel.pommel_mesh->Set_inverse_curve(false);
	pommel.pommel_mesh->Set_inverse_curve(false);
	pommel.mirrored_pommel_mesh->Set_inverse_curve(false);
	pommel.mirrored_pommel_mesh->Set_inverse_curve(false);
	pommel.pommel_curve_variables_thickness.inverseCurve = false;
	pommel.pommel_curve_variables_thickness.inverseCurve = false;

	}

	for (int j = 0; j < (terrainResolution); j++)
	{
		for (int i = 0; i < (terrainResolution); i++)
		{
			blade.blade_mesh->offsetMap[(j * terrainResolution) + i] = -10;
			blade.mirrored_blade_mesh->offsetMap[(j * terrainResolution) + i] = -10;

			guard.guard_mesh->offsetMap[(j * terrainResolution) + i] = -10;
			guard.mirrored_guard_mesh->offsetMap[(j * terrainResolution) + i] = -10;

			handle.handle_mesh->offsetMap[(j * terrainResolution) + i] = -10;
			handle.mirrored_handle_mesh->offsetMap[(j * terrainResolution) + i] = -10;

			pommel.pommel_mesh->offsetMap[(j * terrainResolution) + i] = -10;
			pommel.mirrored_pommel_mesh->offsetMap[(j * terrainResolution) + i] = -10;
		}
	}

	updateHeights();
	updateWidth();
}

