#include "Wall.h"
#include "../../../Scene/GameScene/GameScene.h"
void Wall::Init()
{
	//m_model = std::make_shared<KdModelData>();
	//m_model->Load("Asset/Models/Stage/Stage" + std::to_string(m_stage) + "-" + std::to_string(m_step) + ".gltf");
	/*m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("GroundCollision", m_model, KdCollider::TypeGround );*/
	

	groundBox.Center = m_pos;
	groundBox.Center.y += 1.0f;
	groundBox.Extents = Math::Vector3(10.0f, 6.0f, 2.0f);
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape(
		"WallCollision",
		groundBox,
		KdCollider::TypeGround
	);
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void Wall::Update()
{
	//if (m_pos.y > (float)(m_step - 1.0) * 17.0f) m_pos.y -= 0.5f;
	//m_mWorld = Math::Matrix::CreateScale(4) * Math::Matrix::CreateTranslation(m_pos);
	//m_pDebugWire->AddDebugBox(Math::Matrix::CreateTranslation(groundBox.Center), Math::Vector3(40.0f, 4.0f, 40.0f), Math::Vector3::Zero, true, Math::Color(0, 1, 0, 1));
}

/*void TopGround::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}*/
