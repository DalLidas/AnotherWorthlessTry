#pragma once

struct BindMSG {
	bool phsicsState = true;
	bool gAccelerationState = true;

	float movMultiplier = 1.0f;
	float accMultiplier = 1.0f;
	float CreatedAccMultiplier = 1.0f;

	bool createState = false;
	bool createRandomState = false;
	DirectX::XMFLOAT3 pointOfGod { 10.0f, 15.0f, 10.0f };
	DirectX::XMFLOAT3 sceneBorder { 30.0f, 30.0f, 30.0f };
	Point point;

	bool destroyState = false;

	bool  airResistanceState = false;
	bool  bounceDicrimentState = false;
	float bounceDicrement = 10.0f;
	float airResistanceDicrement = 10.0f;
};