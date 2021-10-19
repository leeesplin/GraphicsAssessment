#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;

class DOFBlurShader : public BaseShader
{
private:
	//buffer for blur values to be sent to pixel shader
	struct BlurBufferType
	{
		float screenWidth;
		float screenHeight;
		float blurDistance;
		bool shouldBlur;
	};

public:
	DOFBlurShader(ID3D11Device* device, HWND hwnd);
	~DOFBlurShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* texture2, float screenHeight, float screenWidth, bool isBlurring, float blurRange);

private:
	void initShader(const wchar_t* cs, const wchar_t* ps);

private:
	ID3D11Buffer * matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* boxBlurBuffer;
};

