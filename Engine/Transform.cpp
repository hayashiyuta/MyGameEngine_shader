#include "Transform.h"

Transform::Transform()
	:matTranslate_(XMMatrixIdentity()), matRotate_(XMMatrixIdentity()), matScale_(XMMatrixIdentity()),
position_(XMFLOAT3(0, 0, 0)), rotate_(XMFLOAT3(0, 0, 0)), scale_(XMFLOAT3(1, 1, 1)),pParent_(nullptr)
{}

Transform::~Transform()
{

}


void Transform::Calclation()
{
	//rotate_.x += 0.04f;
	//rotate_.z += 0.04f;
	//position_.x = 0;
	matTranslate_ = XMMatrixTranslation(position_.x, position_.y, position_.z);

	matRotate_ = XMMatrixRotationX(XMConvertToRadians(rotate_.x))*
		XMMatrixRotationY(XMConvertToRadians(rotate_.y))*
		XMMatrixRotationZ(XMConvertToRadians(rotate_.z));

	matScale_ = XMMatrixScaling(scale_.x, scale_.y, scale_.z);

	
}

XMMATRIX Transform::GetWorldMatrix()
{
	if (pParent_ != nullptr)
	{
		//e‚ª‚ ‚Á‚½‚ç
		return matScale_ * matRotate_ * matTranslate_ * pParent_->GetWorldMatrix();
	}else
	return XMMatrixTranslation(0, 0, 0) * matScale_* matRotate_* matTranslate_;
}

XMMATRIX Transform::GetNormalMatrix()
{
	return matRotate_ * XMMatrixInverse(nullptr, matScale_);
}
