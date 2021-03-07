#include "App1.h"

App1::App1()
{
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in, bool VSYNC, bool FULL_SCREEN)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);

	textureMgr->loadTexture(L"damascus", L"res/Damascus.png");	
	textureMgr->loadTexture(L"bronze", L"res/Bronze.png");
	textureMgr->loadTexture(L"leather", L"res/Leather.png");

	// Create Mesh object and shader object
	blade.blade_mesh = new Blade(renderer->getDevice(), renderer->getDeviceContext());
	blade.mirrored_blade_mesh = new Blade(renderer->getDevice(), renderer->getDeviceContext());

	guard.guard_mesh = new Guard(renderer->getDevice(), renderer->getDeviceContext());
	guard.mirrored_guard_mesh = new Guard(renderer->getDevice(), renderer->getDeviceContext());

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


	if (blade.blade_base_variables.height != blade.blade_mesh->GetHeight())
	{
		blade.mirrored_blade_mesh->SetHeight(blade.blade_base_variables.height);
		blade.blade_mesh->SetHeight(blade.blade_base_variables.height);
		updateHeights();
	}

	if (blade.blade_base_variables.thickness != blade.blade_mesh->GetThickness())
	{
		blade.blade_mesh->SetThickness(blade.blade_base_variables.thickness);
		blade.mirrored_blade_mesh->SetThickness(blade.blade_base_variables.thickness);
		blade.blade_edge();
		blade.regen(renderer->getDevice(), renderer->getDeviceContext());
	}

	if (blade.blade_base_variables.width != blade.blade_mesh->GetWidth())
	{
		blade.blade_mesh->SetWidth(blade.blade_base_variables.width);
		blade.mirrored_blade_mesh->SetWidth(blade.blade_base_variables.width);
		updateWidth();
	}

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
		guard.regen(renderer->getDevice(), renderer->getDeviceContext());
	}

	if (guard.guard_base_variables.width != guard.guard_mesh->GetWidth())
	{
		guard.guard_mesh->SetWidth(guard.guard_base_variables.width);
		guard.mirrored_guard_mesh->SetWidth(guard.guard_base_variables.width);

		updateWidth();
	}

	if (handle.handle_base_variables.height != handle.handle_mesh->GetHeight())
	{
		handle.mirrored_handle_mesh->SetHeight(handle.handle_base_variables.height);
		handle.handle_mesh->SetHeight(handle.handle_base_variables.height);

		updateHeights();
	}

	if (handle.handle_base_variables.thickness != handle.handle_mesh->GetThickness())
	{
		handle.handle_mesh->SetThickness(handle.handle_base_variables.thickness);
		handle.mirrored_handle_mesh->SetThickness(handle.handle_base_variables.thickness);
		handle.regen(renderer->getDevice(), renderer->getDeviceContext());
	}

	if (handle.handle_base_variables.width != handle.handle_mesh->GetWidth())
	{
		handle.handle_mesh->SetWidth(handle.handle_base_variables.width);
		handle.mirrored_handle_mesh->SetWidth(handle.handle_base_variables.width);

		updateWidth();
	}

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
		pommel.regen(renderer->getDevice(), renderer->getDeviceContext());
	}

	if (pommel.pommel_base_variables.width != pommel.pommel_mesh->GetWidth())
	{
		pommel.pommel_mesh->SetWidth(pommel.pommel_base_variables.width);
		pommel.mirrored_pommel_mesh->SetWidth(pommel.pommel_base_variables.width);

		updateWidth();
	}

	if (blade.bladeTipHeight != blade.blade_mesh->pointHeight)
	{
		blade.blade_mesh->pointHeight = blade.bladeTipHeight;
		blade.mirrored_blade_mesh->pointHeight = blade.bladeTipHeight;
		blade.regen(renderer->getDevice(), renderer->getDeviceContext());
	}

	if (blade.blade_bezier_variables.bezier_curve != blade.blade_mesh->bezierCurve)
	{
		blade.blade_mesh->bezierCurve = blade.blade_bezier_variables.bezier_curve;
		blade.mirrored_blade_mesh->bezierCurve = blade.blade_bezier_variables.bezier_curve;
		blade.regen(renderer->getDevice(), renderer->getDeviceContext());
	}

	if (blade.edge_offset != blade.blade_mesh->edge_offset)
	{
		blade.blade_mesh->edge_offset = blade.edge_offset;
		blade.mirrored_blade_mesh->edge_offset = blade.edge_offset;

		blade.blade_edge();
		blade.regen(renderer->getDevice(), renderer->getDeviceContext());
	}

	if (blade.blade_fuller_variables.fuller_base != blade.blade_mesh->fuller_base)
	{
		blade.blade_mesh->fuller_base = blade.blade_fuller_variables.fuller_base;
		blade.mirrored_blade_mesh->fuller_base = blade.blade_fuller_variables.fuller_base;

		blade.blade_fuller();
		blade.regen(renderer->getDevice(), renderer->getDeviceContext());
	}

	if (blade.blade_fuller_variables.fuller_height != blade.blade_mesh->fuller_height)
	{
		blade.blade_mesh->fuller_height = blade.blade_fuller_variables.fuller_height;
		blade.mirrored_blade_mesh->fuller_height = blade.blade_fuller_variables.fuller_height;

		blade.blade_fuller();
		blade.regen(renderer->getDevice(), renderer->getDeviceContext());
	}

	if (blade.blade_fuller_variables.fuller_width != blade.blade_mesh->fuller_width)
	{
		blade.blade_mesh->fuller_width = blade.blade_fuller_variables.fuller_width;
		blade.mirrored_blade_mesh->fuller_width = blade.blade_fuller_variables.fuller_width;

		blade.blade_fuller();
		blade.regen(renderer->getDevice(), renderer->getDeviceContext());
	}

	if (blade.blade_fuller_variables.fuller_depth!= blade.blade_mesh->fuller_depth)
	{
		blade.blade_mesh->fuller_depth = blade.blade_fuller_variables.fuller_depth;
		blade.mirrored_blade_mesh->fuller_depth = blade.blade_fuller_variables.fuller_depth;

		blade.blade_fuller();
		blade.regen(renderer->getDevice(), renderer->getDeviceContext());
	}

	if (guard.guard_curve_variables.Curvature!= guard.guard_mesh->curvature_value)
	{
		guard.guard_mesh->curvature_value = guard.guard_curve_variables.Curvature;
		guard.mirrored_guard_mesh->curvature_value = guard.guard_curve_variables.Curvature;

		guard.guard_curve();
		guard.regen(renderer->getDevice(), renderer->getDeviceContext());
	}

	if (handle.handle_curve_variables.Curvature != handle.handle_mesh->curvature_value)
	{
		handle.handle_mesh->curvature_value = handle.handle_curve_variables.Curvature;
		handle.mirrored_handle_mesh->curvature_value = handle.handle_curve_variables.Curvature;

		handle.handle_curve();
		handle.regen(renderer->getDevice(), renderer->getDeviceContext());
	}

	if (pommel.pommel_curve_variables_thickness.Curvature != pommel.pommel_mesh->curvature_value)
	{
		pommel.pommel_mesh->curvature_value = pommel.pommel_curve_variables_thickness.Curvature;
		pommel.mirrored_pommel_mesh->curvature_value = pommel.pommel_curve_variables_thickness.Curvature;

		pommel.pommel_curve();
		pommel.regen(renderer->getDevice(), renderer->getDeviceContext());
	}

	if (guard.guard_curve_variables.inverseCurve != guard.guard_mesh->inverseCurve)
	{
		guard.guard_mesh->inverseCurve = guard.guard_curve_variables.inverseCurve;
		guard.mirrored_guard_mesh->inverseCurve = guard.guard_curve_variables.inverseCurve;
	}

	if (pommel.pommel_curve_variables_thickness.inverseCurve != pommel.pommel_mesh->inverseCurve)
	{
		pommel.pommel_mesh->inverseCurve = pommel.pommel_curve_variables_thickness.inverseCurve;
		pommel.mirrored_pommel_mesh->inverseCurve = pommel.pommel_curve_variables_thickness.inverseCurve;
	}

	if (handle.loft_variables.base_handle_width != handle.handle_mesh->length_base)
	{
		handle.handle_mesh->length_base = handle.loft_variables.base_handle_width;
		handle.mirrored_handle_mesh->length_base = handle.loft_variables.base_handle_width;
		handle.regen(renderer->getDevice(), renderer->getDeviceContext());
	}

	if (handle.loft_variables.top_handle_width != handle.handle_mesh->length_base)
	{
		handle.handle_mesh->length_top = handle.loft_variables.top_handle_width;
		handle.mirrored_handle_mesh->length_top = handle.loft_variables.top_handle_width;
		handle.regen(renderer->getDevice(), renderer->getDeviceContext());
	}

	for (int s = 0; s < handle.loft_variables.layers.size(); s++)
	{
		if (handle.loft_variables.layers.at(s) != handle.handle_mesh->meshLayers.at(s))
		{
			handle.handle_mesh->meshLayers.at(s) = handle.loft_variables.layers.at(s);
			handle.mirrored_handle_mesh->meshLayers.at(s) = handle.loft_variables.layers.at(s);
			handle.regen(renderer->getDevice(), renderer->getDeviceContext());
		}
	}

	if (pommel.pommel_curve_variables_width.Curvature != pommel.pommel_mesh->pommel_point_curvature)
	{
		pommel.pommel_mesh->pommel_point_curvature = pommel.pommel_curve_variables_width.Curvature;
		pommel.mirrored_pommel_mesh->pommel_point_curvature = pommel.pommel_curve_variables_width.Curvature;
		pommel.regen(renderer->getDevice(), renderer->getDeviceContext());
	}

	if (pommel.pommel_curve_variables_width.inverseCurve != pommel.pommel_mesh->inverse_pommel_curve)
	{
		pommel.pommel_mesh->inverse_pommel_curve = pommel.pommel_curve_variables_width.inverseCurve;
		pommel.mirrored_pommel_mesh->inverse_pommel_curve = pommel.pommel_curve_variables_width.inverseCurve;
		pommel.regen(renderer->getDevice(), renderer->getDeviceContext());
	}

	if (pommel.bottom_width != pommel.pommel_mesh->length_base)
	{
		pommel.pommel_mesh->length_base = pommel.bottom_width;
		pommel.mirrored_pommel_mesh->length_base = pommel.bottom_width;
		pommel.regen(renderer->getDevice(), renderer->getDeviceContext());
	}

	if (pommel.top_width != pommel.pommel_mesh->length_top)
	{
		pommel.pommel_mesh->length_top = pommel.top_width;
		pommel.mirrored_pommel_mesh->length_top = pommel.top_width;
		pommel.regen(renderer->getDevice(), renderer->getDeviceContext());
	}

	if (pommel.curve_degree != pommel.pommel_mesh->curve_degree)
	{
		pommel.pommel_mesh->curve_degree = pommel.curve_degree;
		pommel.mirrored_pommel_mesh->curve_degree = pommel.curve_degree;
		pommel.regen(renderer->getDevice(), renderer->getDeviceContext());
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

	if (guard.guard_bezier_variables.bezier_curve != guard.guard_mesh->bezierCurve)
	{
		guard.guard_mesh->bezierCurve = guard.guard_bezier_variables.bezier_curve;
		guard.mirrored_guard_mesh->bezierCurve = guard.guard_bezier_variables.bezier_curve;
		guard.regen(renderer->getDevice(), renderer->getDeviceContext());
	}

	if (guard.bezier_inverse != guard.guard_mesh->bezierInverse)
	{
		guard.guard_mesh->bezierInverse = guard.bezier_inverse;
		guard.mirrored_guard_mesh->bezierInverse = guard.bezier_inverse;
		guard.regen(renderer->getDevice(), renderer->getDeviceContext());
	}

	if (guard.guard_bezier_variables.bezier[2] != guard.guard_mesh->bezierX[2])
	{
		guard.guard_mesh->bezierX[2] = guard.guard_bezier_variables.bezier[2];
		guard.mirrored_guard_mesh->bezierX[2] = guard.guard_bezier_variables.bezier[2];
		guard.regen(renderer->getDevice(), renderer->getDeviceContext());
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

	handle.handle_mesh->sendData(renderer->getDeviceContext());
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"leather"), light);
	shader->render(renderer->getDeviceContext(), handle.handle_mesh->getIndexCount());

	handle.mirrored_handle_mesh->sendData(renderer->getDeviceContext());
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"leather"), light);
	shader->render(renderer->getDeviceContext(), handle.mirrored_handle_mesh->getIndexCount());

	pommel.pommel_mesh->sendData(renderer->getDeviceContext());
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"bronze"), light);
	shader->render(renderer->getDeviceContext(), pommel.pommel_mesh->getIndexCount());

	pommel.mirrored_pommel_mesh->sendData(renderer->getDeviceContext());
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"bronze"), light);
	shader->render(renderer->getDeviceContext(), pommel.mirrored_pommel_mesh->getIndexCount());
	
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
	ImGui::Checkbox("Wireframe mode", &wireframeToggle);

	ImGui::SliderInt("Weapon Resolution", &terrainResolution, 2, 1024);
	ImGui::SliderFloat("Blade Height", &blade.blade_base_variables.height, 25, 175);
	ImGui::SliderFloat("Blade Width", &blade.blade_base_variables.width, 5, 75);
	ImGui::SliderFloat("Blade Thickness", &blade.blade_base_variables.thickness, 0, 10);

	ImGui::SliderFloat("Guard Height", &guard.guard_base_variables.height, 5, 20);

	ImGui::SliderFloat("Guard Width", &guard.guard_base_variables.width, 0, 250);

	//if (guard.guard_base_variables.width < handle.handle_base_variables.width)
	//{
	//	guard.guard_mesh->SetWidth(handle.handle_base_variables.width);
	//	guard.mirrored_guard_mesh->SetWidth(handle.handle_base_variables.width);
	//	guard.guard_base_variables.width = guard.guard_mesh->GetWidth();
	//	guard.regen(renderer->getDevice(), renderer->getDeviceContext());
	//}
	//if (guard.guard_base_variables.width > (handle.handle_base_variables.width * 2.5))
	//{
	//	guard.guard_mesh->SetWidth(handle.handle_base_variables.width * 2.5);
	//	guard.mirrored_guard_mesh->SetWidth(handle.handle_base_variables.width * 2.5);
	//	guard.guard_base_variables.width = guard.guard_mesh->GetWidth();
	//	guard.regen(renderer->getDevice(), renderer->getDeviceContext());
	//}

	ImGui::SliderFloat("Guard Thickness", &guard.guard_base_variables.thickness, handle.handle_base_variables.thickness, (handle.handle_base_variables.thickness * 2.5));
	ImGui::SliderFloat("Handle Height", &handle.handle_base_variables.height, 25, 175);
	ImGui::SliderFloat("Handle Width", &handle.handle_base_variables.width, 5, 75);
	ImGui::SliderFloat("Handle Thickness", &handle.handle_base_variables.thickness, 0, 10);



	ImGui::SliderFloat("Pommel Height", &pommel.pommel_base_variables.height, (handle.handle_base_variables.width / 1.5), (handle.handle_base_variables.width * 1.5));
	/*if (pommel.pommel_base_variables.height < handle.handle_base_variables.width/1.5)
	{
		pommel.pommel_mesh->SetHeight(handle.handle_base_variables.width/1.5);
		pommel.mirrored_pommel_mesh->SetHeight(handle.handle_base_variables.width/1.5);
		pommel.pommel_base_variables.height = pommel.pommel_mesh->GetHeight();

		updateHeights();
	}
	if (pommel.pommel_base_variables.height > (handle.handle_base_variables.width * 1.5))
	{
		pommel.pommel_mesh->SetHeight((handle.handle_base_variables.width *1.5));
		pommel.mirrored_pommel_mesh->SetHeight((handle.handle_base_variables.width *1.5));
		pommel.pommel_base_variables.height = pommel.pommel_mesh->GetHeight();

		updateHeights();
	}*/

	ImGui::SliderFloat("Pommel Width", &pommel.pommel_base_variables.width, 0.1, (handle.handle_base_variables.width * 1.5));
	//if (pommel.pommel_base_variables.width < (handle.handle_base_variables.width / 1.5))
	//{
	//	pommel.pommel_mesh->SetWidth(handle.handle_base_variables.width / 1.5);
	//	pommel.mirrored_pommel_mesh->SetWidth(handle.handle_base_variables.width / 1.5);
	//	pommel.pommel_base_variables.width = pommel.pommel_mesh->GetWidth();

	//	updateWidth();
	//}

	//if (pommel.pommel_base_variables.height > (handle.handle_base_variables.width * 1.5))
	//{
	//	pommel.pommel_mesh->SetWidth((handle.handle_base_variables.width * 1.5));
	//	pommel.mirrored_pommel_mesh->SetWidth((handle.handle_base_variables.width * 1.5));
	//	pommel.pommel_base_variables.width = pommel.pommel_mesh->GetWidth();

	//	updateWidth();

	//}

	ImGui::SliderFloat("Pommel Thickness", &pommel.pommel_base_variables.thickness, (handle.handle_base_variables.thickness / 2), (handle.handle_base_variables.thickness * 1.5));

	if (ImGui::Button("Regenerate Terrain")) {
		if (terrainResolution != blade.blade_mesh->GetResolution()) {
			blade.blade_mesh->Resize(terrainResolution);
			blade.mirrored_blade_mesh->Resize(terrainResolution);
			guard.guard_mesh->Resize(terrainResolution);
			guard.mirrored_guard_mesh->Resize(terrainResolution);
			handle.handle_mesh->Resize(terrainResolution);
			handle.mirrored_handle_mesh->Resize(terrainResolution);
			pommel.pommel_mesh->Resize(terrainResolution);
			pommel.mirrored_pommel_mesh->Resize(terrainResolution);
		}

		blade.regen(renderer->getDevice(), renderer->getDeviceContext());
		guard.regen(renderer->getDevice(), renderer->getDeviceContext());
		handle.regen(renderer->getDevice(), renderer->getDeviceContext());
		pommel.regen(renderer->getDevice(), renderer->getDeviceContext());
	}

	ImGui::SliderFloat("Tip Height", &blade.bladeTipHeight, 0, 5);

	ImGui::Checkbox("Blade Curve", &blade.blade_bezier_variables.bezier_curve);

	ImGui::SliderInt("Blade Edge", &blade.edge_offset, 1, terrainResolution / 2);

	ImGui::SliderInt("Fuller Base", &blade.blade_fuller_variables.fuller_base, 0, blade.blade_mesh->GetHeight() * 0.5);
	ImGui::SliderInt("Fuller Height", &blade.blade_fuller_variables.fuller_height, terrainResolution * 0.51, terrainResolution * 0.8);
	ImGui::SliderInt("Fuller Width", &blade.blade_fuller_variables.fuller_width, 1, blade.blade_mesh->GetWidth());
	ImGui::SliderFloat("Fuller Depth", &blade.blade_fuller_variables.fuller_depth, 0.1, blade.blade_mesh->GetThickness() * 0.25);

	ImGui::SliderFloat("guard Curve", &guard.guard_curve_variables.Curvature, 0, 5);
	ImGui::Checkbox("Inverse guard Curvature", &guard.guard_curve_variables.inverseCurve);

	ImGui::SliderFloat("handle Curve", &handle.handle_curve_variables.Curvature, 0, 5);

	ImGui::SliderFloat("pommel Curve", &pommel.pommel_curve_variables_thickness.Curvature, 0, 5);
	ImGui::Checkbox("Inverse pommel Curvature", &pommel.pommel_curve_variables_thickness.inverseCurve);

	ImGui::SliderInt("Base Handle Width", &handle.loft_variables.base_handle_width, 0, 5);
	ImGui::SliderInt("Top Handle Width", &handle.loft_variables.top_handle_width, 0, 5);


	ImGui::Text("Layers %i", handle.loft_variables.layers.size());

	if (ImGui::ArrowButton("layers ", 0))
	{
		if (handle.loft_variables.layers.size() > 0)
		{
			handle.loft_variables.layers.pop_back();
			handle.handle_mesh->meshLayers.pop_back();
			handle.mirrored_handle_mesh->meshLayers.pop_back();
		}
	}
	
	ImGui::SameLine();

	if (ImGui::ArrowButton("layers", 1))
	{
		if (handle.loft_variables.layers.size() < (terrainResolution - 1))
		{
			handle.loft_variables.layers.push_back(0);
			handle.handle_mesh->meshLayers.push_back(0);
			handle.mirrored_handle_mesh->meshLayers.push_back(0);
		}
	}

	for (int s = 0; s < handle.loft_variables.layers.size(); s++)
	{
		ImGui::PushID(s);
		ImGui::SliderInt("Layer", &handle.loft_variables.layers.at(s), 0, 5);
		ImGui::SameLine();
		ImGui::Text("%i", s);
		ImGui::PopID();
	}

	ImGui::SliderFloat("Pommel Point Curve", &pommel.pommel_curve_variables_width.Curvature, 0, 5);
	ImGui::Checkbox("Pommel Curve Inverse", &pommel.pommel_curve_variables_width.inverseCurve);
	ImGui::SliderFloat("Pommel base width", &pommel.bottom_width, 0, 2 );
	ImGui::SliderFloat("Pommel top width", &pommel.top_width, 0,2 );
	ImGui::SliderInt("Curve Degree", &pommel.curve_degree, 0, 720);

	for (int x = 0; x < 3; x++)
	{
		ImGui::PushID(x);
		ImGui::SliderFloat("control point X", &blade.blade_bezier_variables.bezier[x], -5, 5);
		ImGui::SameLine();
		ImGui::Text("%i", x);
		ImGui::PopID();
	}


	ImGui::Checkbox("Guard Curve", &guard.guard_bezier_variables.bezier_curve);
	ImGui::Checkbox("Guard Curve Inverse", &guard.bezier_inverse);

	ImGui::SliderFloat("control point Y", &guard.guard_bezier_variables.bezier[2], -5, 5);
	
	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void App1::initValues()
{
	light->setAmbientColour(0.25f, 0.25f, 0.25f, 1.0f);
	light->setDiffuseColour(0.75f, 0.75f, 0.75f, 1.0f);
	light->setDirection(1.0f, -0.0f, 0.0f);

	camera->setPosition(-70.0f, 30.0f, 0.0f);
	camera->setRotation(0.0f, 90.0f, 2.5f);

	terrainResolution = blade.blade_mesh->GetResolution();

	blade.mirrored_blade_mesh->setMirrored(true);
	blade.blade_base_variables.height = blade.blade_mesh->GetHeight();
	blade.blade_base_variables.thickness = blade.blade_mesh->GetThickness();
	blade.blade_base_variables.width = blade.blade_mesh->GetWidth();
	
	guard.mirrored_guard_mesh->setMirrored(true);
	guard.guard_base_variables.height = guard.guard_mesh->GetHeight();
	guard.guard_base_variables.thickness = guard.guard_mesh->GetThickness();
	guard.guard_base_variables.width = guard.guard_mesh->GetWidth();

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
	handle.handle_mesh->Resize(terrainResolution);
	handle.mirrored_handle_mesh->Resize(terrainResolution);
	pommel.pommel_mesh->Resize(terrainResolution);
	pommel.mirrored_pommel_mesh->Resize(terrainResolution);

	updateHeights();
	updateWidth();
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
	pommel.regen(renderer->getDevice(), renderer->getDeviceContext());

	//regenerating handle mesh
	handle.handle_mesh->SetOffsetZ(pommel.pommel_mesh->GetDynamicWidth()/2 - handle.handle_mesh->GetDynamicWidth()/2);
	handle.mirrored_handle_mesh->SetOffsetZ(pommel.pommel_mesh->GetDynamicWidth() / 2 - handle.handle_mesh->GetDynamicWidth() / 2);
	handle.regen(renderer->getDevice(), renderer->getDeviceContext());

	//regenerating guard mesh
	guard.guard_mesh->SetOffsetZ((pommel.pommel_mesh->GetDynamicWidth() / 2 - guard.guard_mesh->GetDynamicWidth()/2));
	guard.mirrored_guard_mesh->SetOffsetZ(pommel.pommel_mesh->GetDynamicWidth() / 2 - guard.guard_mesh->GetDynamicWidth() / 2);
	guard.regen(renderer->getDevice(), renderer->getDeviceContext());

	//regenerating blade mesh
	blade.blade_mesh->SetOffsetZ(pommel.pommel_mesh->GetDynamicWidth()/2 - blade.blade_mesh->GetDynamicWidth()/2);
	blade.mirrored_blade_mesh->SetOffsetZ(pommel.pommel_mesh->GetDynamicWidth() / 2 - blade.blade_mesh->GetDynamicWidth() / 2);
	blade.regen(renderer->getDevice(), renderer->getDeviceContext());
}


