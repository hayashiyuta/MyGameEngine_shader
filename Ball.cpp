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
	hModel_ = Model::Load("Assets\\Ball.fbx");
	assert(hModel_ >= 0);
	transform_.position_.y = 1;
}

//更新
void Ball::Update()
{
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