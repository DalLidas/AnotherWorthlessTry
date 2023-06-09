#pragma once

#include "..//ConstantBuffer.h"
#include "..//IndexBuffer.h"
#include "..//VertexBuffer.h"
#include "..//Vertex.h"

using namespace DirectX;

class SkyBox {
public:
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertexshader>& cbVsVertexshader);

	void Draw(const XMMATRIX& viewProjectionMatrix);

	const XMVECTOR& GetPositionVector() const;
	const XMFLOAT3& GetPositionFloat3() const;

	void SetScale(float scale);
	void SetScaleX(float scale);
	void SetScaleY(float scale);
	void SetScaleZ(float scale);
	void SetPosition(const XMVECTOR& pos);
	void SetPosition(const XMFLOAT3& pos);
	void SetPosition(float x, float y, float z);
	void AdjustPosition(const XMVECTOR& pos);
	void AdjustPosition(const XMFLOAT3& pos);
	void AdjustPosition(float x, float y, float z);

private:
	void UpdateWorldMatrix();

	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;
	ConstantBuffer<CB_VS_vertexshader>* cbVsVertexshader = nullptr;

	Microsoft::WRL::ComPtr <ID3D11RasterizerState> resterazerState = nullptr;

	VertexBuffer<Vertex> vertexBuffer;
	IndexBuffer indexBuffer;

	XMMATRIX worldMatrix = XMMatrixIdentity();
	XMMATRIX scaleMatrix = XMMatrixIdentity();

	XMVECTOR posVector{};
	XMFLOAT3 pos{};
};