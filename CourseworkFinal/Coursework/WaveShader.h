#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;

class WaveShader : public BaseShader
{
private:
	//matrix buffer with added view and projection matrix used for casting shadows
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
		XMMATRIX lightView[2];
		XMMATRIX lightProjection[2];
	};

	//buffer for light values to be sent to pixel shader
	struct LightBufferType
	{
		XMFLOAT4 ambient;
		XMFLOAT4 diffuse[2];
		XMFLOAT3 lightPosition;
		float padding;
		XMFLOAT3 direction;
		float specularPower;
		XMFLOAT4 specularColour;
		float constantFactor;
		float linearFactor;
		float quadraticFactor;
		float padding2;
	};

	//buffer for time values to be sent to vertex shader
	struct TimeBufferType
	{
		float textureTranslation;
		XMFLOAT3 padding;
	};

	//buffer for time values to be sent to vertex shader
	struct CameraBufferType
	{
		XMFLOAT3 cameraPosition;
		float padding2;
	};

public:
	WaveShader(ID3D11Device* device, HWND hwnd);
	~WaveShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* heightMap,ID3D11ShaderResourceView* normalMap, ID3D11ShaderResourceView*depthMap, ID3D11ShaderResourceView*depthMap2, Light* light, Light* light2, XMFLOAT3 cameraPosition, float textureTranslation, float constantFactor, float linearFactor, float quadraticFactor);

private:
	void initShader(const wchar_t* vs, const wchar_t* ps);

private:
	ID3D11Buffer * matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11SamplerState* sampleStateShadow;
	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* timeBuffer;
	ID3D11Buffer* cameraBuffer;
};