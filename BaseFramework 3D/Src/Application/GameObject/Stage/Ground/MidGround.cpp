#include "MidGround.h"
#include "../../../Scene/GameScene/GameScene.h"
void MidGround::Init()
{
	m_model = std::make_shared<KdModelData>();
	m_model->Load("Asset/Models/Ground/midground.gltf");
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("GroundCollision", m_model, KdCollider::TypeGround);
}

void MidGround::Update()
{
	m_mWorld = Math::Matrix::CreateTranslation(m_pos);
}

void MidGround::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}

void MidGround::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}
