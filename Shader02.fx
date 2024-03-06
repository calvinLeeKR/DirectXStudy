Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

struct VS_INPUT
{
    float4 Pos : POSITION;
    float4 Color : COLOR0;
    float2 Tex : TEXCOORD0;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR0;
    float2 Tex : TEXCOORD0;
};
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;

    output.Pos = input.Pos;
    output.Color = input.Color;
    output.Tex = input.Tex;

    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
    return txDiffuse.Sample(samLinear, input.Tex) * input.Color;    // Yellow, with Alpha = 1
}
