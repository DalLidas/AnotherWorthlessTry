#pragma once

struct BindMSG {
	bool createState = false;
	DirectX::XMFLOAT3 pointOfGod { 10.0f, 15.0f, 10.0f };

	bool  airResistanceState = false;
	bool  bounceDicrimentState = false;
	float bounceDicrement = 0.01f;
	float airResistanceDicrement = 0.01f;
};