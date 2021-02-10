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
	blade.normal_blade = new Blade(renderer->getDevice(), renderer->getDeviceContext());
	blade.mirrored_blade = new Blade(renderer->getDevice(), renderer->getDeviceContext());

	guard.normal_guard = new Guard(renderer->getDevice(), renderer->getDeviceContext());
	guard.mirrored_guard = new Guard(renderer->getDevice(), renderer->getDeviceContext());

	handle.normal_handle = new Handle(renderer->getDevice(), renderer->getDeviceContext());
	handle.mirrored_handle = new Handle(renderer->getDevice(), renderer->getDeviceContext());

	pommel.normal_pommel = new Pommel(renderer->getDevice(), renderer->getDeviceContext());
	pommel.mirrored_pommel = new Pommel(renderer->getDevice(), renderer->getDeviceContext());

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


	if (blade.bladeHeight != blade.normal_blade->GetHeight())
	{
		blade.mirrored_blade->SetHeight(blade.bladeHeight);
		blade.normal_blade->SetHeight(blade.bladeHeight);
		updateHeights();
	}

	if (blade.bladeThickness != blade.normal_blade->GetThickness())
	{
		blade.normal_blade->SetThickness(blade.bladeThickness);
		blade.mirrored_blade->SetThickness(blade.bladeThickness);
		blade.blade_edge();
		blade.regen(renderer->getDevice(), renderer->getDeviceContext());
	}

	if (blade.bladeWidth != blade.normal_blade->GetWidth())
	{
		blade.normal_blade->SetWidth(blade.bladeWidth);
		blade.mirrored_blade->SetWidth(blade.bladeWidth);
		updateWidth();
	}

	if (guard.guardHeight != guard.normal_guard->GetHeight())
	{
		guard.mirrored_guard->SetHeight(guard.guardHeight);
		guard.normal_guard->SetHeight(guard.guardHeight);

		updateHeights();
	}

	if (guard.guardThickness != guard.normal_guard->GetThickness())
	{
		guard.normal_guard->SetThickness(guard.guardThickness);
		guard.mirrored_guard->SetThickness(guard.guardThickness);
		guard.regen(renderer->getDevice(), renderer->getDeviceContext());
	}

	if (guard.guardWidth != guard.normal_guard->GetWidth())
	{
		guard.normal_guard->SetWidth(guard.guardWidth);
		guard.mirrored_guard->SetWidth(guard.guardWidth);

		updateWidth();
	}

	if (handle.handleHeight != handle.normal_handle->GetHeight())
	{
		handle.mirrored_handle->SetHeight(handle.handleHeight);
		handle.normal_handle->SetHeight(handle.handleHeight);

		updateHeights();
	}

	if (handle.handleThickness != handle.normal_handle->GetThickness())
	{
		handle.normal_handle->SetThickness(handle.handleThickness);
		handle.mirrored_handle->SetThickness(handle.handleThickness);
		handle.regen(renderer->getDevice(), renderer->getDeviceContext());
	}

	if (handle.handleWidth != handle.normal_handle->GetWidth())
	{
		handle.normal_handle->SetWidth(handle.handleWidth);
		handle.mirrored_handle->SetWidth(handle.handleWidth);

		updateWidth();
	}

	if (pommel.pommelHeight != pommel.normal_pommel->GetHeight())
	{
		pommel.mirrored_pommel->SetHeight(pommel.pommelHeight);
		pommel.normal_pommel->SetHeight(pommel.pommelHeight);

		updateHeights();
	}

	if (pommel.pommelThickness != pommel.normal_pommel->GetThickness())
	{
		pommel.normal_pommel->SetThickness(pommel.pommelThickness);
		pommel.mirrored_pommel->SetThickness(pommel.pommelThickness);
		pommel.regen(renderer->getDevice(), renderer->getDeviceContext());
	}

	if (pommel.pommelWidth != pommel.normal_pommel->GetWidth())
	{
		pommel.normal_pommel->SetWidth(pommel.pommelWidth);
		pommel.mirrored_pommel->SetWidth(pommel.pommelWidth);

		updateWidth();
	}

	if (blade.bladeTipHeight != blade.normal_blade->pointHeight)
	{
		blade.normal_blade->pointHeight = blade.bladeTipHeight;
		blade.mirrored_blade->pointHeight = blade.bladeTipHeight;
		blade.regen(renderer->getDevice(), renderer->getDeviceContext());
	}

	if (blade.isCurve != blade.normal_blade->isCurve)
	{
		blade.normal_blade->isCurve = blade.isCurve;
		blade.mirrored_blade->isCurve = blade.isCurve;
	}

	if (blade.isCurve)
	{
		if (blade.Curvature != blade.normal_blade->curvature_value)
		{
			blade.normal_blade->curvature_value = blade.Curvature;
			blade.mirrored_blade->curvature_value = blade.Curvature;
			blade.regen(renderer->getDevice(), renderer->getDeviceContext());
		}

		if (blade.inverseCurve != blade.normal_blade->inverseCurve)
		{
			blade.normal_blade->inverseCurve = blade.inverseCurve;
			blade.mirrored_blade->inverseCurve = blade.inverseCurve;
		}
	}

	if (blade.edge_offset != blade.normal_blade->edge_offset)
	{
		blade.normal_blade->edge_offset = blade.edge_offset;
		blade.mirrored_blade->edge_offset = blade.edge_offset;

		blade.blade_edge();
		blade.regen(renderer->getDevice(), renderer->getDeviceContext());
	}

	if (blade.fuller_base != blade.normal_blade->fuller_base)
	{
		blade.normal_blade->fuller_base = blade.fuller_base;
		blade.mirrored_blade->fuller_base = blade.fuller_base;

		blade.blade_fuller();
		blade.regen(renderer->getDevice(), renderer->getDeviceContext());
	}

	if (blade.fuller_height != blade.normal_blade->fuller_height)
	{
		blade.normal_blade->fuller_height = blade.fuller_height;
		blade.mirrored_blade->fuller_height = blade.fuller_height;

		blade.blade_fuller();
		blade.regen(renderer->getDevice(), renderer->getDeviceContext());
	}

	if (blade.fuller_width != blade.normal_blade->fuller_width)
	{
		blade.normal_blade->fuller_width = blade.fuller_width;
		blade.mirrored_blade->fuller_width = blade.fuller_width;

		blade.blade_fuller();
		blade.regen(renderer->getDevice(), renderer->getDeviceContext());
	}

	if (blade.fuller_depth != blade.normal_blade->fuller_depth)
	{
		blade.normal_blade->fuller_depth = blade.fuller_depth;
		blade.mirrored_blade->fuller_depth = blade.fuller_depth;

		blade.blade_fuller();
		blade.regen(renderer->getDevice(), renderer->getDeviceContext());
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
	blade.normal_blade->sendData(renderer->getDeviceContext());
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"damascus"), light);
	shader->render(renderer->getDeviceContext(), blade.normal_blade->getIndexCount());

	blade.mirrored_blade->sendData(renderer->getDeviceContext());
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"damascus"), light);
	shader->render(renderer->getDeviceContext(), blade.mirrored_blade->getIndexCount());

	guard.normal_guard->sendData(renderer->getDeviceContext());
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"bronze"), light);
	shader->render(renderer->getDeviceContext(), guard.normal_guard->getIndexCount());

	guard.mirrored_guard->sendData(renderer->getDeviceContext());
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"bronze"), light);
	shader->render(renderer->getDeviceContext(), guard.mirrored_guard->getIndexCount());

	handle.normal_handle->sendData(renderer->getDeviceContext());
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"leather"), light);
	shader->render(renderer->getDeviceContext(), handle.normal_handle->getIndexCount());

	handle.mirrored_handle->sendData(renderer->getDeviceContext());
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"leather"), light);
	shader->render(renderer->getDeviceContext(), handle.mirrored_handle->getIndexCount());

	pommel.normal_pommel->sendData(renderer->getDeviceContext());
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"bronze"), light);
	shader->render(renderer->getDeviceContext(), pommel.normal_pommel->getIndexCount());

	pommel.mirrored_pommel->sendData(renderer->getDeviceContext());
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"bronze"), light);
	shader->render(renderer->getDeviceContext(), pommel.mirrored_pommel->getIndexCount());
	
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
	ImGui::Text( "Camera Pos: (%.2f, %.2f, %.2f)", camera->getPosition().x, camera->getPosition().y, camera->getPosition().z );
	ImGui::Checkbox("Wireframe mode", &wireframeToggle);

	ImGui::SliderInt( "Weapon Resolution", &terrainResolution, 2, 1024 );
	ImGui::SliderFloat("Blade Height", &blade.bladeHeight, 25, 175);
	ImGui::SliderFloat("Blade Width", &blade.bladeWidth, 5, 75);
	ImGui::SliderFloat("Blade Thickness", &blade.bladeThickness, 0, 10);

	ImGui::SliderFloat("Guard Height", &guard.guardHeight, 5, 20);

	ImGui::SliderFloat("Guard Width", &guard.guardWidth, handle.handleWidth, (handle.handleWidth*2.5));

	//if (guard.guardWidth < handle.handleWidth)
	//{
	//	guard.normal_guard->SetWidth(handle.handleWidth);
	//	guard.mirrored_guard->SetWidth(handle.handleWidth);
	//	guard.guardWidth = guard.normal_guard->GetWidth();
	//	guard.regen(renderer->getDevice(), renderer->getDeviceContext());
	//}
	//if (guard.guardWidth > (handle.handleWidth * 2.5))
	//{
	//	guard.normal_guard->SetWidth(handle.handleWidth * 2.5);
	//	guard.mirrored_guard->SetWidth(handle.handleWidth * 2.5);
	//	guard.guardWidth = guard.normal_guard->GetWidth();
	//	guard.regen(renderer->getDevice(), renderer->getDeviceContext());
	//}

	ImGui::SliderFloat("Guard Thickness", &guard.guardThickness, handle.handleThickness,(handle.handleThickness * 2.5));
	ImGui::SliderFloat("Handle Height", &handle.handleHeight, 25, 175);
	ImGui::SliderFloat("Handle Width", &handle.handleWidth, 5, 75);
	ImGui::SliderFloat("Handle Thickness", &handle.handleThickness, 0, 10);



	ImGui::SliderFloat("Pommel Height", &pommel.pommelHeight, (handle.handleWidth/1.5), (handle.handleWidth*1.5));
	/*if (pommel.pommelHeight < handle.handleWidth/1.5)
	{
		pommel.normal_pommel->SetHeight(handle.handleWidth/1.5);
		pommel.mirrored_pommel->SetHeight(handle.handleWidth/1.5);
		pommel.pommelHeight = pommel.normal_pommel->GetHeight();

		updateHeights();
	}
	if (pommel.pommelHeight > (handle.handleWidth * 1.5))
	{
		pommel.normal_pommel->SetHeight((handle.handleWidth *1.5));
		pommel.mirrored_pommel->SetHeight((handle.handleWidth *1.5));
		pommel.pommelHeight = pommel.normal_pommel->GetHeight();

		updateHeights();
	}*/

	ImGui::SliderFloat("Pommel Width", &pommel.pommelWidth, (handle.handleWidth/1.5),(handle.handleWidth*1.5));
	//if (pommel.pommelWidth < (handle.handleWidth / 1.5))
	//{
	//	pommel.normal_pommel->SetWidth(handle.handleWidth / 1.5);
	//	pommel.mirrored_pommel->SetWidth(handle.handleWidth / 1.5);
	//	pommel.pommelWidth = pommel.normal_pommel->GetWidth();

	//	updateWidth();
	//}

	//if (pommel.pommelHeight > (handle.handleWidth * 1.5))
	//{
	//	pommel.normal_pommel->SetWidth((handle.handleWidth * 1.5));
	//	pommel.mirrored_pommel->SetWidth((handle.handleWidth * 1.5));
	//	pommel.pommelWidth = pommel.normal_pommel->GetWidth();

	//	updateWidth();

	//}

	ImGui::SliderFloat("Pommel Thickness", &pommel.pommelThickness, (handle.handleThickness/2), (handle.handleThickness*1.5));

	if( ImGui::Button( "Regenerate Terrain" ) ) {
		if( terrainResolution != blade.normal_blade->GetResolution() ) {
			blade.normal_blade->Resize( terrainResolution );
			blade.mirrored_blade->Resize(terrainResolution);
			guard.normal_guard->Resize(terrainResolution);
			guard.mirrored_guard->Resize(terrainResolution);
			handle.normal_handle->Resize(terrainResolution);
			handle.mirrored_handle->Resize(terrainResolution);
			pommel.normal_pommel->Resize(terrainResolution);
			pommel.mirrored_pommel->Resize(terrainResolution);
		}
		
		blade.regen(renderer->getDevice(), renderer->getDeviceContext());
		guard.regen(renderer->getDevice(), renderer->getDeviceContext());
		handle.regen(renderer->getDevice(), renderer->getDeviceContext());
		pommel.regen(renderer->getDevice(), renderer->getDeviceContext());
	}

	ImGui::SliderFloat("Tip Height", &blade.bladeTipHeight, 0, 5);
	ImGui::SliderFloat("Blade Curvature", &blade.Curvature, terrainResolution/10, terrainResolution);
	ImGui::Checkbox("Add Curve", &blade.isCurve);
	ImGui::Checkbox("Inverse Curvature", &blade.inverseCurve);
	ImGui::SliderInt("Blade Edge", &blade.edge_offset, 1, terrainResolution/2);
	ImGui::SliderInt("Fuller Base", &blade.fuller_base, 0 , blade.normal_blade->GetHeight()*0.5);
	ImGui::SliderInt("Fuller Height", &blade.fuller_height, terrainResolution*0.51, terrainResolution*0.8 );
	ImGui::SliderInt("Fuller Width", &blade.fuller_width, 1, blade.normal_blade->GetWidth());
	ImGui::SliderFloat("Fuller Depth", &blade.fuller_depth, 0.1, blade.normal_blade->GetThickness() * 0.25);

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

	terrainResolution = blade.normal_blade->GetResolution();

	blade.mirrored_blade->setMirrored(true);
	blade.bladeHeight = blade.normal_blade->GetHeight();
	blade.bladeThickness = blade.normal_blade->GetThickness();
	blade.bladeWidth = blade.normal_blade->GetWidth();
	
	guard.mirrored_guard->setMirrored(true);
	guard.guardHeight = guard.normal_guard->GetHeight();
	guard.guardThickness = guard.normal_guard->GetThickness();
	guard.guardWidth = guard.normal_guard->GetWidth();

	handle.normal_handle->setMirrored(true);
	handle.handleHeight = handle.normal_handle ->GetHeight();
	handle.handleThickness = handle.normal_handle->GetThickness();
	handle.handleWidth = handle.normal_handle->GetWidth();

	pommel.normal_pommel->setMirrored(true);
	pommel.pommelHeight = pommel.normal_pommel->GetHeight();
	pommel.pommelThickness = pommel.normal_pommel->GetThickness();
	pommel.pommelWidth = pommel.normal_pommel->GetWidth();

	/////////////////////////////////////////////

	blade.normal_blade->Resize(terrainResolution);
	blade.mirrored_blade->Resize(terrainResolution);
	guard.normal_guard->Resize(terrainResolution);
	guard.mirrored_guard->Resize(terrainResolution);
	handle.normal_handle->Resize(terrainResolution);
	handle.mirrored_handle->Resize(terrainResolution);
	pommel.normal_pommel->Resize(terrainResolution);
	pommel.mirrored_pommel->Resize(terrainResolution);

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
	handle.normal_handle->SetOffsetY(pommel.normal_pommel->GetDynamicHeight());
	handle.mirrored_handle->SetOffsetY(pommel.normal_pommel->GetDynamicHeight());
	handle.regen(renderer->getDevice(), renderer->getDeviceContext());

	//guard third
	guard.normal_guard->SetOffsetY(pommel.normal_pommel->GetDynamicHeight() + handle.normal_handle->GetDynamicHeight());
	guard.mirrored_guard->SetOffsetY(pommel.normal_pommel->GetDynamicHeight() + handle.normal_handle->GetDynamicHeight());
	guard.regen(renderer->getDevice(), renderer->getDeviceContext());

	//blade last
	blade.normal_blade->SetOffsetY(pommel.normal_pommel->GetDynamicHeight() + handle.normal_handle->GetDynamicHeight() + guard.normal_guard->GetDynamicHeight());
	blade.mirrored_blade->SetOffsetY(pommel.normal_pommel->GetDynamicHeight() + handle.normal_handle->GetDynamicHeight() + guard.normal_guard->GetDynamicHeight());
	blade.regen(renderer->getDevice(), renderer->getDeviceContext());
}

