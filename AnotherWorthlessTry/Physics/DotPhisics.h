#pragma once

#include <DirectXMath.h>
#include <vector>

#include "..//Point.h"

class DotPhysics {
public:
	void Initialize(DirectX::XMFLOAT3 sceneBorder, bool bounceDicrimentState = false, bool airResistanceState = false);
	void SetDeltaTime(float dt);

	//std::vector<Point> CalculateScene(const std::vector<Point>& points);

	bool ObjectCollision(const Point& point1, const Point& point2);
	bool BorderCollision(const Point& point);

	Point Move(const Point& point, int num = 1);
	Point Accelerate(const Point& point);

	std::pair<Point, Point> BounceFromObject(const Point& point1, const Point& point2);
	Point BounceFromBorder(const Point& point);

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