#include "TopGround.h"
#include "SideGround.h"
#include "../../../Scene/GameScene/GameScene.h"
void TopGround::Init()
{
	m_model = std::make_shared<KdModelData>();
	m_model->Load("Asset/tmp/Crystal.gltf");
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("GroundCollision", m_model, KdCollider::TypeGround);
}

void TopGround::Update()
{
	if (m_gameScene->GetNowChara() != GameScene::NowChara::Top)
	{
		m_isExpired = true;
		std::shared_ptr<SideGround> sideGround = std::make_shared<SideGround>();
		sideGround->SetGameScene(m_gameScene);
		sideGround->SetPos(GetPos());
		sideGround->Init();
		m_gameScene->AddObject(sideGround);
	}
}

void TopGround::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}

void TopGround::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}
