#include "SceneManager.h"
#include"../TestScene.h"
#include"Model.h"
#include"Input.h"
//コンストラクタ
SceneManager::SceneManager(GameObject* parent)
	: GameObject(parent, "SceneManager")
{

}

SceneManager::~SceneManager()
{
}

//初期化
void SceneManager::Initialize()
{
	currentSceneID_ = SCENE_ID_TEST;
	nextSceneID_ = currentSceneID_;
	Instantiate< TestScene>(this);
	
}

//更新
void SceneManager::Update()
{
	//シーンを切り替える
	//現在のシーンと、ネクストシーンが別だったら切り替え
	
	if (currentSceneID_ != nextSceneID_)
	{
		//そのシーンのオブジェクトを全削除
		auto scene = childList_.begin();
		(*scene)->ReleaseSub();
		SAFE_DELETE(*scene);
		childList_.clear();
		//ロードした全データを全削除
		Model::Release();
		//後片付け完了
		switch (nextSceneID_)
		{
		case SCENE_ID_TEST:
			Instantiate<TestScene>(this);
			break;
		
		}
		currentSceneID_ = nextSceneID_;
	}
	//現在のシーンの跡片付け
	//新しいシーンの準備
}

//描画
void SceneManager::Draw()
{
}

//開放
void SceneManager::Release()
{
}
/// <summary>
/// 指定したシーンを
/// </summary>
/// <param name="_next"></param>
void SceneManager::ChangeScene(SCENE_ID _next)
{
	nextSceneID_ = _next;
	
}