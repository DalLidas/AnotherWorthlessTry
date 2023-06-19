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
	int BorderCollision(const Point& point);

	Point Move(const Point& point, int num = 1);
	Point Accelerate(const Point& point);

	std::pair<Point, Point> BounceFromObject(const Point& point1, const Point& point2);
	Point BounceFromBorder(const Point& point, int borderSide);

	DirectX::XMFLOAT3 BounceDicrement(DirectX::XMFLOAT3 velosity);

	void SetStates(bool bounceDicrimentState = false, bool airResistanceState = false);
	void SetBounceDicrement(float newBounceDicrement);
	void SetAirResistanceDicrement(float newairResistanceDicrement);

private:
	float dt = 0.0f; 

	bool  airResistanceState = false;
	bool  bounceDicrimentState = false;
	float bounceDicrement = 99.0f;
	float airResistanceDicrement = 25.0f;

	enum borderSide {
		topX = 1,
		botX,
		topY,
		botY,
		topZ,
		botZ,
	};

	DirectX::XMFLOAT3 sceneBorder{ 20.0f, 20.0f, 20.0f };
};
