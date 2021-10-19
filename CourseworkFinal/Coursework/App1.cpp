// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"

App1::App1()
{

}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in, bool VSYNC, bool FULL_SCREEN)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);

	//load textures into texture manager
	textureMgr->loadTexture(L"flag", L"res/flag.png");
	textureMgr->loadTexture(L"WaveHeightMap", L"res/waterDisMap.png");
	textureMgr->loadTexture(L"WaveNormalMap", L"res/waterNormalMap.png");
	textureMgr->loadTexture(L"HeightMap", L"res/heightMap.png");
	textureMgr->loadTexture(L"HeightMapFlip", L"res/heightMapFlip.png");
	textureMgr->loadTexture(L"HeightMapColour", L"res/heightMapColour.png");
	textureMgr->loadTexture(L"HeightMapColourFlip", L"res/heightMapColourFlip.png");
	textureMgr->loadTexture(L"NormalMap", L"res/NormalMap.png");
	textureMgr->loadTexture(L"NormalMapFlip", L"res/NormalMapFlip.png");
	textureMgr->loadTexture(L"tree", L"res/tree.png");
	textureMgr->loadTexture(L"skybox", L"res/Skybox2.png");
	textureMgr->loadTexture(L"lighthouse", L"res/brick1.dds");

	//initialize geometry to be used
	waterPlaneMesh = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());
	terrainPlaneMesh = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());
	flagPlaneMeshFront = new FlagMesh(renderer->getDevice(), renderer->getDeviceContext(), 3);
	flagPlaneMeshBack = new FlagMesh(renderer->getDevice(), renderer->getDeviceContext(), 3);
	terrainTessMesh = new TessellationMesh(renderer->getDevice(), renderer->getDeviceContext());
	orthoMesh = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), screenWidth, screenHeight, 0, 0);
	billboardMesh = new BillboardPointMesh(renderer->getDevice(), renderer->getDeviceContext());
	skyboxMesh = new SkyboxMesh(renderer->getDevice(), renderer->getDeviceContext(),1);
	lightHouse = new Model(renderer->getDevice(), renderer->getDeviceContext(), "res/models/Lighthouse.obj");
	
	//initialize shaders
	waveShader = new WaveShader(renderer->getDevice(), hwnd);
	heightMapShader = new HeightMapShader(renderer->getDevice(), hwnd);
	lightShader = new LightShader(renderer->getDevice(), hwnd);
	flagShader = new FlagShader(renderer->getDevice(), hwnd);
	tessShader = new TessellationShader(renderer->getDevice(), hwnd);
	depthShader = new DepthShader(renderer->getDevice(), hwnd);
	blurShader = new DOFBlurShader(renderer->getDevice(), hwnd);
	billboardShader = new BillboardShader(renderer->getDevice(), hwnd);
	skyboxShader = new SkyboxShader(renderer->getDevice(), hwnd);

	//set shadow map dimensions
	int shadowmapWidth = 1024;
	int shadowmapHeight = 1024;

	//set scene dimensions (in units not pixels)
	int sceneWidth = 200;
	int sceneHeight = 100;

	//initialize shadow maps
	shadowMapLight = new ShadowMap(renderer->getDevice(), shadowmapWidth, shadowmapHeight);
	shadowMapLight2 = new ShadowMap(renderer->getDevice(), shadowmapWidth, shadowmapHeight);
	shadowMapCamera = new ShadowMap(renderer->getDevice(), shadowmapWidth, shadowmapHeight);

	//initialize render texutre
	renderTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);

	//initialize lighting values
	pointLight = new Light;
	pointLight->setAmbientColour(0.0f, 0.0f, 0.0f, 1.0f);
	pointLight->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	pointLight->setDirection(0.7f, -0.5f, 0.0f);
	pointLight->setPosition(50.0f, 10.0f, 50.0f);
	pointLight->generateProjectionMatrix(2.1f, 200.0f);

	directionalLight = new Light;
	directionalLight->setAmbientColour(0.2f, 0.2f, 0.2f, 1.0f);
	directionalLight->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	directionalLight->setDirection(0.0f, -0.5f, -0.7f);
	directionalLight->setPosition(50.f, 30.f, 50.f);
	directionalLight->setSpecularPower(10.0f);
	directionalLight->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);
	directionalLight->generateOrthoMatrix((float)sceneWidth, (float)sceneHeight, 0.1f, 100.f);

	//initialize screen dimensions
	screenWidth1 = screenWidth;
	screenHeight1 = screenHeight;
}


