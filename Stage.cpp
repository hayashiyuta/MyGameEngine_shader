#include "Stage.h"
#include "Engine/Model.h"
//コンストラクタ
Stage::Stage(GameObject* parent)
	: GameObject(parent, "Stage"), hModel_(-1)
{
}

//初期化
void Stage::Initialize()
{
	//モデルデータのロード
	hModel_ = Model::Load("Assets\\BoxDefault.fbx");
	assert(hModel_ >= 0);
}

//更新
void Stage::Update()
{
	transform_.rotate_.y += 0.5f;
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