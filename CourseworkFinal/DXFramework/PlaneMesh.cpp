// plane mesh
// Quad mesh made of many quads. Default is 100x100
#include "planemesh.h"

// Initialise buffer and load texture.
PlaneMesh::PlaneMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int lresolution)
{
	resolution = lresolution;
	initBuffers(device);
}

// Release resources.
PlaneMesh::~PlaneMesh()
{
	// Run parent deconstructor
	BaseMesh::~BaseMesh();
}

// Generate plane (including texture coordinates and normals).
void PlaneMesh::initBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	VectorType tangent, binormal, normal;
	unsigned long* indices;
	int index, i, j;
	float positionX, positionZ, u, v, increment;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	
	// Calculate the number of vertices in the terrain mesh.
	vertexCount = (resolution - 1) * (resolution - 1) * 8;


	indexCount = vertexCount;
	vertices = new VertexType[vertexCount];
	indices = new unsigned long[indexCount];
	

	index = 0;
	// UV coords.
	u = 0;
	v = 0;
	increment = 1.0f / resolution;
	
	for (j = 0; j<(resolution - 1); j++)
	{
		for (i = 0; i<(resolution - 1); i++)
		{
			// Upper left.
			positionX = (float)i;
			positionZ = (float)(j);

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].texture = XMFLOAT2(u, v);
			vertices[index].normal = XMFLOAT3(0.0, 1.0, 0.0);
			indices[index] = index;
			VertexType tempVertex1 = vertices[index];
			index++;

			// Upper right.
			positionX = (float)(i + 1);
			positionZ = (float)(j + 1);

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].texture = XMFLOAT2(u + increment, v + increment);
			vertices[index].normal = XMFLOAT3(0.0, 1.0, 0.0);
			indices[index] = index;
			VertexType tempVertex2 = vertices[index];
			index++;

	

			// lower left
			positionX = (float)(i);
			positionZ = (float)(j + 1);
			

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].texture = XMFLOAT2(u, v + increment);
			vertices[index].normal = XMFLOAT3(0.0, 1.0, 0.0);
			indices[index] = index;
			VertexType tempVertex3 = vertices[index];
			index++;

			

			calculateTangentBinormal(tempVertex1, tempVertex2, tempVertex3, tangent, binormal);
			calculateNormal(tangent, binormal, normal);

			vertices[index - 3].normal.x = normal.x;
			vertices[index - 3].normal.y = normal.y;
			vertices[index - 3].normal.z = normal.z;
			vertices[index - 3].tangent.x = tangent.x;
			vertices[index - 3].tangent.y = tangent.y;
			vertices[index - 3].tangent.z = tangent.z;
			vertices[index - 3].binormal.x = binormal.x;
			vertices[index - 3].binormal.y = binormal.y;
			vertices[index - 3].binormal.z = binormal.z;


			vertices[index - 2].normal.x = normal.x;
			vertices[index - 2].normal.y = normal.y;
			vertices[index - 2].normal.z = normal.z;
			vertices[index - 2].tangent.x = tangent.x;
			vertices[index - 2].tangent.y = tangent.y;
			vertices[index - 2].tangent.z = tangent.z;
			vertices[index - 2].binormal.x = binormal.x;
			vertices[index - 2].binormal.y = binormal.y;
			vertices[index - 2].binormal.z = binormal.z;


			vertices[index - 1].normal.x = normal.x;
			vertices[index - 1].normal.y = normal.y;
			vertices[index - 1].normal.z = normal.z;
			vertices[index - 1].tangent.x = tangent.x;
			vertices[index - 1].tangent.y = tangent.y;
			vertices[index - 1].tangent.z = tangent.z;
			vertices[index - 1].binormal.x = binormal.x;
			vertices[index - 1].binormal.y = binormal.y;
			vertices[index - 1].binormal.z = binormal.z;

			// Upper left
			positionX = (float)(i);
			positionZ = (float)(j);

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].texture = XMFLOAT2(u, v);
			vertices[index].normal = XMFLOAT3(0.0, 1.0, 0.0);
			indices[index] = index;	
			VertexType tempVertex4 = vertices[index];
			index++;



			// Bottom right
			positionX = (float)(i + 1);
			positionZ = (float)(j);

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].texture = XMFLOAT2(u + increment, v);
			vertices[index].normal = XMFLOAT3(0.0, 1.0, 0.0);
			indices[index] = index;
			VertexType tempVertex5 = vertices[index];
			index++;

		
			// Upper right.
			positionX = (float)(i + 1);
			positionZ = (float)(j + 1);

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].texture = XMFLOAT2(u + increment, v + increment);
			vertices[index].normal = XMFLOAT3(0.0, 1.0, 0.0);
			indices[index] = index;		
			VertexType tempVertex6 = vertices[index];
			index++;

			calculateTangentBinormal(tempVertex4, tempVertex5, tempVertex6, tangent, binormal);
			calculateNormal(tangent, binormal, normal);

			vertices[index - 3].normal.x = normal.x;
			vertices[index - 3].normal.y = normal.y;
			vertices[index - 3].normal.z = normal.z;
			vertices[index - 3].tangent.x = tangent.x;
			vertices[index - 3].tangent.y = tangent.y;
			vertices[index - 3].tangent.z = tangent.z;
			vertices[index - 3].binormal.x = binormal.x;
			vertices[index - 3].binormal.y = binormal.y;
			vertices[index - 3].binormal.z = binormal.z;


			vertices[index - 2].normal.x = normal.x;
			vertices[index - 2].normal.y = normal.y;
			vertices[index - 2].normal.z = normal.z;
			vertices[index - 2].tangent.x = tangent.x;
			vertices[index - 2].tangent.y = tangent.y;
			vertices[index - 2].tangent.z = tangent.z;
			vertices[index - 2].binormal.x = binormal.x;
			vertices[index - 2].binormal.y = binormal.y;
			vertices[index - 2].binormal.z = binormal.z;


			vertices[index - 1].normal.x = normal.x;
			vertices[index - 1].normal.y = normal.y;
			vertices[index - 1].normal.z = normal.z;
			vertices[index - 1].tangent.x = tangent.x;
			vertices[index - 1].tangent.y = tangent.y;
			vertices[index - 1].tangent.z = tangent.z;
			vertices[index - 1].binormal.x = binormal.x;
			vertices[index - 1].binormal.y = binormal.y;
			vertices[index - 1].binormal.z = binormal.z;

			u += increment;
	
		}

		u = 0;
		v += increment;
	}

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
	
	// Release the arrays now that the buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;
	delete[] indices;
	indices = 0;
}

void PlaneMesh::calculateTangentBinormal(VertexType vertex1, VertexType vertex2, VertexType vertex3, VectorType& tangent, VectorType& binormal)
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

void PlaneMesh::calculateNormal(VectorType tangent, VectorType binormal, VectorType& normal)
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
