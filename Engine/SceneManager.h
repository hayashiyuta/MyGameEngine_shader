#pragma once
#include "GameObject.h"

//ゲームのシーンを列挙
enum SCENE_ID
{
	SCENE_ID_TEST = 0,
	
	
};

//■■シーンを管理するクラス
class SceneManager : public GameObject
{
	SCENE_ID currentSceneID_;
	SCENE_ID nextSceneID_;
public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	SceneManager(GameObject* parent);
	~SceneManager();
	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	void ChangeScene(SCENE_ID _next);
};
