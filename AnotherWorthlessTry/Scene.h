#pragma once

#include <DirectXMath.h>
#include <vector>

#include "Point.h"

class Scene {
public:
	Scene();

	std::vector<Point> GetPoints() const;
	DirectX::XMFLOAT3 GetSceneBorder() const;

	void SetPoints(std::vector<Point> newPoints);
	void SetPoint(Point newPoint, size_t index);
	void SetSceneBorder(DirectX::XMFLOAT3 newSceneBorder);

	void createPoint();
	void createPoint(DirectX::XMFLOAT3 pos);
	void createPoint(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 velosity, DirectX::XMFLOAT3 acceleration);

private:
	DirectX::XMFLOAT3 sceneBorder{ 100.0f, 100.0f, 50.0f };
	DirectX::XMFLOAT3 pointOfGod { 10.0f, 10.0f, 0.0f };
	std::vector<Point> points;

	const DirectX::XMFLOAT3 DEFAULT_VELOSITY { 0.0f, 0.0f, 0.001f };
	const DirectX::XMFLOAT3 DEFAULT_ACCELERATION { 0.0f, 0.0f, 0.001f };

	const DirectX::XMFLOAT3 ZERO_VELOSITY { 0.0f, 0.0f, 0.0f };
	const DirectX::XMFLOAT3 ZERO_ACCELERATION { 0.0f, 0.0f, 0.0f };
};