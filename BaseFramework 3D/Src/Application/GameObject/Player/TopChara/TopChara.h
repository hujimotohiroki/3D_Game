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
	const float speed = 0.125f;
	float m_angle = 0;
	Math::Vector3 toDir = { 0, 1, 0 };
};