App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D objects.
	if (waveShader)
	{
		delete waveShader;
		waveShader = 0;
	}

	if (heightMapShader)
	{
		delete heightMapShader;
		heightMapShader = 0;
	}

	if (lightShader)
	{
		delete lightShader;
		lightShader = 0;
	}
	
	if (flagShader)
	{
		delete flagShader;
		flagShader = 0;
	}

	if (tessShader)
	{
		delete tessShader;
		tessShader = 0;
	}

	if (depthShader)
	{
		delete depthShader;
		depthShader = 0;
	}

	if (blurShader)
	{
		delete blurShader;
		blurShader = 0;
	}

	if (billboardShader)
	{
		delete billboardShader;
		billboardShader = 0;
	}

	if (skyboxShader)
	{
		delete skyboxShader;
		skyboxShader = 0;
	}

}


bool App1::frame()
{
	bool result;

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

	// Perform depth passes
	depthPassLight();
	depthPassLight2();
	depthPassCamera();
	// Render scene
	RenderToTexturePass();
	finalPass();

	return true;
}

void App1::depthPassLight()
{
	// Set the render target to be the render to texture.
	shadowMapLight->BindDsvAndSetNullRenderTarget(renderer->getDeviceContext());

	// get the world, view, and projection matrices from the light and d3d objects.
	directionalLight->generateViewMatrix();
	XMMATRIX lightViewMatrix = directionalLight->getViewMatrix();
	XMMATRIX lightProjectionMatrix = directionalLight->getOrthoMatrix();
	XMMATRIX worldMatrix = renderer->getWorldMatrix();

	//get the depth from the light of all geometry in the scene 
	worldMatrix = XMMatrixTranslation(0, 3.0, 0);
	waterPlaneMesh->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix, NULL, NULL, timer, amplitude, frequency, speed);
	depthShader->render(renderer->getDeviceContext(), waterPlaneMesh->getIndexCount());
	worldMatrix = renderer->getWorldMatrix();

	//get depth of standard plane mesh terrain so that we can always know where shadows are supposed to be when the plane is fully tessellated
	terrainPlaneMesh->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix, textureMgr->getTexture(L"HeightMap"), NULL, timer, amplitude, frequency, speed);
	depthShader->render(renderer->getDeviceContext(), terrainPlaneMesh->getIndexCount());

	//get depth of both flag meshes
	worldMatrix *= XMMatrixTranslation(35.25f, 16.0f, 55.0f);
	flagPlaneMeshBack->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix, NULL, textureMgr->getTexture(L"flag"), timer, amplitude, frequency, speed);
	depthShader->render(renderer->getDeviceContext(), flagPlaneMeshBack->getIndexCount());
	worldMatrix = renderer->getWorldMatrix();

	worldMatrix = XMMatrixRotationY(3.2);
	worldMatrix *= XMMatrixTranslation(37.25f, 16.0f, 55.0f);
	flagPlaneMeshFront->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix, NULL, textureMgr->getTexture(L"flag"), timer, amplitude, frequency, speed);
	depthShader->render(renderer->getDeviceContext(), flagPlaneMeshFront->getIndexCount());
	worldMatrix = renderer->getWorldMatrix();

	//get depth of lighthouse 
	worldMatrix = XMMatrixTranslation(35.0, 5.0, 55.0);
	lightHouse->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix, NULL, NULL, timer, amplitude, frequency, speed);
	depthShader->render(renderer->getDeviceContext(), lightHouse->getIndexCount());

	// Set back buffer as render target and reset view port.
	renderer->setBackBufferRenderTarget();
	renderer->resetViewport();
}

