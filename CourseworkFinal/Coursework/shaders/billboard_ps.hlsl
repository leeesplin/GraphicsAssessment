Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer LightBuffer : register(b0)
{
	float4 ambientColour;
	float4 diffuseColour[2];
	float3 lightPosition;
	float padding;
	float3 direction;
	float padding2;
	float constantFactor;
	float linearFactor;
	float quadraticFactor;
	float padding3;
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 worldPosition : TEXCOORD1;
};

// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculateLighting(float3 lightVector, float3 normal, float4 diffuse)
{
	float intensity = saturate(dot(normal, lightVector));
	float4 colour = saturate(diffuse * intensity);
	return colour;
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
	// Sample the texture
	float4 textureColour = texture0.Sample(sampler0, input.tex);

	//calculate point light vector
	float3 pointLightVector = normalize(lightPosition - input.worldPosition);

	//calculate colour of lights
	float4 directionalLightColour = calculateLighting(-direction, input.normal, diffuseColour[0]);
	float4 pointLightColour = calculateLighting(pointLightVector, input.normal, diffuseColour[1]) * calcAttenuation(lightPosition, input.worldPosition);

	//calculate final light colour by adding ambient and all lights together
	float4 lightColour = ambientColour + directionalLightColour + pointLightColour;

	//return the colour of the pixel 
	return lightColour * textureColour;

}