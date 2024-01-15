//───────────────────────────────────────
// テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D	g_texture : register(t0);	//テクスチャー
SamplerState	g_sampler : register(s0);	//サンプラー

Texture2D		g_toon_texture : register(t1);

//───────────────────────────────────────
// コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer global:register(b0)
{
	float4x4	matWVP;			// ワールド・ビュー・プロジェクションの合成行列
	float4x4	matW;	//ワールド行列
	float4x4	matNormal;
	float4		diffuseColor;		// ディフューズカラー（マテリアルの色）
	float4		ambientColor;
	float4		specularColor;
	bool		isTexture;		// テクスチャ貼ってあるかどうか

};

cbuffer global:register(b1)
{
	float4		lightPosition;
	float4		eyePosition;
}

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
	float4 pos    : SV_POSITION;	//位置
	float2 uv	: TEXCOORD;	//UV座標
	float4 color	: COLOR;	//色（明るさ）
	float4 eyev		:POSITION;
	float4 normal	:NORMAL;
};

/*struct PS_IN
{
	float4 pos    : SV_POSITION;	//位置
	float2 uv	: TEXCOORD;	//UV座標
	float4 eyev		:POSITION1;
	float4 normal	:POSITION2;
	float4 light	:POSITION3;
};*/

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	//ピクセルシェーダーへ渡す情報
	VS_OUT outData = (VS_OUT)0;

	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
	//スクリーン座標に変換し、ピクセルシェーダーへ
	outData.pos = mul(pos, matWVP);
	//outData.uv = uv;
	outData.uv = uv;
	normal.w = 0;
	normal = mul(normal, matNormal);
	normal = normalize(normal);
	outData.normal = normal;

	float4 light = normalize(lightPosition);
	light = normalize(light);

	outData.color = saturate(dot(normal, light));
	float4 posw = mul(pos, matW);
	outData.eyev = eyePosition - posw;


	//法線を回転
	/*normal = mul(normal, matW);

	float4 light = float4(1, 0, 0, 0);
	light = normalize(light);
	outData.color = clamp(dot(normal, light), 0, 1);*/

	//まとめて出力
	return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
	/*float4 light = float4(0.0, 2, 0, 1);//点光源の位置
	light = mul(light, matW);
	float3 LD = inData.pos_ - light;//光の方向ベクトル
	float len = length(LD);//光の方向ベクトルを正規化
	float4 outColor = { 1, 1, 1, 1 };
	float lightMagnitude = seturate(dot(inData.normal, -normalize(LD)));
	float k = seturate(1.0f / (1.0f + 1.0 * len * len));

	return outColor * (0.8 * k * lightMagnitude * 0.2f);*/

	float4 lightSource = float4(1.0f, 1.0f, 1.0f, 1.0f);//ライトの色と明るさ Iin
	float4 ambentSource = ambientColor;//環境(アンビエント係数 Ka)
	//return lightSource * g_texture.Sample(g_sampler, inData.uv) * inData.color;//float4(1,1,1,1)
	float4 diffuse;
	float4 ambient;
	float4 NL = dot(inData.normal,normalize(lightPosition));
	float4 reflect = normalize(2 * NL * inData.normal - normalize(lightPosition));
	float4 specular = pow(saturate(dot(reflect,normalize(inData.eyev))),8) * specularColor;
	//拡散反射
	float2 uv;
	uv.x = abs(dot(normalize(inData.eyev), inData.normal));
	uv.y = abs(dot(normalize(inData.eyev), inData.normal));
	//float4 nk = 0.1 * step(n1, inData.color) + 0.3 * step(n2, inData.color) + 
				//0.3 * step(n3, inData.color) + 0.4 * step(n4, inData.color);
	float4 t1 = g_toon_texture.Sample(g_sampler, uv);



	uv.x = 0.5;

	if (isTexture == 0) {
		diffuse = lightSource * diffuseColor * t1;
		ambient = lightSource * diffuseColor * ambentSource;
	}
	else {
		diffuse = lightSource * g_texture.Sample(g_sampler, inData.uv) * t1;
		ambient = lightSource * g_texture.Sample(g_sampler, inData.uv) * ambentSource;
	}
	//輪郭=視線ベクトルと面の法線の角度が90度付近

	return diffuse + ambient;
	//return t1;
}
