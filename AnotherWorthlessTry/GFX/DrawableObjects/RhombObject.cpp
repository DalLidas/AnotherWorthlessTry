#include "RhombObject.h"

bool RhombObject::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertexshader>& cbVsVertexshader)
{
	this->device = device;
	this->deviceContext = deviceContext;
	this->cbVsVertexshader = &cbVsVertexshader;

	
	//Rhomb
	Vertex v[] =
	{
		Vertex(1.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f), // topX - [0]
		Vertex(0.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f), // topY - [1]
		Vertex(0.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f), // topZ - [2]
		Vertex(0.0f,  -1.0f, 0.0f, 1.0f, 1.0f, 0.0f),// botY - [3]
		Vertex(0.0f,  0.0f, -1.0f, 1.0f, 0.0f, 1.0f), // botZ - [4]
		Vertex(-1.0f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f), // botX - [5]
	};

	//Load Vertex Data
	HRESULT hr = this->vertexBuffer.Initialize(this->device, v, ARRAYSIZE(v));

	DWORD indices[] =
	{
		0, 1, 2, //octant 1
		3, 0, 2, //octant 2
		3, 4, 0, //octant 3
		4, 1, 0, //octant 4
		1, 5, 2, //octant 5
		5, 3, 2, //octant 6
		3, 5, 4, //octant 7
		4, 5, 1, //octant 8
	};

	//Load Index Data
	hr = this->indexBuffer.Initialize(this->device, indices, ARRAYSIZE(indices));

	

	this->SetPosition(0.0f, 0.0f, 0.0f);
	this->SetRotation(0.0f, XM_PIDIV2, 0.0f);
	this->UpdateWorldMatrix();
	return true;
}

void RhombObject::Draw(const XMMATRIX& viewProjectionMatrix)
{
	//Update Constant buffer with WVP Matrix
	this->cbVsVertexshader->data.mat = this->worldMatrix * viewProjectionMatrix; //Calculate World-View-Projection Matrix
	this->cbVsVertexshader->data.mat = XMMatrixTranspose(this->cbVsVertexshader->data.mat);
	this->cbVsVertexshader->ApplyChanges();
	this->deviceContext->VSSetConstantBuffers(0, 1, this->cbVsVertexshader->GetAddressOf());


	this->deviceContext->IASetIndexBuffer(this->indexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	
	UINT offset = 0;

	this->deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), vertexBuffer.StridePtr(), &offset);
	this->deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	this->deviceContext->DrawIndexed(indexBuffer.BufferSize(), 0, 0);

}

void RhombObject::UpdateWorldMatrix()
{
	this->worldMatrix = XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z) * XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z);
	XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, this->rot.y, 0.0f);
	this->vecForward = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	this->vecBackward = XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
	this->vecLeft = XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotationMatrix);
	this->vecRight = XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
}

const XMVECTOR& RhombObject::GetPositionVector() const
{
	return this->posVector;
}

const XMFLOAT3& RhombObject::GetPositionFloat3() const
{
	return this->pos;
}

const XMVECTOR& RhombObject::GetRotationVector() const
{
	return this->rotVector;
}

const XMFLOAT3& RhombObject::GetRotationFloat3() const
{
	return this->rot;
}

void RhombObject::SetPosition(const XMVECTOR& pos)
{
	XMStoreFloat3(&this->pos, pos);
	this->posVector = pos;
	this->UpdateWorldMatrix();
}

void RhombObject::SetPosition(const XMFLOAT3& pos)
{
	this->pos = pos;
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateWorldMatrix();
}

void RhombObject::SetPosition(float x, float y, float z)
{
	this->pos = XMFLOAT3(x, y, z);
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateWorldMatrix();
}

void RhombObject::AdjustPosition(const XMVECTOR& pos)
{
	this->posVector += pos;
	XMStoreFloat3(&this->pos, this->posVector);
	this->UpdateWorldMatrix();
}

void RhombObject::AdjustPosition(const XMFLOAT3& pos)
{
	this->pos.x += pos.y;
	this->pos.y += pos.y;
	this->pos.z += pos.z;
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateWorldMatrix();
}

void RhombObject::AdjustPosition(float x, float y, float z)
{
	this->pos.x += x;
	this->pos.y += y;
	this->pos.z += z;
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateWorldMatrix();
}

void RhombObject::SetRotation(const XMVECTOR& rot)
{
	this->rotVector = rot;
	XMStoreFloat3(&this->rot, rot);
	this->UpdateWorldMatrix();
}

void RhombObject::SetRotation(const XMFLOAT3& rot)
{
	this->rot = rot;
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}

void RhombObject::SetRotation(float x, float y, float z)
{
	this->rot = XMFLOAT3(x, y, z);
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}

void RhombObject::AdjustRotation(const XMVECTOR& rot)
{
	this->rotVector += rot;
	XMStoreFloat3(&this->rot, this->rotVector);
	this->UpdateWorldMatrix();
}

void RhombObject::AdjustRotation(const XMFLOAT3& rot)
{
	this->rot.x += rot.x;
	this->rot.y += rot.y;
	this->rot.z += rot.z;
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}

void RhombObject::AdjustRotation(float x, float y, float z)
{
	this->rot.x += x;
	this->rot.y += y;
	this->rot.z += z;
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}

void RhombObject::SetLookAtPos(XMFLOAT3 lookAtPos)
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

const XMVECTOR& RhombObject::GetForwardVector()
{
	return this->vecForward;
}

const XMVECTOR& RhombObject::GetRightVector()
{
	return this->vecRight;
}

const XMVECTOR& RhombObject::GetBackwardVector()
{
	return this->vecBackward;
}

const XMVECTOR& RhombObject::GetLeftVector()
{
	return this->vecLeft;
}