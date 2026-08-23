#include "FirstBack.h"

void FirstBack::Init()
{
	m_model = std::make_shared<KdModelData>();
	m_model->Load("Asset/Models/Back/FirstBack.gltf");
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("GroundCollision", m_model, KdCollider::TypeGround);
	m_mWorld = Math::Matrix::CreateScale(25.0f) * Math::Matrix::CreateTranslation(m_pos);
}

void FirstBack::Update()
{}

void FirstBack::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}

void FirstBack::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}
