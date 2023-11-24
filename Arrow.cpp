#include "Arrow.h"
#include"Engine/Model.h"
//コンストラクタ
Arrow::Arrow(GameObject* parent)
	: GameObject(parent, "Arrow"),harrow1_(-1),harrow2_(-1),harrow3_(-1)
{
}

//初期化
void Arrow::Initialize()
{
	harrow1_ = Model::Load("Assets\\Arrow.fbx");
	assert(harrow1_ >= 0);
	harrow2_ = Model::Load("Assets\\Arrow.fbx");
	assert(harrow2_ >= 0);
	harrow3_ = Model::Load("Assets\\Arrow.fbx");
	assert(harrow3_ >= 0);
	
}

//更新
void Arrow::Update()
{
}

//描画
void Arrow::Draw()
{
	Model::SetTransform(harrow1_, transform_);
	Model::Draw(harrow1_);
	Model::SetTransform(harrow2_, transform_);
	Model::Draw(harrow2_);
	Model::SetTransform(harrow3_, transform_);
	Model::Draw(harrow3_);
}

//開放
void Arrow::Release()
{
}