void App1::depthPassLight2()
{
	// Set the render target to be the render to texture.
	shadowMapLight2->BindDsvAndSetNullRenderTarget(renderer->getDeviceContext());

	pointLight->generateViewMatrix();
	XMMATRIX pointLightViewMatrix = pointLight->getViewMatrix();
	XMMATRIX pointLightProjectionMatrix = pointLight->getProjectionMatrix();

	XMMATRIX worldMatrix = renderer->getWorldMatrix();

	//get the depth from the light of all geometry in the scene 
	worldMatrix = XMMatrixTranslation(0, 3.0, 0);
	waterPlaneMesh->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, pointLightViewMatrix, pointLightProjectionMatrix, NULL, NULL, timer, amplitude, frequency, speed);
	depthShader->render(renderer->getDeviceContext(), waterPlaneMesh->getIndexCount());
	worldMatrix = renderer->getWorldMatrix();

	//get depth of standard plane mesh terrain so that we can always know where shadows are supposed to be when the plane is fully tessellated
	terrainPlaneMesh->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, pointLightViewMatrix, pointLightProjectionMatrix, textureMgr->getTexture(L"HeightMap"), NULL, timer, amplitude, frequency, speed);
	depthShader->render(renderer->getDeviceContext(), terrainPlaneMesh->getIndexCount());

	//get depth of both flag meshes
	worldMatrix *= XMMatrixTranslation(35.25f, 16.0f, 55.0f);
	flagPlaneMeshBack->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, pointLightViewMatrix, pointLightProjectionMatrix, NULL, textureMgr->getTexture(L"flag"), timer, amplitude, frequency, speed);
	depthShader->render(renderer->getDeviceContext(), flagPlaneMeshBack->getIndexCount());
	worldMatrix = renderer->getWorldMatrix();

	worldMatrix = XMMatrixRotationY(3.2);
	worldMatrix *= XMMatrixTranslation(37.25f, 16.0f, 55.0f);
	flagPlaneMeshFront->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, pointLightViewMatrix, pointLightProjectionMatrix, NULL, textureMgr->getTexture(L"flag"), timer, amplitude, frequency, speed);
	depthShader->render(renderer->getDeviceContext(), flagPlaneMeshFront->getIndexCount());
	worldMatrix = renderer->getWorldMatrix();

	//get depth of lighthouse
	worldMatrix = XMMatrixTranslation(35.0, 5.0, 55.0);
	lightHouse->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, pointLightViewMatrix, pointLightProjectionMatrix, NULL, NULL, timer, amplitude, frequency, speed);
	depthShader->render(renderer->getDeviceContext(), lightHouse->getIndexCount());

	// Set back buffer as render target and reset view port.
	renderer->setBackBufferRenderTarget();
	renderer->resetViewport();
}

