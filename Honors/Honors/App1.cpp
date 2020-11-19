#include "App1.h"

App1::App1()
{
	blade = nullptr;
	shader = nullptr;
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in, bool VSYNC, bool FULL_SCREEN)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);

	textureMgr->loadTexture(L"grass", L"res/grass.png");	
	textureMgr->loadTexture(L"white", L"res/DefaultDiffuse.png");

	// Create Mesh object and shader object
	blade = new Blade(renderer->getDevice(), renderer->getDeviceContext());
	blade_mirrored = new Blade(renderer->getDevice(), renderer->getDeviceContext());

	guard = new Guard(renderer->getDevice(), renderer->getDeviceContext());
	guard_mirrored = new Guard(renderer->getDevice(), renderer->getDeviceContext());

	handle = new Handle(renderer->getDevice(), renderer->getDeviceContext());
	handle_mirrored = new Handle(renderer->getDevice(), renderer->getDeviceContext());

	pommel = new Pommel(renderer->getDevice(), renderer->getDeviceContext());
	pommel_mirrored = new Pommel(renderer->getDevice(), renderer->getDeviceContext());

	shader = new LightShader(renderer->getDevice(), hwnd);

	light = new Light;

	initValues();

}


App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.
	if (blade)
	{
		delete blade;
		blade = 0;
	}
	if (shader)
	{
		delete shader;
		shader = 0;
	}
}


