#include "ShaderVariables.fx"

SamplerState linearSampling
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};


PS_INPUT_POS_TEX VS(VS_INPUT_POS_TEX input)
{
	PS_INPUT_POS_TEX output = (PS_INPUT_POS_TEX)0;
	
	output.Pos = mul(input.Pos, Projection);
	output.Pos.x -= 1;
	output.Pos.y += 1;
	output.Tex = input.Tex;

	return output;
}

float4 PS(PS_INPUT_POS_TEX input) : SV_Target
{
	float4 color = DiffuseTexture.Sample(linearSampling, input.Tex);

	return color;
}

technique11 Render
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}