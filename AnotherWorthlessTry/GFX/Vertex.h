#pragma once

#include <DirectXMath.h>

struct Vertex
{
	Vertex() = default;

	Vertex(float xx, float yy, float zz, float rr, float gg, float bb)
		: pos(xx, yy, zz), color(rr, gg, bb) {}

	DirectX::XMFLOAT3 pos   { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 color { 0.0f, 0.0f, 0.0f };
};