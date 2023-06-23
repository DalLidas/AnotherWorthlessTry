#pragma once

#include <DirectXMath.h>
#include <vector>

#include "Point.h"

class Scene {
public:
	Scene();

	std::vector<Point> GetPoints() const;
	DirectX::XMFLOAT3 GetPointOfGod() const;
	DirectX::XMFLOAT3 GetSceneBorder() const;

	bool IsMoveable(size_t index);
	bool IsCreateGravity(size_t index);

	void SetPoints(const std::vector<Point>& newPoints);
	void SetPoint(const Point& newPoint, size_t index);
	void SetPointOfGod(const DirectX::XMFLOAT3& newPointOfGod);
	void SetSceneBorder(const DirectX::XMFLOAT3& newSceneBorder);

	void CreatePoint();
	void CreatePoint(const DirectX::XMFLOAT3& pos);
	void CreatePoint(float radius, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& velosity, const DirectX::XMFLOAT3& acceleration, bool moveState = true, bool createGravityState = false);

	void DestroyAllPoint();

private:
	DirectX::XMFLOAT3 sceneBorder{ 30.0f, 30.0f, 30.0f };
	DirectX::XMFLOAT3 pointOfGod { 10.0f, 15.0f, 10.0f };
	std::vector<Point> points;

	const DirectX::XMFLOAT3 DEFAULT_VELOSITY { 0.005f, 0.005f, 0.005f };
	const DirectX::XMFLOAT3 DEFAULT_ACCELERATION { 0.0f, -0.001f, 0.0f };

	const DirectX::XMFLOAT3 ZERO_VELOSITY { 0.0f, 0.0f, 0.0f };
	const DirectX::XMFLOAT3 ZERO_ACCELERATION { 0.0f, 0.0f, 0.0f };
};