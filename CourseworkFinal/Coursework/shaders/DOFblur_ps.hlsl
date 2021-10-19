Texture2D orthoMeshTex : register(t0);
Texture2D depthTexture : register(t1);

SamplerState sampler0 : register(s0);

cbuffer BlurBuffer : register(b0)
{
	float screenWidth;
	float screenHeight;
	float blurDistance;
	bool shouldBlur;
};

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

float4 main(InputType input) : SV_TARGET
{
	//initialize the colour to black
	float4 colour = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float depthValue = depthTexture.Sample(sampler0, input.tex).r;

	//determine the floating point size of a texel for a screen with this specific width and height
	float texelWidth = 1.0f / screenWidth;
	float texelHeight = 1.0f / screenHeight;
	
	if (shouldBlur == false)
	{
		colour = orthoMeshTex.Sample(sampler0, input.tex); //if the user has blur toggled off then render scene normally without blur
	}
	else
	{
		//if geometry is within a certain depth range then render normally without blur, if not then blur
		if (depthValue <= 0.995 + (blurDistance/1000) && depthValue >= 0.994 + (blurDistance / 1000))
		{
			
			colour = orthoMeshTex.Sample(sampler0, input.tex);
		}
		else
		{
			//add 27 pixels to the colour
			colour += orthoMeshTex.Sample(sampler0, input.tex);
			colour += orthoMeshTex.Sample(sampler0, input.tex + float2(-texelWidth, -texelHeight));
			colour += orthoMeshTex.Sample(sampler0, input.tex + float2(0.0f, texelHeight));
			colour += orthoMeshTex.Sample(sampler0, input.tex + float2(texelWidth, -texelHeight));
			colour += orthoMeshTex.Sample(sampler0, input.tex + float2(-texelWidth, 0.0f));
			colour += orthoMeshTex.Sample(sampler0, input.tex + float2(texelWidth, 0.0f));
			colour += orthoMeshTex.Sample(sampler0, input.tex + float2(-texelWidth, texelHeight));
			colour += orthoMeshTex.Sample(sampler0, input.tex + float2(0.0f, texelHeight));
			colour += orthoMeshTex.Sample(sampler0, input.tex + float2(texelWidth, texelHeight));
			colour += orthoMeshTex.Sample(sampler0, input.tex + float2(-texelWidth * 2, -texelHeight * 2));
			colour += orthoMeshTex.Sample(sampler0, input.tex + float2(0.0f, texelHeight * 2));
			colour += orthoMeshTex.Sample(sampler0, input.tex + float2(texelWidth * 2, -texelHeight * 2));
			colour += orthoMeshTex.Sample(sampler0, input.tex + float2(-texelWidth * 2, 0.0f));
			colour += orthoMeshTex.Sample(sampler0, input.tex + float2(texelWidth * 2, 0.0f));
			colour += orthoMeshTex.Sample(sampler0, input.tex + float2(-texelWidth * 2, texelHeight * 2));
			colour += orthoMeshTex.Sample(sampler0, input.tex + float2(0.0f, texelHeight * 2));
			colour += orthoMeshTex.Sample(sampler0, input.tex + float2(texelWidth * 2, texelHeight * 2));
			colour += orthoMeshTex.Sample(sampler0, input.tex + float2(-texelWidth * 2 - texelWidth, -texelHeight * 2 - texelHeight));
			colour += orthoMeshTex.Sample(sampler0, input.tex + float2(0.0f, texelHeight * 2 + texelHeight));
			colour += orthoMeshTex.Sample(sampler0, input.tex + float2(texelWidth * 2 + texelHeight, -texelHeight * 2 - texelHeight));
			colour += orthoMeshTex.Sample(sampler0, input.tex + float2(-texelWidth * 2 - texelHeight, 0.0f));
			colour += orthoMeshTex.Sample(sampler0, input.tex + float2(texelWidth * 2 + texelHeight, 0.0f));
			colour += orthoMeshTex.Sample(sampler0, input.tex + float2(-texelWidth * 2 - texelHeight, texelHeight * 2 + texelHeight));
			colour += orthoMeshTex.Sample(sampler0, input.tex + float2(0.0f, texelHeight * 2 + texelHeight));
			colour += orthoMeshTex.Sample(sampler0, input.tex + float2(texelWidth * 2 + texelHeight, texelHeight * 2 + texelHeight));

			colour /= 27.0f;
		}
	}


	// Set the alpha channel to one
	colour.a = 1.0f;

	return colour;
	}