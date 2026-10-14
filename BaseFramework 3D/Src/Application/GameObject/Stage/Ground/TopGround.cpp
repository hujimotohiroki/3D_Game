#include "TopGround.h"
#include "../../../Scene/GameScene/GameScene.h"
void TopGround::Init()
{
	m_model = std::make_shared<KdModelData>();
	m_model->Load("Asset/Models/Stage/Stage" + std::to_string(m_stage) + "-" + std::to_string(m_step) + ".gltf");
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("GroundCollision", m_model, KdCollider::TypeGround );
}

void TopGround::Update()
{
	if (m_pos.y > (float)(m_step - 1.0) * 17.0f) m_pos.y -= 0.5f;
	m_mWorld = Math::Matrix::CreateScale(4) * Math::Matrix::CreateTranslation(m_pos);
}

/*void TopGround::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}*/

void TopGround::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}
