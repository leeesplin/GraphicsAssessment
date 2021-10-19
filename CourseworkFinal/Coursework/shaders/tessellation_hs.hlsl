cbuffer TesselationBuffer : register(b0)
{
	float TesselationFactorSide1;
	float TesselationFactorSide2;
	float TesselationFactorSide3;
	float TesselationFactorInside;
}

cbuffer CameraBuffer : register(b1)
{
	float3 cameraPosition;
	float padding;
}

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

struct ConstantOutputType
{
	float edges[4] : SV_TessFactor;
	float inside[2] : SV_InsideTessFactor;
};

struct OutputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float3 worldPosition : TEXCOORD1;
	float4 colour : COLOR;
};

ConstantOutputType PatchConstantFunction(InputPatch<InputType, 4> inputPatch, uint patchId : SV_PrimitiveID)
{
	ConstantOutputType output;
	int tessFactor = 64;

	//checks the distance between the camera and the centre of the tessellated plane
	float totalDistance = 0;
	float distanceZ = 0;
	float distanceY = cameraPosition.y - inputPatch[patchId].position.y;
	float distanceX = 0;
	float thresholdToTessellate = 150;

	//calculate the centre of the mesh
	if (inputPatch[patchId].position.z == 0)
	{
		distanceZ = cameraPosition.z - (inputPatch[patchId].position.z + 50);
	}
	else if (inputPatch[patchId].position.z == 100)
	{
		distanceZ = cameraPosition.z - (inputPatch[patchId].position.z - 50);
	}

	if (inputPatch[patchId].position.x == 0)
	{
		distanceX = cameraPosition.x - (inputPatch[patchId].position.x + 50);
	}
	else if (inputPatch[patchId].position.x == 100)
	{
		distanceX = cameraPosition.x - (inputPatch[patchId].position.x - 50);
	}

	//to correct the actual distance away from the center of the quad, we must negate any negative distance
	if (distanceZ < 0)
	{
		distanceZ = distanceZ * -1;
	}
	if (distanceY < 0)
	{
		distanceY = distanceY * -1;
	}
	if (distanceX < 0)
	{
		distanceX = distanceX * -1;
	}

	//add a total together of distance from the centre point so we know when to tessellate
	totalDistance = (distanceZ + distanceY + distanceX);
	float tessDivider = totalDistance / 64;

	//only divide the quad if they are a certain distance away
	if (totalDistance >= thresholdToTessellate)
	{
		//divide the quad as the camera gets closer
		tessFactor = 64 / tessDivider;

		//correct the tessellation factor to 10 so it still displays the terrain with reasonable accuracy no matter how far away the camera is
		if (tessFactor < 10)
		{
			tessFactor = 10;
		}
	}
	else
	{
		tessFactor = 64;
	}

	// Set the tessellation factors for the 4 edges of the quad.
	output.edges[0] = tessFactor;
	output.edges[1] = tessFactor;
	output.edges[2] = tessFactor;
	output.edges[3] = tessFactor;

	// Set the tessellation factor for tessallating inside the quad
	output.inside[0] = tessFactor;
	output.inside[1] = tessFactor;

	return output;
}

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_ccw")]
[outputcontrolpoints(4)]
[patchconstantfunc("PatchConstantFunction")]
OutputType main(InputPatch<InputType, 4> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
	OutputType output;


	// Set the values for this control point as the output values.
	output.position = patch[pointId].position;	
	output.normal = patch[pointId].normal;
	output.tex = patch[pointId].tex;
	output.tangent = patch[pointId].tangent;
	output.binormal = patch[pointId].binormal;

	return output;
}