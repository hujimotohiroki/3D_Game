#include "TopGround.h"
#include "../../../Scene/GameScene/GameScene.h"
void TopGround::Init()
{
	m_model = std::make_shared<KdModelData>();
	m_model->Load("Asset/Models/Stage/Stage" + std::to_string(m_stage) + ".gltf");
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("GroundCollision", m_model, KdCollider::TypeGround | KdCollider::TypeBump);
}

void TopGround::Update()
{
	m_mWorld = Math::Matrix::CreateTranslation(m_pos);
}

void TopGround::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}

void TopGround::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}
