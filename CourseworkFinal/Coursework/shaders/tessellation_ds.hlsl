Texture2D texture0 : register(t0);

SamplerState sampler0 : register(s0);

cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	matrix lightViewMatrix[2];
	matrix lightProjectionMatrix[2];
};

struct ConstantOutputType
{
	float edges[4] : SV_TessFactor;
	float inside[2] : SV_InsideTessFactor;
};

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float3 worldPosition : TEXCOORD1;
	float4 colour : COLOR;
};

struct OutputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float3 worldPosition : TEXCOORD1;
	float4 lightViewPos[2] : TEXCOORD2;
	float4 colour : COLOR;
};


[domain("quad")]
OutputType main(ConstantOutputType input, float2 uvwCoord : SV_DomainLocation, const OutputPatch<InputType, 4> patch)
{
	float3 vertexPosition, vertexNormal, vertexTangent, vertexBinormal;
	OutputType output;
	
	//calculate new vertex position
	float3 v1 = lerp(patch[0].position, patch[1].position, uvwCoord.y);
	float3 v2 = lerp(patch[3].position, patch[2].position, uvwCoord.y);
	vertexPosition = lerp(v1, v2, uvwCoord.x);
	
	//calculate new vertex normal
	float3 n1 = lerp(patch[0].normal, patch[1].normal, uvwCoord.y);
	float3 n2 = lerp(patch[3].normal, patch[2].normal, uvwCoord.y);
	vertexNormal = lerp(n1, n2, uvwCoord.x);

	//calculate new vertex tangent
	float3 t1 = lerp(patch[0].tangent, patch[1].tangent, uvwCoord.y);
	float3 t2 = lerp(patch[3].tangent, patch[2].tangent, uvwCoord.y);
	vertexTangent = lerp(t1, t2, uvwCoord.x);

	//calculate new vertex binormal
	float3 b1 = lerp(patch[0].binormal, patch[1].binormal, uvwCoord.y);
	float3 b2 = lerp(patch[3].binormal, patch[2].binormal, uvwCoord.y);
	vertexBinormal = lerp(n1, n2, uvwCoord.x);

	//sample the heightmap texture
	float4 textureColour = textureColour = texture0.SampleLevel(sampler0, uvwCoord, 0);

	//manipulate the vertex based on the colour of the texture
	vertexPosition.y = textureColour.y * 20.0f;

	// Calculate the position of the new vertex against the world, view, and projection matrices.
	output.position = mul(float4(vertexPosition, 1.0f), worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Calculate the position of the new vertex as viewed by the light source.
	output.lightViewPos[0] = mul(float4(vertexPosition, 1.0f), worldMatrix);
	output.lightViewPos[0] = mul(output.lightViewPos[0], lightViewMatrix[0]);
	output.lightViewPos[0] = mul(output.lightViewPos[0], lightProjectionMatrix[0]);

	// Calculate the position of the new vertex as viewed by the light source.
	output.lightViewPos[1] = mul(float4(vertexPosition, 1.0f), worldMatrix);
	output.lightViewPos[1] = mul(output.lightViewPos[1], lightViewMatrix[1]);
	output.lightViewPos[1] = mul(output.lightViewPos[1], lightProjectionMatrix[1]);

	//set the texture coords of the new vertex
	output.tex = uvwCoord;

	// Calculate the normal vector against the world matrix only and normalise.
	output.normal = mul(vertexNormal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);

	// Calculate the tangent vector against the world matrix only and then normalize the final value.
	output.tangent = mul(vertexTangent, (float3x3)worldMatrix);
	output.tangent = normalize(output.tangent);

	// Calculate the binormal vector against the world matrix only and then normalize the final value.
	output.binormal = mul(vertexBinormal, (float3x3)worldMatrix);
	output.binormal = normalize(output.binormal);

	//calculate world position of new vertex 
	output.worldPosition = mul(float4(vertexPosition, 1.0f), worldMatrix).xyz;

	return output;
}

