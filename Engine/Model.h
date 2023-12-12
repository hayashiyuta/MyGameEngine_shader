#pragma once
#include "GameObject.h"
#include <string>
#include <vector>
#include"Fbx.h"

//�����V�[�����Ǘ�����N���X
//class Fbx;
#define SAFE_DELETE(p) if(p != nullptr){ delete p; p = nullptr;}
namespace Model
{
	struct ModelData
	{
		//FBX
		Fbx* pFbx_;
		//�g�����X�t�H�[��
		Transform transfome_;
		//�t�@�C����
		std::string fileName_;
	};
	int Load(std::string fileName);
	void SetTransform(int hModel, Transform transfome);
	Fbx* GetModel(int _hModel);
	void Draw(int hModel);
	void Release();
	void ToggleRenderState();
	//���f���̃|�C���^���Ԃ�����ł����x�N�^
}; 
