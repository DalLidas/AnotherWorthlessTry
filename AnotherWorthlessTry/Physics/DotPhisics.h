#pragma once

#include <DirectXMath.h>
#include <vector>

#include "..//Point.h"

class DotPhysics {
public:
	void Initialize(DirectX::XMFLOAT3 sceneBorder, bool bounceDicrimentState = false, bool airResistanceState = false);
	void SetDeltaTime(float dt);

	void CalculateScene(std::vector<Point> points);

	bool ObjectCollision(const Point& point1, const Point& point2);
	bool BorderCollision(const Point& point);

	Point Move(const Point& point);
	void Accelerate(Point& point);

	void BounceFromObject(Point& point1, Point& point2);
	void BounceFromBorder(Point& point);

	void SetStates(bool bounceDicrimentState = false, bool airResistanceState = false);
	void SetBounceDicrement(float newBounceDicrement);
	void SetAirResistanceDicrement(float newairResistanceDicrement);

private:
	float dt = 0.0f; 

	bool airResistanceState = false;
	bool  bounceDicrimentState = false;
	float bounceDicrement = 0.01f;
	float airResistanceDicrement = 0.01f;

	DirectX::XMFLOAT3 sceneBorder{ 100.0f, 100.0f, 50.0f };
};