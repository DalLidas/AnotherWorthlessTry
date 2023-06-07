#pragma once

#include <DirectXMath.h>

struct Point {
	Point() = default;

	Point(float radius, DirectX::XMFLOAT3 pointPos, DirectX::XMFLOAT3 velosity, DirectX::XMFLOAT3 acceleration)
		: radius(radius), pointPos(pointPos), velosity(velosity), acceleration(acceleration) {}

	float radius = 1.0f;
	DirectX::XMFLOAT3 pointPos;
	DirectX::XMFLOAT3 velosity;
	DirectX::XMFLOAT3 acceleration;
};
