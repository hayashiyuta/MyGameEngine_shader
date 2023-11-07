#pragma once
#include <DirectXMath.h>
#include "Direct3D.h"
#include "Texture.h"
#include<vector>
using namespace DirectX;

#define SAFE_DELETE_ARRAY(p) if(p != nullptr){ delete p; p = nullptr;}

class Spirete
{
	
	//コンスタントバッファー
	struct CONSTANT_BUFFER
	{
		XMMATRIX	matW;
	};

	//頂点情報
	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
	};
	UINT64 vertexNum;//頂点数
	std::vector<VERTEX> vertices_;//頂点情報
	ID3D11Buffer* pVertexBuffer_;	//頂点バッファのアドレスを入れる

	UINT64 indexNum;//インデックス数
	std::vector<int> index_;//インデックス情報

	//メンバ変数を作ったらコンストラクタで初期化
	ID3D11Buffer* pIndexBuffer_;//インデックスバッファ
	ID3D11Buffer* pConstantBuffer_;	//コンスタントバッファ
	Texture* pTexture_;

public:
	Spirete();
	~Spirete();
	//void
	HRESULT Initialize();
	void Draw(XMMATRIX& worldMatrix);
	void Release();

private:
	
	virtual void InitVertexData(); //頂点情報の準備
	HRESULT CreateVertexBuffer();//頂点バッファを作成

	virtual void InitIndexData();//インデックス情報の準備
	HRESULT CreateIndexBuffer();//インデックスバッファを作成

	HRESULT CreateConstantBuffer();//コンスタントバッファ作成

	HRESULT LoadTexture();//テクスチャをロード


	void PassDataTcCB(DirectX::XMMATRIX& worldMatrix);//コンスタントバッファに各種情報を渡す
	void SetBufferToPipeline();//各バッファをパイプラインにセット
};