void App1::depthPassCamera()
{
	// Set the render target to be the render to texture.
	shadowMapCamera->BindDsvAndSetNullRenderTarget(renderer->getDeviceContext());

	// get the world, view, and projection matrices from the camera and d3d objects.
	XMMATRIX cameraViewMatrix = camera->getViewMatrix();
	XMMATRIX cameraProjectionMatrix = renderer->getProjectionMatrix();
	XMMATRIX worldMatrix = renderer->getWorldMatrix();

	//get the depth from the camera off all geometry in the scene
	worldMatrix = XMMatrixTranslation(0, 3.0, 0);
	waterPlaneMesh->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, cameraViewMatrix, cameraProjectionMatrix, NULL, NULL, timer, amplitude, frequency, speed);
	depthShader->render(renderer->getDeviceContext(), waterPlaneMesh->getIndexCount());
	worldMatrix = renderer->getWorldMatrix();

	//get depth of standard plane mesh terrain so that we can always know where shadows are supposed to be when the plane is fully tessellated
	terrainPlaneMesh->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, cameraViewMatrix, cameraProjectionMatrix, textureMgr->getTexture(L"HeightMap"), NULL, timer, amplitude, frequency, speed);
	depthShader->render(renderer->getDeviceContext(), terrainPlaneMesh->getIndexCount());

	//get depth of both flags
	worldMatrix *= XMMatrixTranslation(35.25f, 16.0f, 55.0f);
	flagPlaneMeshBack->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, cameraViewMatrix, cameraProjectionMatrix, NULL, textureMgr->getTexture(L"flag"), timer, amplitude, frequency, speed);
	depthShader->render(renderer->getDeviceContext(), flagPlaneMeshBack->getIndexCount());
	worldMatrix = renderer->getWorldMatrix();

	worldMatrix = XMMatrixRotationY(3.2);
	worldMatrix *= XMMatrixTranslation(37.25f, 16.0f, 55.0f);
	flagPlaneMeshFront->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, cameraViewMatrix, cameraProjectionMatrix, NULL, textureMgr->getTexture(L"flag"), timer, amplitude, frequency, speed);
	depthShader->render(renderer->getDeviceContext(), flagPlaneMeshFront->getIndexCount());
	worldMatrix = renderer->getWorldMatrix();

	//get depth of lighthouse
	worldMatrix = XMMatrixTranslation(35.0, 5.0, 55.0);
	lightHouse->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, cameraViewMatrix, cameraProjectionMatrix, NULL, NULL, timer, amplitude, frequency, speed);
	depthShader->render(renderer->getDeviceContext(), lightHouse->getIndexCount());

	// Set back buffer as render target and reset view port.
	renderer->setBackBufferRenderTarget();
	renderer->resetViewport();
}

void App1::RenderToTexturePass()
{
	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view, projection from the camera and Direct3D objects.
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();

	//set up render target 
	renderTexture->setRenderTarget(renderer->getDeviceContext());
	renderTexture->clearRenderTarget(renderer->getDeviceContext(), 0.39f, 0.58f, 0.93f, 1.0f);

	pointLight->setPosition(lightX, lightY, lightZ);

	//texture scrolling value calculation
	textureTranslation += 0.0001f;

	//allows for infinite scrolling of texture
	if (textureTranslation > 1.0f)
	{
		textureTranslation = 0.0f;
	}


	//sets up skybox
	renderer->setZBuffer(false); //turn off z-buffer so that skybox can be drawn behind all geometry
	worldMatrix = XMMatrixRotationY(3.0f); //rotate the skybox so that the moon is in the right position
	worldMatrix *= XMMatrixTranslation(camera->getPosition().x, camera->getPosition().y, camera->getPosition().z); //make skybox follow the player's position
	skyboxMesh->sendData(renderer->getDeviceContext());
	skyboxShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"skybox"));
	skyboxShader->render(renderer->getDeviceContext(), skyboxMesh->getIndexCount());
	worldMatrix = renderer->getWorldMatrix();
	renderer->setZBuffer(true);

	//move and manipulate the water planemesh and render
	worldMatrix = XMMatrixTranslation(0, 3.0, 0);
	waterPlaneMesh->sendData(renderer->getDeviceContext());
	waveShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"WaveHeightMap"), textureMgr->getTexture(L"WaveNormalMap"), shadowMapLight->getDepthMapSRV(), 
		shadowMapLight2->getDepthMapSRV(), directionalLight, pointLight, camera->getPosition(), textureTranslation, constantFactor, linearFactor, quadraticFactor);
	waveShader->render(renderer->getDeviceContext(), waterPlaneMesh->getIndexCount());
	worldMatrix = renderer->getWorldMatrix();

	//manipulate the terrain tessellation mesh, subdividing as the camer gets closer and vice versa and renders
	//gets the depth map of all objects from the light and casts shadows on the terrain
	terrainTessMesh->sendData(renderer->getDeviceContext());
	tessShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"HeightMapFlip"), textureMgr->getTexture(L"HeightMapColourFlip"), textureMgr->getTexture(L"NormalMapFlip"),
		shadowMapLight->getDepthMapSRV(), shadowMapLight2->getDepthMapSRV(), camera->getPosition(), directionalLight, pointLight, constantFactor, linearFactor, quadraticFactor);
	tessShader->render(renderer->getDeviceContext(), terrainTessMesh->getIndexCount());
	
	//move and manipulate the flag meshes and render
	worldMatrix *= XMMatrixTranslation(35.25f, 16.0f, 55.0f);
	flagPlaneMeshBack->sendData(renderer->getDeviceContext());
	flagShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"flag"), directionalLight, pointLight, timer, constantFactor, linearFactor, quadraticFactor, amplitude, frequency, speed);
	flagShader->render(renderer->getDeviceContext(), flagPlaneMeshBack->getIndexCount());
	worldMatrix = renderer->getWorldMatrix();

	worldMatrix = XMMatrixRotationY(3.2);
	worldMatrix *= XMMatrixTranslation(37.25f, 16.0f, 55.0f);
	flagPlaneMeshFront->sendData(renderer->getDeviceContext());
	flagShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"flag"), directionalLight, pointLight, timer, constantFactor, linearFactor, quadraticFactor, amplitude, frequency, speed);
	flagShader->render(renderer->getDeviceContext(), flagPlaneMeshFront->getIndexCount());
	worldMatrix = renderer->getWorldMatrix();

	//move and render the lighthouse with basic lighting shader
	worldMatrix = XMMatrixTranslation(35.0, 5.0, 55.0);
	lightHouse->sendData(renderer->getDeviceContext());
	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"lighthouse"), directionalLight, pointLight, constantFactor, linearFactor, quadraticFactor);
	lightShader->render(renderer->getDeviceContext(), lightHouse->getIndexCount());
	worldMatrix = renderer->getWorldMatrix();

	//create and render the tree billboards
	//set the blendstate to make colours with 0 alpha value invisible, allowing the user to see the geometry behind
	worldMatrix = XMMatrixTranslation(90.0, 12.0, 70.0);
	renderer->getDeviceContext()->OMSetBlendState(billboardShader->blendState, NULL, 0xffffffff);
	billboardMesh->sendData(renderer->getDeviceContext());
	billboardShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"tree"), directionalLight, pointLight, constantFactor, linearFactor, quadraticFactor);
	billboardShader->render(renderer->getDeviceContext(), lightHouse->getIndexCount());
	renderer->getDeviceContext()->OMSetBlendState(billboardShader->defaultBlendState, NULL, 0xffffffff);
	renderer->setBackBufferRenderTarget();	
}

