#include "Ball.h"
#include "Engine/Model.h"
//コンストラクタ
Ball::Ball(GameObject* parent)
	: GameObject(parent, "Ball"), hModel_(-1), hDice_(-1)
{
}

//初期化
void Ball::Initialize()
{
	//モデルデータのロード
	hModel_ = Model::Load("Assets\\Ball.fbx");
	hDice_ = Model::Load("Assets\\diceblue.fbx");
	assert(hModel_ >= 0);
	assert(hDice_ >= 0);
	transform_.position_.x = 1;
	transform_.position_.y = 3;
	transform_.scale_ = XMFLOAT3(4, 4, 4);
	trDice_.position_ = XMFLOAT3(1, 4, 0);
}

//更新
void Ball::Update()
{
	transform_.rotate_.y += 0.5f;
	trDice_.rotate_.y += 0.5f;
}

//描画
void Ball::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
	Model::SetTransform(hDice_, trDice_);
	Model::Draw(hDice_);
}

//開放
void Ball::Release()
{
}