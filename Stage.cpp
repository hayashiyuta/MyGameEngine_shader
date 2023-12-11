#include "Stage.h"
#include "Engine/Model.h"
/*void Stage::IntConstantBuffer()
{
	//コンスタントバッファ作成
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CBUFF_STAGESCENE);
	cb.Usage = D3D11_USAGE_DEFAULT;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	// コンスタントバッファの作成
	Direct3D::pDevice_->CreateBuffer(&cb, nullptr, &pCBStageScene_);
}*/
//コンストラクタ
Stage::Stage(GameObject* parent)
	: GameObject(parent, "Stage"), hModel_(-1)
{
}

//初期化
void Stage::Initialize()
{
	//モデルデータのロード
	hModel_ = Model::Load("Assets\\Ground.fbx");
	assert(hModel_ >= 0);
	transform_.scale_.x = 3.0f;
	transform_.scale_.z = 3.0f;
}

//更新
void Stage::Update()
{
	//transform_.rotate_.y += 0.5f;

	//CBUFF_STAGESCENE cb;
	//cb.lightPosition = lightSourcePosition_;
	//XMStoreFloat4(&cb.eyePos, Camera::GetCamPosition());

	
	//コンスタントバッファ
	//Direct3D::pContext_->VSSetConstantBuffers(1, 1, &pCBStageScene_);	//頂点シェーダー用	
	//Direct3D::pContext_->PSSetConstantBuffers(1, 1, &pCBStageScene_);	//ピクセルシェーダー用

}

//描画
void Stage::Draw()
{
	/*for (int x = 0; x < 15; x++)
	{
		for (int z = 0; z < 15; z++)
		{
			transform_.position_.x = x;
			transform_.position_.z = z;
			
		}
	}*/
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

//開放
void Stage::Release()
{
}