void App1::finalPass()
{
	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();

	renderer->setZBuffer(false);
	XMMATRIX orthoMatrix = renderer->getOrthoMatrix();
	XMMATRIX orthoViewMatrix = camera->getOrthoViewMatrix();

	//render the scene texture to an ortho mesh and blur (depth of field)
	orthoMesh->sendData(renderer->getDeviceContext());
	blurShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, renderTexture->getShaderResourceView(), shadowMapCamera->getDepthMapSRV(), screenHeight1, screenWidth1, shouldBlur, blurDistance);
	blurShader->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());
	renderer->setZBuffer(true);

	// Render GUI
	gui();

	// Swap the buffers
	renderer->endScene();
}

void App1::gui()
{
	// Force turn off unnecessary shader stages.
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->HSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->DSSetShader(NULL, NULL, 0);

	// Build UI
	ImGui::Text("FPS: %.2f", timer->getFPS());
	ImGui::Checkbox("Wireframe mode", &wireframeToggle);

	//controllable light position
	ImGui::SliderFloat("LightX", &lightX, -100.0f, 100.0f, "%.3f", 1.0f);
	ImGui::SliderFloat("LightY", &lightY, -100.0f, 100.0f, "%.3f", 1.0f);
	ImGui::SliderFloat("LightZ", &lightZ, -100.0f, 100.0f, "%.3f", 1.0f);

	ImGui::SliderFloat("Flag Speed", &speed, 1.0f, 10.0f, "%.3f", 1.0f);

	//toggleable blur and controllable blur distance
	ImGui::Checkbox("Blur", &shouldBlur);
	ImGui::SliderFloat("Blur Distance", &blurDistance, 0.0f, 10.0f, "%.3f", 1.0f);

	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

