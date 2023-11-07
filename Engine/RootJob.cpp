#include "RootJob.h"
//#include "../PlayScene.h"
#include"SceneManager.h"
RootJob::RootJob(GameObject* parent)
	:GameObject(parent, "RootJob")
{
}

RootJob::~RootJob()
{
}

void RootJob::Initialize()
{
	/*PlayScene* pPlayScene;
	pPlayScene = new PlayScene(this);

	pPlayScene->Initialize();
	childList_.push_back(pPlayScene);*/
	//Instantiate<PlayScene>(this);
	Instantiate< SceneManager>(this);
}

void RootJob::Update()
{

}

void RootJob::Draw()
{
	//for (auto& e : childList_)
		//e->Draw();//‘S•”‚½‚Ç‚é
}

void RootJob::Release()
{
}
