Texture2D displacementMap : register(t0);

SamplerState sampler0 : register(s0);


cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	matrix lightViewMatrix[2];
	matrix lightProjectionMatrix[2];
};

cbuffer TimeBuffer : register(b1)
{
	float textureTranslation;
	float3 padding;
}

cbuffer CameraBuffer : register(b2)
{
	float3 cameraPosition;
	float padding2;
}

struct InputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

struct OutputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float3 worldPosition : TEXCOORD1;
	float3 viewDirection : TEXCOORD2;
	float4 lightViewPos[2] : TEXCOORD3;
};

OutputType main(InputType input)
{
	OutputType output;

	//scroll the texture over the plane
	input.tex.y += textureTranslation;

	//sample displacement texture
	float4 TextureColour = displacementMap.SampleLevel(sampler0, input.tex, 0);

	//map the height according to the displacement map
	input.position.y += TextureColour.y * 2; //multiply by 2 so the wave displacement is easier to see

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	// Calculate the normal vector against the world matrix only and normalise.
	output.normal = mul(input.normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);

	// Calculate the tangent vector against the world matrix only and then normalize the final value.
	output.tangent = mul(input.tangent, (float3x3)worldMatrix);
	output.tangent = normalize(output.tangent);

	// Calculate the binormal vector against the world matrix only and then normalize the final value.
	output.binormal = mul(input.binormal, (float3x3)worldMatrix);
	output.binormal = normalize(output.binormal);

	output.worldPosition = mul(input.position, worldMatrix).xyz;

	// Determine the viewing direction based on the position of the camera and the position of the vertex in the world.
	output.viewDirection = cameraPosition.xyz - output.worldPosition.xyz;

	// Calculate the position of the vertice as viewed by the light source.
	output.lightViewPos[0] = mul(input.position, worldMatrix);
	output.lightViewPos[0] = mul(output.lightViewPos[0], lightViewMatrix[0]);
	output.lightViewPos[0] = mul(output.lightViewPos[0], lightProjectionMatrix[0]);

	output.lightViewPos[1] = mul(input.position, worldMatrix);
	output.lightViewPos[1] = mul(output.lightViewPos[1], lightViewMatrix[1]);
	output.lightViewPos[1] = mul(output.lightViewPos[1], lightProjectionMatrix[1]);

	// Normalize the viewing direction vector.
	output.viewDirection = normalize(output.viewDirection);

	return output;
}