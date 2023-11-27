#include "Arrow.h"
#include"Engine/Model.h"
//コンストラクタ
Arrow::Arrow(GameObject* parent)
	: GameObject(parent, "Arrow"),harrow1_(-1), harrow2_(-1), harrow3_(-1)
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
	Transform ar1, ar2, ar3;
	ar1.rotate_   = { 0,0,90 };
	ar1.scale_    = { 2,1,1 };
	ar1.position_ = { 0,2,0 };

	ar2.rotate_   = { 0,0,0 };
	ar2.scale_    = { 2,1,1 };
	ar2.position_ = { 2,0.5f,0 };

	ar3.rotate_   = { 0,90,0 };
	ar3.scale_    = { 2,1,1 };
	ar3.position_ = { 0,0,-2 };

	Model::SetTransform(harrow1_, ar1);
	Model::Draw(harrow1_);
	Model::SetTransform(harrow2_, ar2);
	Model::Draw(harrow2_);
	Model::SetTransform(harrow3_, ar3);
	Model::Draw(harrow3_);
}

//開放
void Arrow::Release()
{
}