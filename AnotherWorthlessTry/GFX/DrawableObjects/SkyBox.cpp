#include "SkyBox.h"

bool SkyBox::Initialize(ID3D11Device* device_, ID3D11DeviceContext* deviceContext_, ConstantBuffer<CB_VS_vertexshader>& cbVsVertexshader_)
{
	this->device = device_;
	this->deviceContext = deviceContext_;
	this->cbVsVertexshader = &cbVsVertexshader_;

	XMFLOAT3 Gray{ 134.0f / 255.0f, 142.0f / 255.0f, 150.0f / 255.0f };
	XMFLOAT3 DarkGray{ 33.0f / 255.0f, 37.0f / 255.0f, 41.0f / 255.0f };

	//Cube
	Vertex v[] =
	{
		Vertex(-0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.0f), //FRONT Bottom Left   - [0]
		Vertex(-0.5f,   0.5f, -0.5f, 0.0f, 0.0f, 0.0f), //FRONT Top Left      - [1]
		Vertex(0.5f,   0.5f, -0.5f, 1.0f, 0.0f, 0.0f), //FRONT Top Right     - [2]
		Vertex(0.5f,  -0.5f, -0.5f, 1.0f, 1.0f, 0.0f), //FRONT Bottom Right   - [3]
		Vertex(-0.5f,  -0.5f, 0.5f, 0.0f, 1.0f, 0.0f), //BACK Bottom Left   - [4]
		Vertex(-0.5f,   0.5f, 0.5f, 0.0f, 0.0f, 0.0f), //BACK Top Left      - [5]
		Vertex(0.5f,   0.5f, 0.5f, 1.0f, 0.0f, 0.0f), //BACK Top Right     - [6]
		Vertex(0.5f,  -0.5f, 0.5f, 1.0f, 1.0f, 0.0f), //BACK Bottom Right   - [7]
	};

	//Load Vertex Data
	HRESULT hr = this->vertexBuffer.Initialize(this->device, v, ARRAYSIZE(v));

	DWORD indices[] =
	{
		2, 1, 0, //FRONT
		3, 2, 0, //FRONT
		6, 7, 4, //BACK 
		5, 6, 4, //BACK
		6, 2, 3, //RIGHT SIDE
		7, 6, 3, //RIGHT SIDE
		1, 5, 4, //LEFT SIDE
		0, 1, 4, //LEFT SIDE
		6, 5, 1, //TOP
		2, 6, 1, //TOP
		7, 3, 0, //BOTTOM
		4, 7, 0, //BOTTOM
	};

	//Load Index Data
	hr = this->indexBuffer.Initialize(this->device, indices, ARRAYSIZE(indices));
	if (FAILED(hr))
		return false;

	this->SetPosition(0.0f, 0.0f, 0.0f);
	this->UpdateWorldMatrix();
	return true;
}

void SkyBox::Draw(const XMMATRIX& viewProjectionMatrix)
{
	//Update Constant buffer with WVP Matrix
	this->cbVsVertexshader->data.mat = this->scaleMatrix * this->worldMatrix * viewProjectionMatrix; //Calculate World-View-Projection Matrix
	this->cbVsVertexshader->data.mat = XMMatrixTranspose(this->cbVsVertexshader->data.mat);
	this->cbVsVertexshader->ApplyChanges();
	this->deviceContext->VSSetConstantBuffers(0, 1, this->cbVsVertexshader->GetAddressOf());

	this->deviceContext->IASetIndexBuffer(this->indexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);

	UINT offset = 0;

	this->deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), vertexBuffer.StridePtr(), &offset);
	this->deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	this->deviceContext->DrawIndexed(indexBuffer.BufferSize(), 0, 0);

}

void SkyBox::UpdateWorldMatrix()
{
	this->worldMatrix = XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z);
}

const XMVECTOR& SkyBox::GetPositionVector() const
{
	return this->posVector;
}

const XMFLOAT3& SkyBox::GetPositionFloat3() const
{
	return this->pos;
}

void SkyBox::SetScale(const XMFLOAT3& scale)
{
	this->scaleMatrix = XMMatrixScaling(scale.x, scale.y, scale.z);
}

void SkyBox::SetScaleX(float scale)
{
	this->scaleMatrix = XMMatrixScaling(scale, 1.0f, 1.0f);
}

void SkyBox::SetScaleY(float scale)
{
	this->scaleMatrix = XMMatrixScaling(1.0f, scale, 1.0f);
}

void SkyBox::SetScaleZ(float scale)
{
	this->scaleMatrix = XMMatrixScaling(1.0f, 1.0f, scale);
}

void SkyBox::SetPosition(const XMVECTOR& pos_)
{
	XMStoreFloat3(&this->pos, pos_);
	this->posVector = pos_;
	this->UpdateWorldMatrix();
}

void SkyBox::SetPosition(const XMFLOAT3& pos_)
{
	this->pos = pos_;
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateWorldMatrix();
}

void SkyBox::SetPosition(float x, float y, float z)
{
	this->pos = XMFLOAT3(x, y, z);
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateWorldMatrix();
}

void SkyBox::AdjustPosition(const XMVECTOR& pos_)
{
	this->posVector += pos_;
	XMStoreFloat3(&this->pos, this->posVector);
	this->UpdateWorldMatrix();
}

void SkyBox::AdjustPosition(const XMFLOAT3& pos_)
{
	this->pos.x += pos_.y;
	this->pos.y += pos_.y;
	this->pos.z += pos_.z;
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateWorldMatrix();
}

void SkyBox::AdjustPosition(float x, float y, float z)
{
	this->pos.x += x;
	this->pos.y += y;
	this->pos.z += z;
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateWorldMatrix();
}