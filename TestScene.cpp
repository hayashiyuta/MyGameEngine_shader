#include "TestScene.h"
#include"Engine/Input.h"
#include "Engine/SceneManager.h"
#include"Stage.h"
#include "Ball.h"
#include "Arrow.h"
//コンストラクタ
TestScene::TestScene(GameObject* parent)
	: GameObject(parent, "TestScene")
{
}

//初期化
void TestScene::Initialize()
{
	Instantiate<Stage>(this);
	Instantiate<Ball>(this);
	Instantiate<Arrow>(this);
}

//更新
void TestScene::Update()
{
	
}

//描画
void TestScene::Draw()
{
}

//開放
void TestScene::Release()
{
}