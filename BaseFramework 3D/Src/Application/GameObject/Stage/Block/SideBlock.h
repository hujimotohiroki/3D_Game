#pragma once
#include "../StageObjectBase.h"

class SideBlock :public StageObjectBase
{
public:
	void Init() override;
	void Update()override;
	void PostUpdate()override;
	void GenerateDepthMapFromLight()override;
	void DrawLit()override;
	void SetTarget(std::weak_ptr<KdGameObject> target) { m_wpTarget = target; }
private:
	std::weak_ptr<KdGameObject> m_wpTarget;
	float m_gravity = 0;
	float m_gravityacc = 0.01f;
};