#include "Stage.h"
#include "Engine/Model.h"
/*void Stage::IntConstantBuffer()
{
	//�R���X�^���g�o�b�t�@�쐬
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CBUFF_STAGESCENE);
	cb.Usage = D3D11_USAGE_DEFAULT;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	// �R���X�^���g�o�b�t�@�̍쐬
	Direct3D::pDevice_->CreateBuffer(&cb, nullptr, &pCBStageScene_);
}*/
//�R���X�g���N�^
Stage::Stage(GameObject* parent)
	: GameObject(parent, "Stage"), hModel_(-1)
{
}

//������
void Stage::Initialize()
{
	//���f���f�[�^�̃��[�h
	hModel_ = Model::Load("Assets\\Ground.fbx");
	assert(hModel_ >= 0);
	transform_.scale_.x = 3.0f;
	transform_.scale_.z = 3.0f;
}

//�X�V
void Stage::Update()
{
	//transform_.rotate_.y += 0.5f;

	//CBUFF_STAGESCENE cb;
	//cb.lightPosition = lightSourcePosition_;
	//XMStoreFloat4(&cb.eyePos, Camera::GetCamPosition());

	
	//�R���X�^���g�o�b�t�@
	//Direct3D::pContext_->VSSetConstantBuffers(1, 1, &pCBStageScene_);	//���_�V�F�[�_�[�p	
	//Direct3D::pContext_->PSSetConstantBuffers(1, 1, &pCBStageScene_);	//�s�N�Z���V�F�[�_�[�p

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