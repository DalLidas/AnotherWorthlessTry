#pragma once

#include "vector"

#include "..//ConstantBuffer.h"
#include "..//IndexBuffer.h"
#include "..//VertexBuffer.h"
#include "..//Vertex.h"

#include <GeometricPrimitive.h>


using namespace DirectX;

class SphereObject {
public:
	bool Initialize(ID3D11DeviceContext* deviceContext);

	void Draw(const XMMATRIX& viewMatrix, const XMMATRIX& ProjectionMatrix, const XMVECTOR& color);

	const XMVECTOR& GetPositionVector() const;
	const XMFLOAT3& GetPositionFloat3() const;
	const XMVECTOR& GetRotationVector() const;
	const XMFLOAT3& GetRotationFloat3() const;

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
	void SetRotation(const XMVECTOR& rot);
	void SetRotation(const XMFLOAT3& rot);
	void SetRotation(float x, float y, float z);
	void AdjustRotation(const XMVECTOR& rot);
	void AdjustRotation(const XMFLOAT3& rot); 
	void AdjustRotation(float x, float y, float z);
	void SetLookAtPos(XMFLOAT3 lookAtPos);
	const XMVECTOR& GetForwardVector();
	const XMVECTOR& GetRightVector();
	const XMVECTOR& GetBackwardVector();
	const XMVECTOR& GetLeftVector();

private:
	void UpdateWorldMatrix();

	ID3D11DeviceContext* deviceContext = nullptr;

	std::unique_ptr<DirectX::GeometricPrimitive> shape;

	XMMATRIX worldMatrix = XMMatrixIdentity();
	XMMATRIX scaleMatrix = XMMatrixIdentity();

	XMVECTOR posVector;
	XMVECTOR rotVector;
	XMFLOAT3 pos;
	XMFLOAT3 rot;

	const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_BACKWARD_VECTOR = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	const XMVECTOR DEFAULT_LEFT_VECTOR = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_RIGHT_VECTOR = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	XMVECTOR vecForward;
	XMVECTOR vecLeft;
	XMVECTOR vecRight;
	XMVECTOR vecBackward;
};