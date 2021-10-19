// Colour shader.h
// Simple shader example.
#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;


class DepthShader : public BaseShader
{
private:
	//buffer for time value to be sent to vertex shader
	struct TimeBufferType
	{
		float time;
		float amplitude;
		float frequency;
		float speed;
	};

public:
	DepthShader(ID3D11Device* device, HWND hwnd);
	~DepthShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* texture2, Timer* time, float amplitude, float frequency, float speed);

private:
	void initShader(const wchar_t* vs, const wchar_t* ps);

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* timeBuffer;

	float timer1 = 0.0f; //value to hold the time to be used for calculating vertex manipulation of flag
};
