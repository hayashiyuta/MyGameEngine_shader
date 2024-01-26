//───────────────────────────────────────
// テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D	g_texture : register(t0);	//テクスチャー
SamplerState	g_sampler : register(s0);	//サンプラー

Texture2D		normalTex: register(t1);

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
	float		shininess;
	int			hasTexture;		// テクスチャ貼ってあるかどうか
	int			hasNormalMap;

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
	float4 pos		: SV_POSITION;	//位置
	float2 uv		: TEXCOORD;	//UV座標
	float4 eyev		: POSITION;
	float4 Neyev	: POSITION1;
	float4 normal	: NORMAL;
	float4 light	: POSITION2;
	float4 color	: COLOR;
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
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL, float4 tangent : TANGENT)
{
	//ピクセルシェーダーへ渡す情報
	VS_OUT outData = (VS_OUT)0;

	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
	//スクリーン座標に変換し、ピクセルシェーダーへ
	outData.pos = mul(pos, matWVP);
	outData.uv = (float2)uv;

	float3 tmp = cross(tangent, normal);
	float4 binormal = {tmp, 0};
	binormal = mul(binormal, matNormal);
	binormal = normalize(binormal);//従法線ベクトルをローカル座標に変換

	normal.w = 0;
	/*normal = mul(normal, matNormal);
	normal = normalize(normal);//法線ベクトルをローカル座標に変換
	outData.normal = normal;*/
	outData.normal = normalize(mul(normal, matNormal));

	tangent = mul(tangent, matNormal);
	tangent.w = 0;
	tangent = normalize(tangent);//接線ベクトルをローカル座標に変換

	//binormal = mul(binormal, matNormal);
	//binormal = normalize(binormal);

	float4 posw = mul(pos, matW);
	float4 eye = normalize(posw - eyePosition);//ワールド座標の視線ベクトル

	outData.Neyev.x = dot(eye, tangent);//接線間の視線ベクトル
	outData.Neyev.y = dot(eye, binormal);
	outData.Neyev.z = dot(eye, normal);
	outData.Neyev.w = 0;

	float4 light = normalize(lightPosition);
	light.w = 0;
	light = normalize(light);

	outData.color = mul(light, outData.normal);
	outData.color.w = 0.0;

	outData.light.x = dot(light, tangent);//接線間の光源ベクトル
	outData.light.y = dot(light, binormal);
	outData.light.z = dot(light, normal);
	outData.light.w = 0;
	
	//まとめて出力
	return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{

	float4 lightSource = float4(1.0f, 1.0f, 1.0f, 1.0f);//ライトの色と明るさ Iin
	//float4 ambentSource = ambientColor;//環境(アンビエント係数 Ka)
	//return lightSource * g_texture.Sample(g_sampler, inData.uv) * inData.color;//float4(1,1,1,1)
	float4 diffuse;
	float4 ambient;
	float4 specular;
	if (hasNormalMap)
	{
		//inData.light = normalize(inData.light);
		float4 tmpNormal = normalTex.Sample(g_sampler, inData.uv) * 2.0f - 1.0f;
		tmpNormal = normalize(tmpNormal);
		tmpNormal.w = 0;
		
		float4 NL = clamp(dot(normalize(inData.light), tmpNormal), 0, 1);
		float4 reflection = reflect(normalize(inData.light), tmpNormal);
		float4 specular = pow(dot(normalize(reflection), inData.Neyev), shininess) * specularColor;

		if (hasTexture != 0)
		{
			diffuse = lightSource * g_texture.Sample(g_sampler, inData.uv) * NL;
			ambient = lightSource * g_texture.Sample(g_sampler, inData.uv) * ambientColor;
		}
		else
		{
			diffuse = lightSource * diffuseColor * NL;
			ambient = lightSource * diffuseColor * ambientColor;
		}
		float4 result = diffuse + ambient + specular;
		return   result;
	}
	else
	{
		float4 reflection = reflect(normalize(lightPosition), inData.normal);
		float4 specular = pow(saturate(dot(reflection, inData.eyev)), shininess) * specularColor;

		if (hasTexture == 0)
		{
			diffuse = lightSource * diffuseColor * inData.color;
			ambient = lightSource * diffuseColor * ambientColor;
		}
		else
		{
			diffuse = lightSource * g_texture.Sample(g_sampler, inData.uv) * inData.color;
			ambient = lightSource * g_texture.Sample(g_sampler, inData.uv) * ambientColor;
		}
		float4 result = diffuse + ambient + specular;
		if (hasTexture)
			result.a = inData.uv.x;
		return result;
	}
}
