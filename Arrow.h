#pragma once
#include "Engine/GameObject.h"


//■■シーンを管理するクラス
class Arrow : public GameObject
{
	int harrow1_;    //モデル番号
	int harrow2_;
	int harrow3_;
public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	Arrow(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
};
