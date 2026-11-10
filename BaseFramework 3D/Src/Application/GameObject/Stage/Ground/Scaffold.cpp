#include "Scaffold.h"
#include "../../../Scene/GameScene/GameScene.h"
void Scaffold::Init()
{
	m_model = std::make_shared<KdModelData>();
	m_model->Load("Asset/Models/Stage/Scaffold.gltf");
	/*m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("GroundCollision", m_model, KdCollider::TypeGround );*/
	

	groundBox.Center = Math::Vector3::Zero;
	groundBox.Center.y -= 0.1f;
	groundBox.Extents = Math::Vector3(2.0f, 0.1f, 2.0f);
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape(
		"ScaffoldCollision",
		groundBox,
		KdCollider::TypeGround
	);
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void Scaffold::Update()
{
	//if (m_pos.y > (float)(m_step - 1.0) * 17.0f) m_pos.y -= 0.5f;
	m_mWorld = Math::Matrix::CreateScale(4) * Math::Matrix::CreateTranslation(m_pos);
	//m_pDebugWire->AddDebugBox(Math::Matrix::CreateTranslation(m_pos), Math::Vector3(8.0f, 0.4f, 8.0f), Math::Vector3::Zero, true, Math::Color(0, 1, 0, 1));
}

/*void TopGround::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}*/

void Scaffold::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}
