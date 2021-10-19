Texture2D normalMap : register(t0);
Texture2D shadowMap1 : register(t1);
Texture2D shadowMap2 : register(t2);

SamplerState sampler0 : register(s0);
SamplerState shadowSampler : register(s1);

cbuffer LightBuffer : register(b0)
{
	float4 ambientColour;
	float4 diffuseColour[2];
	float3 lightPosition;
	float padding;
	float3 direction;
	float specularPower;
	float4 specularColour;
	float constantFactor;
	float linearFactor;
	float quadraticFactor;
	float padding2;
};

struct InputType
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

// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse)
{
	float intensity = saturate(dot(normal, lightDirection));
	float4 colour = saturate(diffuse * intensity);
	return colour;
}

//blinn phong specular calculation
float4 calcSpecular(float3 lightDirection, float3 normal, float3 viewVector, float4 specularColour, float4 specularPower)
{
	float3 halfway = normalize(lightDirection + viewVector);
	float specularIntensity = pow(max(dot(normal, halfway), 0.0f), specularPower);
	return saturate(specularColour * specularIntensity);
}

// Calculate lighting attenuation, causing the light to fall off the further away it is from the geometry
float calcAttenuation(float3 lightPos, float3 worldPos)
{
	//gets the distance between the light and the current pixel
	float dist = length(lightPos - worldPos);

	//calculate attenuation value
	float attenuation = 1 / (constantFactor + (linearFactor * dist) + (quadraticFactor * pow(dist, 2)));

	return attenuation;
}


float4 main(InputType input) : SV_TARGET
{
float depthValue;
	float lightDepthValue;
	float depthValue2;
	float light2DepthValue;
	float shadowMapBias = 0.015f;
	float shadowMapBias2 = 0.005f;
	float4 textureColour;
	float4 pointLightColour;
	float3 pointLightVector;
	float4 directionalLightColour;
	float4 finalLightColour;
	float4 bumpMap;
	float3 bumpNormal;

	textureColour = float4(0.24f, 0.37f, 0.6f, 1.0f);

	//sample texture for normal map
	bumpMap = normalMap.Sample(sampler0, input.tex);

	//calculate the normals of the bump map
	bumpMap = (bumpMap*2.0f) - 1.0f;
	bumpNormal = (bumpMap.x * input.tangent) + (bumpMap.y * input.binormal) + (bumpMap.z * input.normal);
	bumpNormal = normalize(bumpNormal);

	// Calculate the projected texture coordinates.
	float2 pTexCoord = input.lightViewPos[0].xy / input.lightViewPos[0].w;
	pTexCoord *= float2(0.5, -0.5);
	pTexCoord += float2(0.5f, 0.5f);

	// Calculate the projected texture coordinates.
	float2 pTexCoord2 = input.lightViewPos[1].xy / input.lightViewPos[1].w;
	pTexCoord2 *= float2(0.5, -0.5);
	pTexCoord2 += float2(0.5f, 0.5f);

	// Sample the shadow map (get depth of geometry)
	depthValue = shadowMap1.Sample(shadowSampler, pTexCoord).r;

	// Calculate the depth from the light.
	lightDepthValue = input.lightViewPos[0].z / input.lightViewPos[0].w;
	lightDepthValue -= shadowMapBias;

	// Sample the shadow map (get depth of geometry)
	depthValue2 = shadowMap2.Sample(shadowSampler, pTexCoord2).r;

	// Calculate the depth from the light.
	light2DepthValue = input.lightViewPos[1].z / input.lightViewPos[1].w;
	light2DepthValue -= shadowMapBias2;

	//light with ambient light(shadows)
	finalLightColour = ambientColour;

	// Determine if the projected coordinates are in the 0 to 1 range.  If not don't do lighting.
	if (pTexCoord.x > 0.f || pTexCoord.x < 1.f || pTexCoord.y > 0.f || pTexCoord.y < 1.f)
	{
		//light parts of scene correctly if they are in front in the depth buffer 
		if (lightDepthValue < depthValue)
		{
			directionalLightColour = calculateLighting(direction, bumpNormal, diffuseColour[0]);

			finalLightColour += directionalLightColour;
		}
	}

	// Determine if the projected coordinates are in the 0 to 1 range.  If not don't do lighting.
	if (pTexCoord2.x > 0.f || pTexCoord2.x < 1.f || pTexCoord2.y > 0.f || pTexCoord2.y < 1.f)
	{
		if (light2DepthValue < depthValue2)
		{
			pointLightVector = normalize(lightPosition - input.worldPosition);
			pointLightColour = calculateLighting(pointLightVector, bumpNormal, diffuseColour[1]) * calcAttenuation(lightPosition, input.worldPosition);

			finalLightColour += pointLightColour;
		}
	}


	finalLightColour = saturate(finalLightColour);
	return saturate(finalLightColour) * textureColour + calcSpecular(direction, bumpNormal, input.viewDirection, specularColour, specularPower);
}



