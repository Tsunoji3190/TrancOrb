#include "UI.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	//	画像表示
	float4 output = tex.Sample(samLinear, input.tex);

	//	画像表示その2
    float4 output2 = tex2.Sample(samLinear, input.tex);
	
	float4 ret;
	//	画像データ1を使う。
	//	lerp関数は、第三引数を0～1で指定して、第一引数と第二引数を割合でブレンドする。
	//	つまり、第三引数が0に近いほど第一引数の値が、
	//	1に近いほど第二引数の値が、強くブレンドされる。
	//	例）第三引数が0.5fの場合は、「第一引数の値50％＋第二引数の値50％」が戻り値となる
    ret = lerp(float4(0, 0, 0, 0), output, 1.0f);
	return ret;
}