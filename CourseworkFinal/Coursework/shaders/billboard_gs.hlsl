cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer PositionBuffer
{
	//index that uses positions to create a quad around a vertex, offsetting the new vertex by a position + original vertex position
	static float3 g_positions[4] =
	{
		float3(-4,8,0),
		float3(-4,-4,0),
		float3(4,8,0),
		float3(4,-4,0)
	};
};

cbuffer TexCoordBuffer
{
	static float2 g_texCoords[4] =
	{
		float2(0,0),
		float2(0,1),
		float2(1,0),
		float2(1,1)
	};
};


struct InputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct OutputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 worldPosition : TEXCOORD1;
};

[maxvertexcount(4)]
void main(point InputType input[1], inout TriangleStream<OutputType> triStream)
{
	OutputType output;

	//create a quad with texture coords around each vertex
	for (int i = 0; i < 4; i++)
	{
		float4 vposition = float4(g_positions[i], 1.0f) + input[0].position;
		output.position = mul(vposition, worldMatrix);
		output.position = mul(output.position, viewMatrix);
		output.position = mul(output.position, projectionMatrix);		
		output.tex = g_texCoords[i];
		output.normal = input[0].normal;
		output.worldPosition = mul(vposition, worldMatrix).xyz;
		triStream.Append(output);
	}

    triStream.RestartStrip();
}