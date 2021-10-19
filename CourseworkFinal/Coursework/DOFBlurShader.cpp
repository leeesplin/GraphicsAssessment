#include "DOFBlurShader.h"

DOFBlurShader::DOFBlurShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	initShader(L"DOFblur_vs.cso", L"DOFblur_ps.cso");
}

DOFBlurShader::~DOFBlurShader()
{
	// Release the sampler state.
	if (sampleState)
	{
		sampleState->Release();
		sampleState = 0;
	}

	// Release the matrix constant buffer.
	if (matrixBuffer)
	{
		matrixBuffer->Release();
		matrixBuffer = 0;
	}

	// Release the blur buffer.
	if (boxBlurBuffer)
	{
		boxBlurBuffer->Release();
		boxBlurBuffer = 0;
	}

	// Release the layout.
	if (layout)
	{
		layout->Release();
		layout = 0;
	}

	//Release base shader components
	BaseShader::~BaseShader();
}

void DOFBlurShader::initShader(const wchar_t* vsFilename, const wchar_t* psFilename)
{
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC boxBlurBufferDesc;

	// Load (+ compile) shader files
	loadVertexShader(vsFilename);
	loadPixelShader(psFilename);

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	renderer->CreateSamplerState(&samplerDesc, &sampleState);

	// Setup blur buffer
	// Setup the description of the blur buffer that is in the pixel shader.
	boxBlurBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	boxBlurBufferDesc.ByteWidth = sizeof(BlurBufferType);
	boxBlurBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	boxBlurBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	boxBlurBufferDesc.MiscFlags = 0;
	boxBlurBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&boxBlurBufferDesc, NULL, &boxBlurBuffer);
}


void DOFBlurShader::setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* texture2, float screenHeight, float screenWidth, bool isBlurring, float blurRange)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;

	// Transpose the matrices to prepare them for the shader.
	XMMATRIX tworld = XMMatrixTranspose(worldMatrix);
	XMMATRIX tview = XMMatrixTranspose(viewMatrix);
	XMMATRIX tproj = XMMatrixTranspose(projectionMatrix);

	deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->world = tworld;
	dataPtr->view = tview;
	dataPtr->projection = tproj;
	deviceContext->Unmap(matrixBuffer, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);

	//Additional
	// Send blur data to pixel shader
	BlurBufferType* blurPtr;
	deviceContext->Map(boxBlurBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	blurPtr = (BlurBufferType*)mappedResource.pData;
	blurPtr->screenHeight = screenHeight;
	blurPtr->screenWidth = screenWidth;
	blurPtr->blurDistance = blurRange;	
	blurPtr->shouldBlur = isBlurring;
	deviceContext->Unmap(boxBlurBuffer, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &boxBlurBuffer);

	// Set shader texture resource and sampler in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);
	deviceContext->PSSetShaderResources(1, 1, &texture2);
	deviceContext->PSSetSamplers(0, 1, &sampleState);
}
