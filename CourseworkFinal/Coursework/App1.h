// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "../DXFramework/DXF.h"
#include "WaveShader.h"
#include "HeightMapShader.h"
#include "LightShader.h"
#include "FlagShader.h"
#include "TessellationShader.h"
#include "DepthShader.h"
#include "DOFBlurShader.h"
#include "BillboardShader.h"
#include "SkyboxShader.h"

class App1 : public BaseApplication
{
public:
	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN);

	bool frame();

protected:
	bool render();
	void depthPassLight(); //depth pass used to calculate where shadows should be in relation to the directional light
	void depthPassLight2(); //depth pass used to calculate where shadows should be in relation to the directional light
	void depthPassCamera(); //depth pass used to see how far away things are from the camera to be used with depth of field blurring
	void RenderToTexturePass(); //renders the scene to a texture so that it can be blurred
	void finalPass(); //renders the final render of the scene
	void gui(); //displays all gui elements

private:
	PlaneMesh* waterPlaneMesh; //plane mesh used for the water
	PlaneMesh* terrainPlaneMesh; //plane mesh used for the terrain without tessellation
	FlagMesh* flagPlaneMeshFront; //flag mesh (small plane mesh) used for the front of the flag
	FlagMesh* flagPlaneMeshBack; //flag mesh (small plane mesh) used for the back of the flag
	TessellationMesh* terrainTessMesh; //tessellation mesh used for the dynamically tessellated terrain
	BillboardPointMesh* billboardMesh; //point mesh used for the creation of billboards with the geometry shader
	SkyboxMesh* skyboxMesh; //cube mesh used for the skybox around the scene

	WaveShader* waveShader; //manipulates the vertices in the plane to simulate waves using a scrolling texture
	HeightMapShader* heightMapShader; //offsets vertices by the colour value of a heightmap image
	LightShader* lightShader; //basic lighting shader
	FlagShader* flagShader; //manipulates the flag to look as if it's waving
	TessellationShader* tessShader; //similar purpose as the heightmap shader except the plane is subdivided as the camera gets closer and combine as it gets further away
	DepthShader* depthShader; //checks the depth of objects from a point
	DOFBlurShader* blurShader; //blurs the scene using depth of field
	BillboardShader* billboardShader; //creates billboards of trees to be placed in the scene
	SkyboxShader* skyboxShader; //modified lighting shader for skybox
	 
	ShadowMap* shadowMapLight; //depth map for the position of the light
	ShadowMap* shadowMapLight2; //depth map for the position of the light
	ShadowMap* shadowMapCamera; //depth map for the position of the camera

	Model* lightHouse; // lighthouse model

	OrthoMesh* orthoMesh; //ortho mesh for the scene to be rendered on
	RenderTexture* renderTexture; //render texture for the scene to be rendered to

	Light* pointLight; //point light
	Light* directionalLight; //directional light

	//adjustable positions for the point light
	float lightX = 29.0f;
	float lightY = 21.0f;
	float lightZ = 60.0f;

	float textureTranslation = 0.0f; //value that determines how much the scrolling texture should move

	//screen dimensions
	float screenWidth1;
	float screenHeight1;

	//adjustable vlaues for blurring
	bool shouldBlur = false;
	float blurDistance = 0.0f;

	//attenuation values
	float constantFactor = 0.2f;
	float linearFactor = 0.125f;
	float quadraticFactor = 0.0f;

	//flag manipulation values
	float amplitude = 0.2f;
	float frequency = 2.0f;
	float speed = 4.0f;
};

#endif