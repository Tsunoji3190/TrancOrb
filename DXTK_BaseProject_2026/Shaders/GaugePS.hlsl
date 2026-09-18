#include "Gauge.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	//	指定された画像の表示
	float4 output = tex.Sample(samLinear, input.tex);

	//	真っ白な板ポリゴン
	float4 outputw = float4(1, 1, 1, 1);

	//	ゲージの現在値から、メーター用画像が丁度いい位置でαグラデーションとなるには、
	//	どのような値を「smoothValue」に代入すればいいだろう？
	//	ヒント：一つの値を代入すればOK。計算式まで書く必要性なし！
    float smoothValue = input.color.x;

	//output.a *= 1 - step(smoothValue,input.tex.x);

	//output.a *= lerp(1.0f, 0.5f, step(smoothValue,input.tex.x) );
	
	//if(smoothValue < input.tex.x)
	//{
	//	output.a *= 1.0f;
	//}
	//else
	//{
	//	output.a *= 0.5f;
	//}

	//output.rgb *= lerp( float3(1,1,1), float3(0,0,1), step(smoothValue,input.tex.x) );

	//output.a *=1 - smoothstep(smoothValue,smoothValue, input.tex.x);
    output.a *= 1 - smoothstep(smoothValue-0.2f, smoothValue+0.2f, input.tex.x);
	
	return output;
}