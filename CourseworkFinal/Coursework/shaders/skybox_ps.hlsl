Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 worldPosition : TEXCOORD1;
};

float4 main(InputType input) : SV_TARGET
{	
	// Sample the texture
	float4 textureColour = texture0.Sample(sampler0, input.tex);;
	
	//no lighting is applied since we don't want the skybox to interact with the scene, only texture is applied
	return textureColour;
}



