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
	shader = new LightShader(renderer->getDevice(), hwnd);

	light = new Light;
	light->setAmbientColour( 0.25f, 0.25f, 0.25f, 1.0f );
	light->setDiffuseColour(0.75f, 0.75f, 0.75f, 1.0f);
	light->setDirection(1.0f,-0.0f, 0.0f);

	camera->setPosition(-70.0f, 10.0f, 0.0f);
	camera->setRotation( 0.0f, 90.0f, 2.5f );

	terrainResolution = blade->GetResolution();
	blade_mirrored->setMirrored(true);
	bladeHeight = blade->GetHeight();
	bladeThickness = blade->GetThickness();
	bladeWidth = blade->GetWidth();

	blade->Resize(terrainResolution);
	blade_mirrored->Resize(terrainResolution);

	blade->Regenerate(renderer->getDevice(), renderer->getDeviceContext());
	blade_mirrored->Regenerate(renderer->getDevice(), renderer->getDeviceContext());
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
	ImGui::SliderFloat("Blade Width", &bladeWidth, 5,75 );
	ImGui::SliderFloat("Blade Thickness", &bladeThickness, 0, 10);

	if( ImGui::Button( "Regenerate Terrain" ) ) {
		if( terrainResolution != blade->GetResolution() ) {
			blade->Resize( terrainResolution );
			blade_mirrored->Resize(terrainResolution);
		}
		blade->Regenerate( renderer->getDevice(), renderer->getDeviceContext() );
		blade_mirrored->Regenerate(renderer->getDevice(), renderer->getDeviceContext());
	}




	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

