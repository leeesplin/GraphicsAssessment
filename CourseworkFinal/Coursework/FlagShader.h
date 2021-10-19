#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;

class FlagShader : public BaseShader
{
private:
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

	//buffer for time values to be sent to vertex shader
	struct TimeBufferType
	{
		float time;
		float amplitude;
		float frequency;
		float speed;
	};

public:
	FlagShader(ID3D11Device* device, HWND hwnd);
	~FlagShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture1, Light* light, Light* light2, Timer* time, float constantFactor, float linearFactor, float quadraticFactor, float amplitude, float frequency, float speed);

private:
	void initShader(const wchar_t* vs, const wchar_t* ps);

private:
	ID3D11Buffer * matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11SamplerState* sampleState2;
	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* timeBuffer;

	float timer1 = 0.0f; //value to hold the time to be used for calculating vertex manipulation of flag
};

