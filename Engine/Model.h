#pragma once
#include "GameObject.h"
#include <string>
#include <vector>
#include"Fbx.h"

//■■シーンを管理するクラス
//class Fbx;
#define SAFE_DELETE(p) if(p != nullptr){ delete p; p = nullptr;}
namespace Model
{
	struct ModelData
	{
		//FBX
		Fbx* pFbx_;
		//トランスフォーム
		Transform transfome_;
		//ファイル名
		std::string fileName_;
	};
	int Load(std::string fileName);
	void SetTransform(int hModel, Transform transfome);

	void Draw(int hModel);
	void Release();
	//モデルのポインタをぶち込んでおくベクタ
}; 
