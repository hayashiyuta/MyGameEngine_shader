#include "Stage.h"
#include "Engine/Model.h"
//�R���X�g���N�^
Stage::Stage(GameObject* parent)
	: GameObject(parent, "Stage"), hModel_(-1)
{
}

//������
void Stage::Initialize()
{
	//���f���f�[�^�̃��[�h
	hModel_ = Model::Load("Assets\\BoxDefault.fbx");
	assert(hModel_ >= 0);
}

//�X�V
void Stage::Update()
{
	transform_.rotate_.y += 0.5f;
}

//�`��
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

//�J��
void Stage::Release()
{
}