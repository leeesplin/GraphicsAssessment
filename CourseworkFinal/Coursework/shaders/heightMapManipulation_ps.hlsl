Texture2D colourTexture : register(t0);
Texture2D bumpMapTexture : register(t1);
Texture2D shadowMap : register(t2);

SamplerState sampler0 : register(s0);
SamplerState shadowSampler : register(s1);

cbuffer LightBuffer : register(b0)
{
	float4 ambientColour;
	float4 diffuseColour;
	float3 direction;
	float padding;
};

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float3 worldPosition : TEXCOORD1;
	float4 lightViewPos : TEXCOORD2;
};

// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculateLighting(float3 lightDirection, float3 bumpNormal, float4 diffuse)
{
	float intensity = saturate(dot(bumpNormal, lightDirection));
	float4 colour = saturate(diffuse * intensity);
	return colour;
}

float4 main(InputType input) : SV_TARGET
{
	float depthValue;
	float lightDepthValue;
	float shadowMapBias = 0.005f;
	float4 textureColour;
	float4 lightColour;
	float4 bumpMap;
	float3 bumpNormal;

	// Sample the textures for the texture to be used on the geometry and the bump map texture
	textureColour = colourTexture.Sample(sampler0, input.tex);
	bumpMap = bumpMapTexture.Sample(sampler0, input.tex);

	//use bump map to calculate the normal of the current vertex
	bumpMap = (bumpMap*2.0f) - 1.0f;
	bumpNormal = (bumpMap.x * input.tangent) + (bumpMap.y * input.binormal) + (bumpMap.z * input.normal);
	bumpNormal = normalize(bumpNormal);

	// Calculate the projected texture coordinates.
	float2 pTexCoord = input.lightViewPos.xy / input.lightViewPos.w;
	pTexCoord *= float2(0.5, -0.5);
	pTexCoord += float2(0.5f, 0.5f);

	// Determine if the projected coordinates are in the 0 to 1 range.  If not don't do lighting.
	if (pTexCoord.x < 0.f || pTexCoord.x > 1.f || pTexCoord.y < 0.f || pTexCoord.y > 1.f)
	{
		return textureColour;
	}

	// Sample the shadow map (get depth of geometry)
	depthValue = shadowMap.Sample(shadowSampler, pTexCoord).r;

	// Calculate the depth from the light.
	lightDepthValue = input.lightViewPos.z / input.lightViewPos.w;
	lightDepthValue -= shadowMapBias;

	//Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light th
	if (lightDepthValue < depthValue)
	{
			lightColour = ambientColour + calculateLighting(-direction, bumpNormal, diffuseColour);
	}

	lightColour = saturate(lightColour + ambientColour);
	return saturate(lightColour * textureColour);
}



