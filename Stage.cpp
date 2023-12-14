#include "Stage.h"
#include "Engine/Model.h"
#include"Engine/Camera.h"
#include"Engine/Input.h"

namespace {
	const XMFLOAT4 DEF_LIGHT_POSITION{ 2, 2, 1, 0 };
}

void Stage::IntConstantBuffer()
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
}
//コンストラクタ
Stage::Stage(GameObject* parent)
	: GameObject(parent, "Stage"), hModel_(-1),lightSourcePosition_(DEF_LIGHT_POSITION)
{
}

//初期化
void Stage::Initialize()
{
	//モデルデータのロード
	//hModel_ = Model::Load("Assets\\Ground.fbx");
	hLightBall_ = Model::Load("Assets/Ball3.fbx");
	//assert(hModel_ >= 0);
	assert(hLightBall_ >= 0);
	Camera::SetPosition(XMVECTOR{ 0, 10, -20, 0 });
	Camera::SetTarget(XMVECTOR{ 0, 2, 0, 0 });
	//transform_.scale_.x = 3.0f;
	//transform_.scale_.z = 3.0f;

	trLightBall.position_ = { 0, 0, 0 };
	trLightBall.rotate_ = { 0, 0, 0 };
	trLightBall.scale_ = { 0.4, 0.4, 0.4 };
    
	
	IntConstantBuffer();
}

//更新
void Stage::Update()
{
	//transform_.rotate_.y += 0.5f;

    if (Input::IsKeyUp(DIK_SPACE))
    {
        Model::ToggleRenderState();
    }
    //transform_.rotate_.y += 0.5f;
    // trDonuts.rotate_.y += 0.5f;
    if (Input::IsKey(DIK_RIGHT))
    {
        XMFLOAT4 p = GetLightPos();
        XMFLOAT4 margin{ p.x + 0.1f, p.y + 0.0f, p.z + 0.0f, p.w + 0.0f };

        //Model::GetModel(hModel_)->SetLightPos(margin);
        SetLightPos(margin);
    }
    if (Input::IsKey(DIK_LEFT))
    {
        XMFLOAT4 p = GetLightPos();
        XMFLOAT4 margin{ p.x - 0.1f, p.y - 0.0f, p.z - 0.0f, p.w - 0.0f };

        /// Model::GetModel(hModel_)->SetLightPos(margin);
        SetLightPos(margin);
    }
    if (Input::IsKey(DIK_UP))
    {
        XMFLOAT4 p = GetLightPos();
        XMFLOAT4 margin{ p.x - 0.0f, p.y + 0.1f, p.z - 0.0f, p.w - 0.0f };

        //Model::GetModel(hModel_)->SetLightPos(margin);
        SetLightPos(margin);
    }
    if (Input::IsKey(DIK_DOWN))
    {
        XMFLOAT4 p = GetLightPos();
        XMFLOAT4 margin{ p.x - 0.0f, p.y - 0.1f, p.z - 0.0f, p.w - 0.0f };

        //Model::GetModel(hModel_)->SetLightPos(margin);
        SetLightPos(margin);
    }
    if (Input::IsKey(DIK_W))
    {
        XMFLOAT4 p = GetLightPos();
        XMFLOAT4 margin{ p.x - 0.0f, p.y - 0.0f, p.z + 0.1f, p.w + 0.0f };

        //Model::GetModel(hModel_)->SetLightPos(margin);
        SetLightPos(margin);
    }
    if (Input::IsKey(DIK_S))
    {
        XMFLOAT4 p = GetLightPos();
        XMFLOAT4 margin{ p.x - 0.0f, p.y - 0.0f, p.z - 0.1f, p.w - 0.0f };

        //Model::GetModel(hModel_)->SetLightPos(margin);
        SetLightPos(margin);
    }
    XMFLOAT4 tmp{ GetLightPos() };
    trLightBall.position_ = { tmp.x, tmp.y,tmp.z };

	CBUFF_STAGESCENE cb;
	cb.lightPosition = lightSourcePosition_;
    cb.eyePos = tmp;
	XMStoreFloat4(&cb.eyePos, Camera::GetCamPosition());

	Direct3D::pContext_->UpdateSubresource(pCBStageScene_,
		0, NULL, &cb, 0, 0);
	//コンスタントバッファ
	Direct3D::pContext_->VSSetConstantBuffers(1, 1, &pCBStageScene_);	//頂点シェーダー用	
	Direct3D::pContext_->PSSetConstantBuffers(1, 1, &pCBStageScene_);	//ピクセルシェーダー用
    
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
	//Model::SetTransform(hModel_, transform_);
	//Model::Draw(hModel_);
    Model::SetTransform(hLightBall_, trLightBall);
    Model::Draw(hLightBall_);
}

//開放
void Stage::Release()
{
}