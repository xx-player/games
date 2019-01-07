/*********************************************************
*                                                        *
*  像素着色文件                                          *
*                                                        *
*                       湖北·恩施 xx_player 2019        *
**********************************************************/
struct PSInput
{
    float4 position  : SV_POSITION;
    float2 uv        : TEXCOORD0;
};

Texture2D       g_txDiffuse    : register(t0);
SamplerState    g_sampler      : register(s0);

float4 PSMain(PSInput input) : SV_TARGET
{
	float4 color =g_txDiffuse.Sample(g_sampler, input.uv);
	clip(color[3]-0.3f);
	return color;
}