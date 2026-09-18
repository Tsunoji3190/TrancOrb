
cbuffer ConstBuffer	: register(b0)
{
	matrix matWorld;
	matrix matView;
	matrix matProj;
	float4 diffuse;
};

struct VS_INPUT
{
	float3 Pos : POSITION;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
};