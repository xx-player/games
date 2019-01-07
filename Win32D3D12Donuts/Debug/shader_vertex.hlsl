//*********************************************************
//
//  顶点着色文件
//                     
//                       湖北·恩施 xx_player 2019
//*********************************************************

struct VSInput
{
    float3 position  : POSITION;
    float3 normal    : NORMAL;
    float2 uv        : TEXCOORD0;
    float3 tangent   : TANGENT;
};

struct PSInput
{
    float4 position  : SV_POSITION;
    float2 uv        : TEXCOORD0;
};

cbuffer cb0 : register(b0)
{
    float4x4 g_mWorldViewProj;
	float4x4 g_mTextureTranslation;
};

PSInput VSMain(VSInput input)
{
    PSInput result;
    result.position = mul(float4(input.position, 1.0f), g_mWorldViewProj);
    result.uv = mul(float4(input.uv, 0.0f, 1.0f), g_mTextureTranslation).xy;
    return result;
}
