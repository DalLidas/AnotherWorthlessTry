#pragma once

#include <DirectXMath.h>
#include <vector>

#include "Point.h"

class Scene {
public:
	Scene();

	std::vector<Point> GetPoints() const;
	DirectX::XMFLOAT3 GetSceneBorder() const;

	void SetPoints(const std::vector<Point>& newPoints);
	void SetPoint(const Point& newPoint, size_t index);
	void SetSceneBorder(const DirectX::XMFLOAT3& newSceneBorder);

	void createPoint();
	void createPoint(const DirectX::XMFLOAT3& pos);
	void createPoint(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& velosity, const DirectX::XMFLOAT3& acceleration);

private:
	DirectX::XMFLOAT3 sceneBorder{ 20.0f, 20.0f, 20.0f };
	DirectX::XMFLOAT3 pointOfGod { 40.0f, 40.0f, 0.0f };
	std::vector<Point> points;

	const DirectX::XMFLOAT3 DEFAULT_VELOSITY { 0.001f, 0.005f, 0.0f };
	const DirectX::XMFLOAT3 DEFAULT_ACCELERATION { 0.0f, -0.000001f, 0.0f };

	const DirectX::XMFLOAT3 ZERO_VELOSITY { 0.0f, 0.0f, 0.0f };
	const DirectX::XMFLOAT3 ZERO_ACCELERATION { 0.0f, 0.0f, 0.0f };
};