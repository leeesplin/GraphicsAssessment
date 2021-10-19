// Light shader.h
// Basic single light shader setup
#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;


class TessellationShader : public BaseShader
{

private:
	//matrix buffer with added view and projection matrices used for casting shadows
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
		float padding2;
		float constantFactor;
		float linearFactor;
		float quadraticFactor;
		float padding3;
	};

	//buffer for camera position values to be sent to hull shader
	struct CameraBufferType
	{
		XMFLOAT3 cameraPosition;
		float padding;
	};


public:
	TessellationShader(ID3D11Device* device, HWND hwnd);
	~TessellationShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* heightMap, ID3D11ShaderResourceView* colourTexture, ID3D11ShaderResourceView* bumpTexture, ID3D11ShaderResourceView*depthMap, ID3D11ShaderResourceView*depthMap2, XMFLOAT3 cameraPosition, Light* light, Light* light2, float constantFactor, float linearFactor, float quadraticFactor);

private:
	void initShader(const wchar_t* vsFilename, const wchar_t* psFilename);
	void initShader(const wchar_t* vsFilename, const wchar_t* hsFilename, const wchar_t* dsFilename, const wchar_t* psFilename);

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* timeBuffer;
	ID3D11Buffer* cameraBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11SamplerState* sampleStateShadow;
};
