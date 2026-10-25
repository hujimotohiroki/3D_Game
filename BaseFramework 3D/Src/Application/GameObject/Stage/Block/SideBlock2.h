#pragma once
#include "../StageObjectBase.h"

class SideBlock2 :public StageObjectBase
{
public:
	void Init() override;
	void Update()override;
	void PostUpdate()override;
	//void GenerateDepthMapFromLight()override;
	void DrawLit()override;
	void SetTarget(std::weak_ptr<KdGameObject> target) { m_wpTarget = target; }
	void PushBlock(int Flg);
private:
	std::weak_ptr<KdGameObject> m_wpTarget;
	float m_gravity = 0;
	float m_gravityacc = 0.05f;
	Math::Matrix RotMat = Math::Matrix::Identity;
	DirectX::BoundingOrientedBox blockBox;
	Math::Vector3 m_collisionHalfSize = Math::Vector3(8.0f, 24.0f, 8.0f);
	int rollFlg = 0;
	int rollCount = 0;
};