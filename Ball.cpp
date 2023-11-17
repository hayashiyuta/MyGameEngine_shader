#include "Ball.h"
#include "Engine/Model.h"
//�R���X�g���N�^
Ball::Ball(GameObject* parent)
	: GameObject(parent, "Ball"), hModel_(-1)
{
}

//������
void Ball::Initialize()
{
	//���f���f�[�^�̃��[�h
	hModel_ = Model::Load("Assets\\Ball.fbx");
	assert(hModel_ >= 0);
	transform_.position_.y = 1;
}

//�X�V
void Ball::Update()
{
}

//�`��
void Ball::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

//�J��
void Ball::Release()
{
}