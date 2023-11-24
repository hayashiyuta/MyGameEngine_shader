#include "Arrow.h"
#include"Engine/Model.h"
//�R���X�g���N�^
Arrow::Arrow(GameObject* parent)
	: GameObject(parent, "Arrow"),harrow1_(-1),harrow2_(-1),harrow3_(-1)
{
}

//������
void Arrow::Initialize()
{
	harrow1_ = Model::Load("Assets\\Arrow.fbx");
	assert(harrow1_ >= 0);
	harrow2_ = Model::Load("Assets\\Arrow.fbx");
	assert(harrow2_ >= 0);
	harrow3_ = Model::Load("Assets\\Arrow.fbx");
	assert(harrow3_ >= 0);
	
}

//�X�V
void Arrow::Update()
{
}

//�`��
void Arrow::Draw()
{
	Model::SetTransform(harrow1_, transform_);
	Model::Draw(harrow1_);
	Model::SetTransform(harrow2_, transform_);
	Model::Draw(harrow2_);
	Model::SetTransform(harrow3_, transform_);
	Model::Draw(harrow3_);
}

//�J��
void Arrow::Release()
{
}