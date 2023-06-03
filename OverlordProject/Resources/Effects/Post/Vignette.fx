//=============================================================================
//// Shader uses position and texture
//=============================================================================
SamplerState samPoint
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Mirror;
	AddressV = Mirror;
};

Texture2D gTexture;

/// Create Depth Stencil State (ENABLE DEPTH WRITING)
DepthStencilState depthWrite
{
	DEPTHWRITEMASK = ALL;
};
/// Create Rasterizer State (Backface culling) 
RasterizerState backCull
{
	CullMode = back;
};

//IN/OUT STRUCTS
//--------------
struct VS_INPUT
{
	float3 Position : POSITION;
	float2 TexCoord : TEXCOORD0;
};

struct PS_INPUT
{
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD1;
};


//VERTEX SHADER
//-------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.Position = float4(input.Position, 1.f);
	output.TexCoord = input.TexCoord;

	return output;
}


//PIXEL SHADER
//------------
float4 PS(PS_INPUT input) : SV_Target
{
	float inner = 0.f;
	float outer = 2.f;


	//float vOffset = 0;
	//float vMul = 0.7;
	//float maxVignetFactor = 0.7f;

	float4 vignetteColor = float4(0.f, 0.f, 0.f, 1.f);
	float4 imageColor = gTexture.Sample(samPoint, input.TexCoord);

	float2 imageCoord = float2(input.TexCoord.x * 2 - 1, input.TexCoord.y * 2 - 1);


	float factor = (imageCoord.x * imageCoord.x + imageCoord.y * imageCoord.y);


	factor = saturate(lerp(0.f, 1.f, (factor - inner) / (outer - inner)));

	float3 finalColor = (float3)lerp(imageColor, vignetteColor, factor);


	return float4(finalColor, 1.f);
}


//TECHNIQUE
//---------
technique11 Blur
{
	pass P0
	{
		// Set states...
		SetRasterizerState(backCull);
		SetDepthStencilState(depthWrite, 0);
		SetVertexShader(CompileShader(vs_4_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS()));
	}
}