#pragma once
#include "Engine/GameObject.h"


//�����V�[�����Ǘ�����N���X
class Arrow : public GameObject
{
	int harrow1_;    //���f���ԍ�
	int harrow2_;
	int harrow3_;
public:
	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iSceneManager�j
	Arrow(GameObject* parent);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;
};
