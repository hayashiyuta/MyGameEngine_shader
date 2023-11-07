#pragma once

#include "Transform.h"
#include <string>
#include<list>

#define SAFE_DELETE(p) if(p != nullptr){ delete p; p = nullptr;}
using std::string;
class SphereCollider;

class GameObject
{
private:
	struct OBJECT_STATE
	{
		unsigned initilized : 1; //èâä˙âªçœÇ©
		unsigned entered : 1;    //çXêVÇ∑ÇÈÇ©
		unsigned visible : 1;    //ï`âÊÇ∑ÇÈÇ©
		unsigned dead : 1;       //çÌèúÇ∑ÇÈÇ©
	};
	OBJECT_STATE state_;
protected:
	std::list<GameObject*> childList_;
	Transform	transform_;
	GameObject* pParent_;
	std::string	objectName_;
	SphereCollider* pCollider_;

public:
	GameObject();
	GameObject(GameObject* parent, const std::string& name);
	~GameObject();

	virtual void Initialize() =0;
	virtual void Update() =0;
	virtual void Draw() = 0;
	virtual void Release() = 0;
	virtual void OnCollision(GameObject* pTarget) {};
	void UpdateSub();
	void DrawSub();
	void ReleaseSub();
	bool IsDead();
	void KillMe();
	void SetPosition(XMFLOAT3 position_);
	GameObject* FindChildObject(std::string _objName);
	GameObject* GetRootJob();
	GameObject* FindObject(std::string _objName);
	void AddCollider(SphereCollider* pCollider);
	void Collision(GameObject* pTarget);
	void RoundRodin(GameObject* pTarget);
	std::string GetObjectName();

	template <class T>
	GameObject* Instantiate(GameObject* parent)
	{
		T* p;
		p = new T(parent);
		p->Initialize();
		childList_.push_back(p);
		return p;
	}
	
};
