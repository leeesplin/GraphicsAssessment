// Point mesh.
// For geometry shader demonstration.
// Note sendData() override.
#include "BillboardPointMesh.h"

// Initialise buffers and load texture.
BillboardPointMesh::BillboardPointMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	initBuffers(device);
}

// Release resources.
BillboardPointMesh::~BillboardPointMesh()
{
	// Run parent deconstructor
	BaseMesh::~BaseMesh();
}

// Generate point mesh. Simple triangle.
void BillboardPointMesh::initBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	vertexCount = 9;
	indexCount = 9;


	vertices = new VertexType[vertexCount];
	indices = new unsigned long[indexCount];

	// Load the vertex array with data.
	vertices[0].position = XMFLOAT3(0.0f, 0.0f, 15.0f);  // Top.
	vertices[0].texture = XMFLOAT2(0.0f, 1.0f);
	vertices[0].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	vertices[1].position = XMFLOAT3(-15.0f, -2.0f, 15.0f);  // Bottom left.
	vertices[1].texture = XMFLOAT2(0.0f, 0.0f);
	vertices[1].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	vertices[2].position = XMFLOAT3(-15.0f, 0.0f, 0.0f);  // Bottom right.
	vertices[2].texture = XMFLOAT2(1.0f, 0.0f);
	vertices[2].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	vertices[3].position = XMFLOAT3(-15.0f, 0.0f, -15.0f);  // Top.
	vertices[3].texture = XMFLOAT2(0.0f, 1.0f);
	vertices[3].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	vertices[4].position = XMFLOAT3(0.0f, -1.0f, -15.0f);  // Bottom left.
	vertices[4].texture = XMFLOAT2(0.0f, 0.0f);
	vertices[4].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	vertices[5].position = XMFLOAT3(15.0f, 0.0f, -15.0f);  // Bottom right.
	vertices[5].texture = XMFLOAT2(1.0f, 0.0f);
	vertices[5].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	vertices[6].position = XMFLOAT3(15.0f, 0.0f, 0.0f);  // Bottom right.
	vertices[6].texture = XMFLOAT2(1.0f, 0.0f);
	vertices[6].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	vertices[7].position = XMFLOAT3(15.0f, 0.0f, 15.0f);  // Bottom right.
	vertices[7].texture = XMFLOAT2(1.0f, 0.0f);
	vertices[7].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	vertices[8].position = XMFLOAT3(0.0f, -2.0f, 0.0f);  // Bottom right.
	vertices[8].texture = XMFLOAT2(1.0f, 0.0f);
	vertices[8].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	// Load the index array with data.
	indices[0] = 0;  // Top/
	indices[1] = 1;  // Bottom left.
	indices[2] = 2;  // Bottom right.
	indices[3] = 3;  // Bottom left.
	indices[4] = 4;  // Bottom right.
	indices[5] = 5;  // Bottom right.
	indices[6] = 6;  // Bottom right.
	indices[7] = 7;  // Bottom right.
	indices[8] = 8;  // Bottom right.

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	// Now create the vertex buffer.
	device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	// Create the index buffer.
	device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;
}

// Override sendData()
// Change in primitive topology (pointlist instead of trianglelist) for geometry shader use.
void BillboardPointMesh::sendData(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
}