void App1::updateWidth()
{
	//regenerating pommel mesh
	pommel.regen(renderer->getDevice(), renderer->getDeviceContext());

	//regenerating handle mesh
	handle.normal_handle->SetOffsetZ(pommel.normal_pommel->GetDynamicWidth()/2 - handle.normal_handle->GetDynamicWidth()/2);
	handle.mirrored_handle->SetOffsetZ(pommel.normal_pommel->GetDynamicWidth() / 2 - handle.normal_handle->GetDynamicWidth() / 2);
	handle.regen(renderer->getDevice(), renderer->getDeviceContext());

	//regenerating guard mesh
	guard.normal_guard->SetOffsetZ((pommel.normal_pommel->GetDynamicWidth() / 2 - guard.normal_guard->GetDynamicWidth()/2));
	guard.mirrored_guard->SetOffsetZ(pommel.normal_pommel->GetDynamicWidth() / 2 - guard.normal_guard->GetDynamicWidth() / 2);
	guard.regen(renderer->getDevice(), renderer->getDeviceContext());

	//regenerating blade mesh
	blade.normal_blade->SetOffsetZ(pommel.normal_pommel->GetDynamicWidth()/2 - blade.normal_blade->GetDynamicWidth()/2);
	blade.mirrored_blade->SetOffsetZ(pommel.normal_pommel->GetDynamicWidth() / 2 - blade.normal_blade->GetDynamicWidth() / 2);
	blade.regen(renderer->getDevice(), renderer->getDeviceContext());
}


