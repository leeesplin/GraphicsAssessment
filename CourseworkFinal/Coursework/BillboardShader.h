// Light shader.h
// Geometry shader example.
#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;


class BillboardShader : public BaseShader
{
private:
	//buffer for all lights in the scene to be sent to pixel shader
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

public:

	BillboardShader(ID3D11Device* device, HWND hwnd);
	~BillboardShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, Light* light, Light* light2, float constantFactor, float linearFactor, float quadraticFactor);
	
	//blend states for using alpha transparency and setting it back to default state
	ID3D11BlendState* blendState;
	ID3D11BlendState* defaultBlendState;

private:
	void initShader(const wchar_t* vsFilename, const wchar_t* psFilename);
	void initShader(const wchar_t* vsFilename, const wchar_t* gsFilename, const wchar_t* psFilename);

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* lightBuffer;

};