bool App1::frame()
{
	bool result;


	if (bladeHeight != blade->GetHeight())
	{
		blade->Regenerate(renderer->getDevice(), renderer->getDeviceContext());
		blade->SetHeight(bladeHeight);
	
		blade_mirrored->Regenerate(renderer->getDevice(), renderer->getDeviceContext());
		blade_mirrored->SetHeight(bladeHeight);
	}

	if (bladeThickness != blade->GetThickness())
	{
		blade->Regenerate(renderer->getDevice(), renderer->getDeviceContext());
		blade->SetThickness(bladeThickness);

		blade_mirrored->Regenerate(renderer->getDevice(), renderer->getDeviceContext());
		blade_mirrored->SetThickness(bladeThickness);
	}

	if (bladeWidth != blade->GetWidth())
	{
		blade->Regenerate(renderer->getDevice(), renderer->getDeviceContext());
		blade->SetWidth(bladeWidth);

		blade_mirrored->Regenerate(renderer->getDevice(), renderer->getDeviceContext());
		blade_mirrored->SetWidth(bladeWidth);
	}

	if (guardHeight != guard->GetHeight())
	{
		guard->Regenerate(renderer->getDevice(), renderer->getDeviceContext());
		guard->SetHeight(guardHeight);

		guard_mirrored->Regenerate(renderer->getDevice(), renderer->getDeviceContext());
		guard_mirrored->SetHeight(guardHeight);
	}

	if (guardThickness != guard->GetThickness())
	{
		guard->Regenerate(renderer->getDevice(), renderer->getDeviceContext());
		guard->SetThickness(guardThickness);

		guard_mirrored->Regenerate(renderer->getDevice(), renderer->getDeviceContext());
		guard_mirrored->SetThickness(guardThickness);
	}

	if (guardWidth != guard->GetWidth())
	{
		guard->Regenerate(renderer->getDevice(), renderer->getDeviceContext());
		guard->SetWidth(guardWidth);

		guard_mirrored->Regenerate(renderer->getDevice(), renderer->getDeviceContext());
		guard_mirrored->SetWidth(guardWidth);
	}

	if (handleHeight != handle->GetHeight())
	{
		handle->Regenerate(renderer->getDevice(), renderer->getDeviceContext());
		handle->SetHeight(handleHeight);

		handle_mirrored->Regenerate(renderer->getDevice(), renderer->getDeviceContext());
		handle_mirrored->SetHeight(handleHeight);
	}

	if (handleThickness != handle->GetThickness())
	{
		handle->Regenerate(renderer->getDevice(), renderer->getDeviceContext());
		handle->SetThickness(handleThickness);

		handle_mirrored->Regenerate(renderer->getDevice(), renderer->getDeviceContext());
		handle_mirrored->SetThickness(handleThickness);
	}

	if (handleWidth != handle->GetWidth())
	{
		handle->Regenerate(renderer->getDevice(), renderer->getDeviceContext());
		handle->SetWidth(handleWidth);

		handle_mirrored->Regenerate(renderer->getDevice(), renderer->getDeviceContext());
		handle_mirrored->SetWidth(handleWidth);
	}

	if (pommelHeight != pommel->GetHeight())
	{
		pommel->Regenerate(renderer->getDevice(), renderer->getDeviceContext());
		pommel->SetHeight(pommelHeight);

		pommel_mirrored->Regenerate(renderer->getDevice(), renderer->getDeviceContext());
		pommel_mirrored->SetHeight(pommelHeight);
	}

	if (pommelThickness != pommel->GetThickness())
	{
		pommel->Regenerate(renderer->getDevice(), renderer->getDeviceContext());
		pommel->SetThickness(pommelThickness);

		pommel_mirrored->Regenerate(renderer->getDevice(), renderer->getDeviceContext());
		pommel_mirrored->SetThickness(pommelThickness);
	}

	if (pommelWidth != pommel->GetWidth())
	{
		pommel->Regenerate(renderer->getDevice(), renderer->getDeviceContext());
		pommel->SetWidth(pommelWidth);

		pommel_mirrored->Regenerate(renderer->getDevice(), renderer->getDeviceContext());
		pommel_mirrored->SetWidth(pommelWidth);
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
	blade->sendData(renderer->getDeviceContext());
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"grass"), light);
	shader->render(renderer->getDeviceContext(), blade->getIndexCount());

	blade_mirrored->sendData(renderer->getDeviceContext());
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"grass"), light);
	shader->render(renderer->getDeviceContext(), blade_mirrored->getIndexCount());

	guard->sendData(renderer->getDeviceContext());
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"grass"), light);
	shader->render(renderer->getDeviceContext(), guard->getIndexCount());

	guard_mirrored->sendData(renderer->getDeviceContext());
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"grass"), light);
	shader->render(renderer->getDeviceContext(), guard_mirrored->getIndexCount());

	handle->sendData(renderer->getDeviceContext());
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"grass"), light);
	shader->render(renderer->getDeviceContext(), handle->getIndexCount());

	handle_mirrored->sendData(renderer->getDeviceContext());
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"grass"), light);
	shader->render(renderer->getDeviceContext(), handle_mirrored->getIndexCount());

	pommel->sendData(renderer->getDeviceContext());
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"grass"), light);
	shader->render(renderer->getDeviceContext(), pommel->getIndexCount());

	pommel_mirrored->sendData(renderer->getDeviceContext());
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"grass"), light);
	shader->render(renderer->getDeviceContext(), pommel_mirrored->getIndexCount());
	
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
	ImGui::SliderFloat("Blade Height", &bladeHeight, 25, 175);
	ImGui::SliderFloat("Blade Width", &bladeWidth, 5, 75);
	ImGui::SliderFloat("Blade Thickness", &bladeThickness, 0, 10);

	ImGui::SliderFloat("Guard Height", &guardHeight, 25, 175);
	ImGui::SliderFloat("Guard Width", &guardWidth, 5, 75);
	ImGui::SliderFloat("Guard Thickness", &guardThickness, 0, 10);

	ImGui::SliderFloat("Handle Height", &handleHeight, 25, 175);
	ImGui::SliderFloat("Handle Width", &handleWidth, 5, 75);
	ImGui::SliderFloat("Handle Thickness", &handleThickness, 0, 10);

	ImGui::SliderFloat("Pommel Height", &pommelHeight, 25, 175);
	ImGui::SliderFloat("Pommel Width", &pommelWidth, 5, 75);
	ImGui::SliderFloat("Pommel Thickness", &pommelThickness, 0, 10);

	if( ImGui::Button( "Regenerate Terrain" ) ) {
		if( terrainResolution != blade->GetResolution() ) {
			blade->Resize( terrainResolution );
			blade_mirrored->Resize(terrainResolution);
		}
		blade->Regenerate( renderer->getDevice(), renderer->getDeviceContext() );
		blade_mirrored->Regenerate(renderer->getDevice(), renderer->getDeviceContext());
	}



	ImGui:
	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void App1::initValues()
{
	light->setAmbientColour(0.25f, 0.25f, 0.25f, 1.0f);
	light->setDiffuseColour(0.75f, 0.75f, 0.75f, 1.0f);
	light->setDirection(1.0f, -0.0f, 0.0f);

	camera->setPosition(-70.0f, 10.0f, 0.0f);
	camera->setRotation(0.0f, 90.0f, 2.5f);

	terrainResolution = blade->GetResolution();

	blade_mirrored->setMirrored(true);
	bladeHeight = blade->GetHeight();
	bladeThickness = blade->GetThickness();
	bladeWidth = blade->GetWidth();
	
	guard_mirrored->setMirrored(true);
	guardHeight = guard->GetHeight();
	guardThickness = guard->GetThickness();
	guardWidth = guard->GetWidth();

	guard->SetOffsetY(-15);
	guard_mirrored->SetOffsetY(-15);

	handle_mirrored->setMirrored(true);
	handleHeight = handle ->GetHeight();
	handleThickness = handle->GetThickness();
	handleWidth = handle->GetWidth();

	handle->SetOffsetY(-30);
	handle_mirrored->SetOffsetY(-30);

	pommel_mirrored->setMirrored(true);
	pommelHeight = pommel->GetHeight();
	pommelThickness = pommel->GetThickness();
	pommelWidth = pommel->GetWidth();

	pommel->SetOffsetY(-45);
	pommel_mirrored->SetOffsetY(-45);

	/////////////////////////////////////////////

	blade->Resize(terrainResolution);
	blade_mirrored->Resize(terrainResolution);

	blade->Regenerate(renderer->getDevice(), renderer->getDeviceContext());
	blade_mirrored->Regenerate(renderer->getDevice(), renderer->getDeviceContext());

	guard->Resize(terrainResolution);
	guard_mirrored->Resize(terrainResolution);

	guard->Regenerate(renderer->getDevice(), renderer->getDeviceContext());
	guard_mirrored->Regenerate(renderer->getDevice(), renderer->getDeviceContext());

	handle->Resize(terrainResolution);
	handle_mirrored->Resize(terrainResolution);

	handle->Regenerate(renderer->getDevice(), renderer->getDeviceContext());
	handle_mirrored->Regenerate(renderer->getDevice(), renderer->getDeviceContext());

	pommel->Resize(terrainResolution);
	pommel_mirrored->Resize(terrainResolution);

	pommel->Regenerate(renderer->getDevice(), renderer->getDeviceContext());
	pommel_mirrored->Regenerate(renderer->getDevice(), renderer->getDeviceContext());

}