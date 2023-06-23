#include "SphereObject.h"


bool SphereObject::Initialize(ID3D11DeviceContext* deviceContext_)
{
	this->deviceContext = deviceContext_;

	this->shape = shape->CreateSphere(deviceContext_);

	this->SetPosition(0.0f, 0.0f, 0.0f);
	this->SetRotation(0.0f, 0.0f, 0.0f);
	this->UpdateWorldMatrix();
	return true;
}

void SphereObject::Draw(const XMMATRIX& viewMatrix, const XMMATRIX& ProjectionMatrix, const XMVECTOR& color)
{
	shape->Draw(scaleMatrix * worldMatrix, viewMatrix, ProjectionMatrix, color);
}



void SphereObject::UpdateWorldMatrix()
{
	this->worldMatrix = XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z) * XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z);
	XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, this->rot.y, 0.0f);
	this->vecForward = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	this->vecBackward = XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
	this->vecLeft = XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotationMatrix);
	this->vecRight = XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
}

const XMVECTOR& SphereObject::GetPositionVector() const
{
	return this->posVector;
}

const XMFLOAT3& SphereObject::GetPositionFloat3() const
{
	return this->pos;
}

const XMVECTOR& SphereObject::GetRotationVector() const
{
	return this->rotVector;
}

const XMFLOAT3& SphereObject::GetRotationFloat3() const
{
	return this->rot;
}

void SphereObject::SetScale(float scale)
{
	this->scaleMatrix = XMMatrixScaling(scale, scale, scale);
}

void SphereObject::SetScaleX(float scale)
{
	this->scaleMatrix = XMMatrixScaling(scale, 1.0f, 1.0f);
}

void SphereObject::SetScaleY(float scale)
{
	this->scaleMatrix = XMMatrixScaling(1.0f, scale, 1.0f);
}

void SphereObject::SetScaleZ(float scale)
{
	this->scaleMatrix = XMMatrixScaling(1.0f, 1.0f, scale);
}

void SphereObject::SetPosition(const XMVECTOR& pos_)
{
	XMStoreFloat3(&this->pos, pos_);
	this->posVector = pos_;
	this->UpdateWorldMatrix();
}

void SphereObject::SetPosition(const XMFLOAT3& pos_)
{
	this->pos = pos_;
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateWorldMatrix();
}

void SphereObject::SetPosition(float x, float y, float z)
{
	this->pos = XMFLOAT3(x, y, z);
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateWorldMatrix();
}

void SphereObject::AdjustPosition(const XMVECTOR& pos_)
{
	this->posVector += pos_;
	XMStoreFloat3(&this->pos, this->posVector);
	this->UpdateWorldMatrix();
}

void SphereObject::AdjustPosition(const XMFLOAT3& pos_)
{
	this->pos.x += pos_.y;
	this->pos.y += pos_.y;
	this->pos.z += pos_.z;
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateWorldMatrix();
}

void SphereObject::AdjustPosition(float x, float y, float z)
{
	this->pos.x += x;
	this->pos.y += y;
	this->pos.z += z;
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateWorldMatrix();
}

void SphereObject::SetRotation(const XMVECTOR& rot_)
{
	this->rotVector = rot_;
	XMStoreFloat3(&this->rot, rot_);
	this->UpdateWorldMatrix();
}

void SphereObject::SetRotation(const XMFLOAT3& rot_)
{
	this->rot = rot_;
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}

void SphereObject::SetRotation(float x, float y, float z)
{
	this->rot = XMFLOAT3(x, y, z);
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}

void SphereObject::AdjustRotation(const XMVECTOR& rot_)
{
	this->rotVector += rot_;
	XMStoreFloat3(&this->rot, this->rotVector);
	this->UpdateWorldMatrix();
}

void SphereObject::AdjustRotation(const XMFLOAT3& rot_)
{
	this->rot.x += rot_.x;
	this->rot.y += rot_.y;
	this->rot.z += rot_.z;
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}

void SphereObject::AdjustRotation(float x, float y, float z)
{
	this->rot.x += x;
	this->rot.y += y;
	this->rot.z += z;
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}

void SphereObject::SetLookAtPos(XMFLOAT3 lookAtPos)
{
	//Verify that look at pos is not the same as cam pos. They cannot be the same as that wouldn't make sense and would result in undefined behavior.
	if (lookAtPos.x == this->pos.x && lookAtPos.y == this->pos.y && lookAtPos.z == this->pos.z)
		return;

	lookAtPos.x = this->pos.x - lookAtPos.x;
	lookAtPos.y = this->pos.y - lookAtPos.y;
	lookAtPos.z = this->pos.z - lookAtPos.z;

	float pitch = 0.0f;
	if (lookAtPos.y != 0.0f)
	{
		const float distance = sqrtf(lookAtPos.x * lookAtPos.x + lookAtPos.z * lookAtPos.z);
		pitch = atanf(lookAtPos.y / distance);
	}

	float yaw = 0.0f;
	if (lookAtPos.x != 0.0f)
	{
		yaw = atanf(lookAtPos.x / lookAtPos.z);
	}
	if (lookAtPos.z > 0)
		yaw += XM_PI;

	this->SetRotation(pitch, yaw, 0.0f);
}

const XMVECTOR& SphereObject::GetForwardVector()
{
	return this->vecForward;
}

const XMVECTOR& SphereObject::GetRightVector()
{
	return this->vecRight;
}

const XMVECTOR& SphereObject::GetBackwardVector()
{
	return this->vecBackward;
}

const XMVECTOR& SphereObject::GetLeftVector()
{
	return this->vecLeft;
}