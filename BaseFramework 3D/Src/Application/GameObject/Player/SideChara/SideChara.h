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
	
	std::shared_ptr<KdAnimator>	m_spAnimator = nullptr;
	std::shared_ptr<KdAnimator>	m_spAnimator2 = nullptr;
	const float speed = 0.25f;
	float m_gravity = 0;
	float m_gravityacc = 0.009f;
	bool m_jumpFlg = false;
};