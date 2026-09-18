#include "Particle.hlsli"

static const int vnum = 4;

static const float4 offset_array[vnum] =
{
	float4(-1.0f,  1.0f, 0.0f, 0.0f),	//	左上
	float4( 1.0f,  1.0f, 0.0f, 0.0f),	//	右上
	float4(-1.0f, -1.0f, 0.0f, 0.0f),	//	左下
	float4(1.0f, -1.0f, 0.0f, 0.0f),	//	右下

};

[maxvertexcount(vnum)]
void main(
	point PS_INPUT input[1],
	inout TriangleStream< PS_INPUT > output
)
{

	for (int i = 0; i < vnum; i++)
	{
		PS_INPUT element;

		float4 res = offset_array[i];
        res *= 0.5f;
		

        //element.Pos = mul(input[0].Pos, matWorld) + mul(res, matWorld);

		element.Pos = input[0].Pos + mul(res, matWorld);

		element.Pos = mul(element.Pos, matView);
		element.Pos = mul(element.Pos, matProj);

		
		element.Tex.x =  offset_array[i].x + 1.0f;
		element.Tex.y = -offset_array[i].y + 1.0f;
		element.Tex /= 2.0f;
		
		output.Append(element);
	}
	output.RestartStrip();
}