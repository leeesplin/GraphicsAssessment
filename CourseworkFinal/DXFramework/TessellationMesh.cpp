// Tessellation mesh.
// Builds a simple triangle mesh for tessellation demonstration
// Overrides sendData() function for different primitive topology
#include "tessellationmesh.h"

// initialise buffers and load texture.
TessellationMesh::TessellationMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	initBuffers(device);
}

// Release resources.
TessellationMesh::~TessellationMesh()
{
	// Run parent deconstructor
	BaseMesh::~BaseMesh();
}

// Build triangle (with texture coordinates and normals).
void TessellationMesh::initBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	VectorType tangent, binormal, normal;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	vertexCount = 4;
	indexCount = 4;

	vertices = new VertexType[vertexCount];
	indices = new unsigned long[indexCount];

	// Load the vertex array with data.
	vertices[0].position = XMFLOAT3(0.0f, 0.0f, 0.0f);  // Top left
	vertices[0].texture = XMFLOAT2(0.0f, 1.0f);
	vertices[0].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);

	vertices[1].position = XMFLOAT3(0.0f, 0.0f, 100.0f);  // Bottom left.
	vertices[1].texture = XMFLOAT2(0.0f, 0.0f);
	vertices[1].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);

	vertices[2].position = XMFLOAT3(100.0f, 0.0f, 100.0f);  // Bottom right.
	vertices[2].texture = XMFLOAT2(1.0f, 0.0f);
	vertices[2].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);

	vertices[3].position = XMFLOAT3(100.0f, 0.0f, 0.0f);  // top right
	vertices[3].texture = XMFLOAT2(1.0f, 0.0f);
	vertices[3].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);

	calculateTangentBinormal(vertices[0], vertices[1], vertices[3], tangent, binormal);
	calculateNormal(tangent, binormal, normal);

	vertices[0].normal.x = normal.x;
	vertices[0].normal.y = normal.y;
	vertices[0].normal.z = normal.z;
	vertices[0].tangent.x = tangent.x;
	vertices[0].tangent.y = tangent.y;
	vertices[0].tangent.z = tangent.z;
	vertices[0].binormal.x = binormal.x;
	vertices[0].binormal.y = binormal.y;
	vertices[0].binormal.z = binormal.z;


	vertices[1].normal.x = normal.x;
	vertices[1].normal.y = normal.y;
	vertices[1].normal.z = normal.z;
	vertices[1].tangent.x = tangent.x;
	vertices[1].tangent.y = tangent.y;
	vertices[1].tangent.z = tangent.z;
	vertices[1].binormal.x = binormal.x;
	vertices[1].binormal.y = binormal.y;
	vertices[1].binormal.z = binormal.z;

	vertices[2].normal.x = normal.x;
	vertices[2].normal.y = normal.y;
	vertices[2].normal.z = normal.z;
	vertices[2].tangent.x = tangent.x;
	vertices[2].tangent.y = tangent.y;
	vertices[2].tangent.z = tangent.z;
	vertices[2].binormal.x = binormal.x;
	vertices[2].binormal.y = binormal.y;
	vertices[2].binormal.z = binormal.z;

	vertices[3].normal.x = normal.x;
	vertices[3].normal.y = normal.y;
	vertices[3].normal.z = normal.z;
	vertices[3].tangent.x = tangent.x;
	vertices[3].tangent.y = tangent.y;
	vertices[3].tangent.z = tangent.z;
	vertices[3].binormal.x = binormal.x;
	vertices[3].binormal.y = binormal.y;
	vertices[3].binormal.z = binormal.z;

	// Load the index array with data.
	indices[0] = 3;  // Top left
	indices[1] = 2;  // Bottom left.
	indices[2] = 1;  // Bottom right.
	indices[3] = 0;  // Top right.

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

// Override sendData() to change topology type. Control point patch list is required for tessellation.
void TessellationMesh::sendData(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	// Set the type of primitive that should be rendered from this vertex buffer, in this case control patch for tessellation.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
}

void TessellationMesh::calculateTangentBinormal(VertexType vertex1, VertexType vertex2, VertexType vertex3, VectorType& tangent, VectorType& binormal)
{
	float vector1[3], vector2[3];
	float tuVector[2], tvVector[2];
	float den;
	float length;


	// Calculate the two vectors for this face.
	vector1[0] = vertex2.position.x - vertex1.position.x;
	vector1[1] = vertex2.position.y - vertex1.position.y;
	vector1[2] = vertex2.position.z - vertex1.position.z;

	vector2[0] = vertex3.position.x - vertex1.position.x;
	vector2[1] = vertex3.position.y - vertex1.position.y;
	vector2[2] = vertex3.position.z - vertex1.position.z;

	// Calculate the tu and tv texture space vectors.
	tuVector[0] = vertex2.texture.x - vertex1.texture.x;
	tvVector[0] = vertex2.texture.y - vertex1.texture.y;

	tuVector[1] = vertex3.texture.x - vertex1.texture.x;
	tvVector[1] = vertex3.texture.y - vertex1.texture.y;

	// Calculate the denominator of the tangent/binormal equation.
	den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

	// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
	tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
	tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
	tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

	binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
	binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
	binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

	// Calculate the length of this normal.
	length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));

	// Normalize the normal and then store it
	tangent.x = tangent.x / length;
	tangent.y = tangent.y / length;
	tangent.z = tangent.z / length;

	// Calculate the length of this normal.
	length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));

	// Normalize the normal and then store it
	binormal.x = binormal.x / length;
	binormal.y = binormal.y / length;
	binormal.z = binormal.z / length;

	return;
}

void TessellationMesh::calculateNormal(VectorType tangent, VectorType binormal, VectorType& normal)
{
	float length;


	// Calculate the cross product of the tangent and binormal which will give the normal vector.
	normal.x = (tangent.y * binormal.z) - (tangent.z * binormal.y);
	normal.y = (tangent.z * binormal.x) - (tangent.x * binormal.z);
	normal.z = (tangent.x * binormal.y) - (tangent.y * binormal.x);

	// Calculate the length of the normal.
	length = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));

	// Normalize the normal.
	normal.x = normal.x / length;
	normal.y = normal.y / length;
	normal.z = normal.z / length;

	return;
}

