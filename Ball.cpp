#include "Ball.h"
#include "Engine/Model.h"
//コンストラクタ
Ball::Ball(GameObject* parent)
	: GameObject(parent, "Ball"), hModel_(-1)
{
}

//初期化
void Ball::Initialize()
{
	//モデルデータのロード
	hModel_ = Model::Load("Assets\\Ball4.fbx");
	assert(hModel_ >= 0);
	transform_.position_.x = 1;
	transform_.position_.y = 1;
	
	//transform_.scale_ = { 3,3,3 };
}

//更新
void Ball::Update()
{
	transform_.rotate_.y += 0.5f;
}

//描画
void Ball::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

//開放
void Ball::Release()
{
}