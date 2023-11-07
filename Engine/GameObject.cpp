#include "GameObject.h"
#include"SphereCollider.h"
GameObject::GameObject()
{

}

GameObject::GameObject(GameObject* parent, const std::string& name):childList_(0), pParent_(parent), objectName_(name), pCollider_(nullptr)
{
	state_ = { 0, 1, 1, 0 };
	if (parent != nullptr)
		this->transform_.pParent_ = &(parent->transform_);
}

GameObject::~GameObject()
{

}
//削除するかどうか
bool GameObject::IsDead()
{
	return (state_.dead != 0);
}

//自分を削除する
void GameObject::KillMe()
{
	state_.dead = 1;
}

void GameObject::UpdateSub()
{
	Update();
	RoundRodin(GetRootJob());
	for (auto itr = childList_.begin(); itr != childList_.end(); itr++)//すべての子供のUpdateSubを呼ぶ
	{
		(*itr)->UpdateSub();
	}
	for (auto it = childList_.begin(); it != childList_.end();)
	{
		if ((*it)->IsDead() == true)
		{
			(*it)->ReleaseSub();
			SAFE_DELETE(*it);
			it = childList_.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void GameObject::DrawSub()
{
	Draw();
	for (auto itr = childList_.begin(); itr != childList_.end(); itr++)//すべての子供のDrawSubを呼ぶ
	{
		(*itr)->DrawSub();
	}
}

void GameObject::ReleaseSub()
{
	
	for (auto itr = childList_.begin(); itr != childList_.end(); itr++)//すべての子供のReleaseSubを呼ぶ
	{
		
		(*itr)->ReleaseSub();//*itrのリリースを呼ぶ
		SAFE_DELETE(*itr);//*itrを消す
	}
	Release();
}

void GameObject::SetPosition(XMFLOAT3 position_)
{
	transform_.position_ = position_;

}

GameObject* GameObject::FindChildObject(std::string _objName)
{
	if (_objName == this->objectName_)
	{
		return(this);//自分が_objNameのオブジェクトだったら自分のポインタを返す
	}
	else
	{
		for (auto e:childList_)
		{
			GameObject* obj = e->FindChildObject(_objName);
			if (obj != nullptr)
				return obj;
		}
	}
	return nullptr;
}
/// <summary>
/// 再起呼び出しでRootJobを探してそのアドレスを返す関数
/// </summary>
/// <returns>戻り値はRootJobのアドレス(GameObject* 型)</returns>
GameObject* GameObject::GetRootJob()
{
	if (pParent_ == nullptr)
		return this;

	return pParent_->GetRootJob();

}

GameObject* GameObject::FindObject(std::string _objName)
{
	return GetRootJob()->FindChildObject(_objName);
}

void GameObject::AddCollider(SphereCollider* pCollider)
{
	pCollider_ = pCollider;
}

void GameObject::Collision(GameObject* pTarget)
{
	if (pTarget == this ||pTarget->pCollider_ == nullptr)
		return;//自分自身、又はターゲットにコライダーがアタッチされていない

	//自分とターゲットの距離 <= R1+R2なら
	/*XMVECTOR v{transform_.position_.x - pTarget->transform_.position_.x,
	           transform_.position_.y - pTarget->transform_.position_.y,
	           transform_.position_.z - pTarget->transform_.position_.z ,0};
	XMVECTOR dist = XMVector3Dot(v, v);*/
	float dist = (transform_.position_.x - pTarget->transform_.position_.x) * (transform_.position_.x - pTarget->transform_.position_.x)
		+ (transform_.position_.y - pTarget->transform_.position_.y) * (transform_.position_.y - pTarget->transform_.position_.y)
		+ (transform_.position_.z - pTarget->transform_.position_.z) * (transform_.position_.z - pTarget->transform_.position_.z);
	float rDist = (this->pCollider_->GetRadius() + pTarget->pCollider_->GetRadius()) + (this->pCollider_->GetRadius() + pTarget->pCollider_->GetRadius());
	//もし自分とターゲットがぶつかっていたら
	if (dist <= rDist)
	{
		//onCollosion(pTarget)を呼び出す
		OnCollision(pTarget);
	}
	
	
	
}

void GameObject::RoundRodin(GameObject* pTarget)
{
	if (pCollider_ == nullptr)
		return;
	if (pTarget->pCollider_ != nullptr)//自分とターゲット
		Collision(pTarget);
	//自分の子供全部とターゲット
	for (auto itr : pTarget->childList_)
		RoundRodin(itr);
}

string GameObject::GetObjectName()
{
	return objectName_;
}

