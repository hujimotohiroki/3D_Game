#pragma once
#include "../StageObjectBase.h"

class TopBlock :public StageObjectBase
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
	Math::Vector3 m_dir = Math::Vector3::Zero;
	const float speed = 0.05f;
};