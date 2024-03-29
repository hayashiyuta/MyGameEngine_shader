#pragma once

#include <d3d11.h>
#include <fbxsdk.h>
#include <string>
#include<vector>
#include "Transform.h"
#include"Direct3D.h"
#include"Transform.h"
#include"Camera.h"
#include"Texture.h"

using std::vector;

#pragma comment(lib, "LibFbxSDK-MD.lib")
#pragma comment(lib, "LibXml2-MD.lib")
#pragma comment(lib, "zlib-MD.lib")
//class Texture;//ポインタならこれでOK(ヘッダをインクルードしなくていい)
enum RENDER_STATE
{
	RENDER_DIRLIGHT,
	RENDER_PNTLIGHT,
};

class Fbx
{
	//マテリアル
	struct MATERIAL
	{
		Texture* pTexture;
		XMFLOAT4	diffuse;
		XMFLOAT4	ambient;			//環境光（アンビエント）への反射強度
		XMFLOAT4	specular;			//鏡面反射光（スペキュラ）への反射強度
		float		shininess;			//ハイライトの強さ（サイズ）
	};
	struct CONSTANT_BUFFER
	{
		XMMATRIX	matWVP;//wvpワールドビュープロジェクション
		XMMATRIX    matW;//ワールド変換のみ
		XMMATRIX	matNormal;//スケール×平行移動の逆行列
		XMFLOAT4	diffuseColor;// ディフューズカラー（FBXからとってきた面の色）
		XMFLOAT4	ambientColor;
		XMFLOAT4	specularColor;
		BOOL		isTextured;		// テクスチャ貼ってあるかどうか
	};
	//XMFLOAT4	lightPosition;
	//XMFLOAT4	eyePos;

	struct VERTEX
	{
		XMVECTOR position;//
		XMVECTOR uv;//ポリゴン数
		XMVECTOR normal;//法線ベクトル
	};

	int vertexCount_;	//頂点数
	int polygonCount_;	//ポリゴン数
	int materialCount_;	//マテリアルの個数

	ID3D11Buffer* pVertexBuffer_;
	ID3D11Buffer** pIndexBuffer_;
	ID3D11Buffer* pConstantBuffer_;
	MATERIAL* pMaterialList_;
	vector<int>index_Count_;
	RENDER_STATE state_;

public:

	Fbx();
	HRESULT Load(std::string fileName);
	void    InitVertex(fbxsdk::FbxMesh* mesh);
	void    InitIndex(fbxsdk::FbxMesh* mesh);
	void    IntConstantBuffer();	//コンスタントバッファ準備
	void    InitMaterial(fbxsdk::FbxNode* pNode);
	void    Draw(Transform& transform);
	void	SetRenderingShader(RENDER_STATE _state) { state_ = _state; }
	void    Release();
};
