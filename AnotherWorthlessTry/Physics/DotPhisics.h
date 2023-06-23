#pragma once

#include <DirectXMath.h>
#include <vector>

#include "..//Point.h"

class DotPhysics {
public:
	void Initialize(DirectX::XMFLOAT3 sceneBorder_, bool bounceDicrimentState_ = false, bool airResistanceState_ = false);
	void SetDeltaTime(float dt_);
	void SetSceneBorder(const DirectX::XMFLOAT3& sceneBorder_);

	//std::vector<Point> CalculateScene(const std::vector<Point>& points);

	bool ObjectCollision(const Point& point1, const Point& point2);
	int BorderCollision(const Point& point);

	Point Move(const Point& point, float num = 0.001f);
	Point Accelerate(const Point& point, float num = 0.001f);
	Point PAccelerate(const Point& point, float num = 0.001f);
	Point GAccelerate(const Point& point, float num = 0.001f);

	DirectX::XMFLOAT3 DistanceVector(const DirectX::XMFLOAT3& pos1, const DirectX::XMFLOAT3& pos2);
	float Distance(const DirectX::XMFLOAT3& distanceVector);

	std::pair<Point, Point> BounceFromObject(const Point& point1, const Point& point2);
	Point BounceFromBorder(const Point& point, int borderSide);
	DirectX::XMFLOAT3 CalcAcceleration(const Point& point1, const Point& point2, float multiplier);

	DirectX::XMFLOAT3 BounceDicrement(DirectX::XMFLOAT3 velosity);

	void SetStates(bool gAccelerationState = true, bool bounceDicrimentState = false, bool airResistanceState = false);
	void SetBounceDicrement(float newBounceDicrement);
	void SetAirResistanceDicrement(float newairResistanceDicrement);

private:
	float dt = 0.0f; 

	bool  gAccelerationState = true;
	bool  airResistanceState = false;
	bool  bounceDicrimentState = false;
	float bounceDicrement = 20.0f;
	float airResistanceDicrement = 20.0f;

	enum borderSide {
		topX = 1,
		botX,
		topY,
		botY,
		topZ,
		botZ,
	};

	DirectX::XMFLOAT3 sceneBorder{ 20.0f, 20.0f, 20.0f };
	DirectX::XMFLOAT3 gAcceleration{ 0.0f, -10.0f, 0.0f };
};
