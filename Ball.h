#pragma once
#include "Engine/GameObject.h"


//■■シーンを管理するクラス
class Ball : public GameObject
{
	int hModel_;    //モデル番号
	int hDice_;
	Transform trDice_;
public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	Ball(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
};
