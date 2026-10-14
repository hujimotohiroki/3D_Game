#pragma once
#include "../PlayerBase.h"

class TopChara :public PlayerBase
{
public:
	void Init() override;
	void Update()override;
	void PostUpdate()override;
	void GenerateDepthMapFromLight()override;
	void DrawLit()override;
private:
	const float speed = 0.12f;
	float m_gravity = 0;
	float m_gravityacc = 0.009f;
	float m_angle = 0;
	Math::Vector3 toDir = { 0, 1, 0 };
};