Texture2D heightMapTexture : register(t0);
Texture2D flagTexture : register(t1);

SamplerState sampler0  : register(s0); 

cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer TimeBuffer : register(b1)
{
	float time;
	float amplitude;
	float frequency;
	float speed;
}

struct InputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct OutputType
{
    float4 position : SV_POSITION;
    float4 depthPosition : TEXCOORD0;
};

OutputType main(InputType input)
{
    OutputType output;

	//textures are sampled if they have been passed into the vertex shader
	//to make this shader more reusable, textures are passed into it based on what 
	//geometry is being passed into the shaders. If a heightmap texture is passed in then
	//that means that the heightmap plane is being passed into the shader and opertations 
	//can be performed based on that
	float4 heightMap = heightMapTexture.SampleLevel(sampler0, input.tex, 0);
	float4 flagTex = flagTexture.SampleLevel(sampler0, input.tex, 0);

	//if a heightmap texture has been passed in then do heightmap vertex manipulation
	if (heightMap.w != 0.0f)
	{
		input.position.y = heightMap.y * 20.0f;
	}

	//if a flag texture has been passed in then do flag vertex manipulation
	if (flagTex.w != 0)
	{
		input.position.z += sin(input.position.x / (frequency/2) + (time/2) * (speed/2)) * (amplitude/2);
	}

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    // Store the position value in a second input value for depth value calculations.
    output.depthPosition = output.position;
	
    return output;
}