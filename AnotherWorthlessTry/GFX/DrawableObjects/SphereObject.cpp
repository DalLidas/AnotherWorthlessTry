#include "SphereObject.h"

bool SphereObject::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertexshader>& cbVsVertexshader)
{
	this->device = device;
	this->deviceContext = deviceContext;
	this->cbVsVertexshader = &cbVsVertexshader;

	/*try
	{*/
		//Textured Square
		Vertex v[] =
		{
			Vertex(-0.5f,  -0.5f, -0.5f, 0.0f, 1.0f), //FRONT Bottom Left   - [0]
			Vertex(-0.5f,   0.5f, -0.5f, 0.0f, 0.0f), //FRONT Top Left      - [1]
			Vertex(0.5f,   0.5f, -0.5f, 1.0f, 0.0f), //FRONT Top Right     - [2]
			Vertex(0.5f,  -0.5f, -0.5f, 1.0f, 1.0f), //FRONT Bottom Right   - [3]
			Vertex(-0.5f,  -0.5f, 0.5f, 0.0f, 1.0f), //BACK Bottom Left   - [4]
			Vertex(-0.5f,   0.5f, 0.5f, 0.0f, 0.0f), //BACK Top Left      - [5]
			Vertex(0.5f,   0.5f, 0.5f, 1.0f, 0.0f), //BACK Top Right     - [6]
			Vertex(0.5f,  -0.5f, 0.5f, 1.0f, 1.0f), //BACK Bottom Right   - [7]
		};

		//Load Vertex Data
		HRESULT hr = this->vertexBuffer.Initialize(this->device, v, ARRAYSIZE(v));

		//COM_ERROR_IF_FAILED(hr, "Failed to initialize vertex buffer.");

		DWORD indices[] =
		{
			0, 1, 2, //FRONT
			0, 2, 3, //FRONT
			4, 7, 6, //BACK 
			4, 6, 5, //BACK
			3, 2, 6, //RIGHT SIDE
			3, 6, 7, //RIGHT SIDE
			4, 5, 1, //LEFT SIDE
			4, 1, 0, //LEFT SIDE
			1, 5, 6, //TOP
			1, 6, 2, //TOP
			0, 3, 7, //BOTTOM
			0, 7, 4, //BOTTOM
		};

		//Load Index Data
		hr = this->indexBuffer.Initialize(this->device, indices, ARRAYSIZE(indices));

		//COM_ERROR_IF_FAILED(hr, "Failed to initialize index buffer.");
	/*}
	catch (COMException& exception)
	{
		ExceptionLoger::ExceptionCall(exception);
		return false;
	}
	*/

	this->SetPosition(0.0f, 0.0f, 0.0f);
	this->SetRotation(0.0f, 0.0f, 0.0f);
	this->UpdateWorldMatrix();
	return true;
}

void SphereObject::SetTexture(ID3D11ShaderResourceView* texture)
{
	this->texture = texture;
}

void SphereObject::Draw(const XMMATRIX& viewProjectionMatrix)
{
	//Update Constant buffer with WVP Matrix
	this->cbVsVertexshader->data.mat = this->worldMatrix * viewProjectionMatrix; //Calculate World-View-Projection Matrix
	this->cbVsVertexshader->data.mat = XMMatrixTranspose(this->cbVsVertexshader->data.mat);
	this->cbVsVertexshader->ApplyChanges();
	this->deviceContext->VSSetConstantBuffers(0, 1, this->cbVsVertexshader->GetAddressOf());

	this->deviceContext->PSSetShaderResources(0, 1, &this->texture); //Set Texture
	this->deviceContext->IASetIndexBuffer(this->indexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	
	UINT offset = 0;

	this->deviceContext->IASetVertexBuffers(0, 1, this->vertexBuffer.GetAddressOf(), this->vertexBuffer.StridePtr(), &offset);
	this->deviceContext->DrawIndexed(this->indexBuffer.BufferSize(), 0, 0); //Draw
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

void SphereObject::SetPosition(const XMVECTOR& pos)
{
	XMStoreFloat3(&this->pos, pos);
	this->posVector = pos;
	this->UpdateWorldMatrix();
}

void SphereObject::SetPosition(const XMFLOAT3& pos)
{
	this->pos = pos;
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateWorldMatrix();
}

void SphereObject::SetPosition(float x, float y, float z)
{
	this->pos = XMFLOAT3(x, y, z);
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateWorldMatrix();
}

void SphereObject::AdjustPosition(const XMVECTOR& pos)
{
	this->posVector += pos;
	XMStoreFloat3(&this->pos, this->posVector);
	this->UpdateWorldMatrix();
}

void SphereObject::AdjustPosition(const XMFLOAT3& pos)
{
	this->pos.x += pos.y;
	this->pos.y += pos.y;
	this->pos.z += pos.z;
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

void SphereObject::SetRotation(const XMVECTOR& rot)
{
	this->rotVector = rot;
	XMStoreFloat3(&this->rot, rot);
	this->UpdateWorldMatrix();
}

void SphereObject::SetRotation(const XMFLOAT3& rot)
{
	this->rot = rot;
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}

void SphereObject::SetRotation(float x, float y, float z)
{
	this->rot = XMFLOAT3(x, y, z);
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}

void SphereObject::AdjustRotation(const XMVECTOR& rot)
{
	this->rotVector += rot;
	XMStoreFloat3(&this->rot, this->rotVector);
	this->UpdateWorldMatrix();
}

void SphereObject::AdjustRotation(const XMFLOAT3& rot)
{
	this->rot.x += rot.x;
	this->rot.y += rot.y;
	this->rot.z += rot.z;
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
		const float distance = sqrt(lookAtPos.x * lookAtPos.x + lookAtPos.z * lookAtPos.z);
		pitch = atan(lookAtPos.y / distance);
	}

	float yaw = 0.0f;
	if (lookAtPos.x != 0.0f)
	{
		yaw = atan(lookAtPos.x / lookAtPos.z);
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