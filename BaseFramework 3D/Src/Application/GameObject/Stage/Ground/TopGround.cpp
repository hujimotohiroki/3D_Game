#include "TopGround.h"
#include "../../../Scene/GameScene/GameScene.h"
void TopGround::Init()
{
	m_model = std::make_shared<KdModelData>();
	m_model->Load("Asset/Models/Stage/Stage" + std::to_string(m_stage) + "-" + std::to_string(m_step) + ".gltf");
	/*m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("GroundCollision", m_model, KdCollider::TypeGround );*/
	

	groundBox.Center = Math::Vector3::Zero;
	groundBox.Center.y += 1.0f;
	groundBox.Extents = Math::Vector3(stagesize*2, 1.0f, stagesize*2);
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape(
		"GroundCollision",
		groundBox,
		KdCollider::TypeGround
	);
	wallBox[0].Center = Math::Vector3(groundBox.Center.x + groundBox.Extents.x+1.0f, groundBox.Center.y, groundBox.Center.z);
	wallBox[0].Extents = Math::Vector3(1.0f, 10.0f, groundBox.Extents.z);
	m_pCollider->RegisterCollisionShape(
		"WallCollision0",
		wallBox[0],
		KdCollider::TypeGround
	);
	wallBox[1].Center = Math::Vector3(groundBox.Center.x - groundBox.Extents.x-1.0f, groundBox.Center.y, groundBox.Center.z);
	wallBox[1].Extents = Math::Vector3(1.0f, 10.0f, groundBox.Extents.z);
	m_pCollider->RegisterCollisionShape(
		"WallCollision1",
		wallBox[1],
		KdCollider::TypeGround
	);
	wallBox[2].Center = Math::Vector3(groundBox.Center.x, groundBox.Center.y, groundBox.Center.z + groundBox.Extents.z+1.0f);
	wallBox[2].Extents = Math::Vector3(groundBox.Extents.x, 10.0f, 1.0f);
	m_pCollider->RegisterCollisionShape(
		"WallCollision2",
		wallBox[2],
		KdCollider::TypeGround
	);
	wallBox[3].Center = Math::Vector3(groundBox.Center.x, groundBox.Center.y, groundBox.Center.z - groundBox.Extents.z-1.0f);
	wallBox[3].Extents = Math::Vector3(groundBox.Extents.x, 10.0f, 1.0f);
	m_pCollider->RegisterCollisionShape(
		"WallCollision3",
		wallBox[3],
		KdCollider::TypeGround
	);
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void TopGround::Update()
{
	if (m_pos.y > (float)(m_step - 1.0) * 17.0f) m_pos.y -= 0.5f;
	m_mWorld = Math::Matrix::CreateScale(4) * Math::Matrix::CreateTranslation(m_pos);
	//m_pDebugWire->AddDebugBox(Math::Matrix::CreateTranslation(groundBox.Center), Math::Vector3(40.0f, 4.0f, 40.0f), Math::Vector3::Zero, true, Math::Color(0, 1, 0, 1));
}

/*void TopGround::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}*/

void TopGround::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}
