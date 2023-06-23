#pragma once

#include <DirectXMath.h>

struct Point {
	Point() = default;

	Point(float radius, DirectX::XMFLOAT3 pointPos, DirectX::XMFLOAT3 velosity, DirectX::XMFLOAT3 acceleration, bool moveState, bool createGravityState)
		: radius(radius), pointPos(pointPos), velosity(velosity), acceleration(acceleration), moveState(moveState), createGravityState(createGravityState){}

	bool moveState = true;
	bool createGravityState = false;

	float radius = 1.0f;
	DirectX::XMFLOAT3 pointPos{ 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 velosity{ 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 acceleration{ 0.0f, 0.0f, 0.0f };
};
