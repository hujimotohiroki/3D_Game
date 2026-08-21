#pragma once
#include "../PlayerBase.h"

class SideChara :public PlayerBase
{
public:
	void Init() override;
	void Update()override;
	void PostUpdate()override;
	void GenerateDepthMapFromLight()override;
	void DrawLit()override;
private:
	
	const float speed = 0.25f;
	float m_gravity = 0;
	float m_gravityacc = 0.01f;
	bool m_jumpFlg = false;
};