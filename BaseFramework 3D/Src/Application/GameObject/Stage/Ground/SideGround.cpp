#include "SideGround.h"
#include "TopGround.h"
#include "../../../Scene/GameScene/GameScene.h"
void SideGround::Init()
{
	m_model = std::make_shared<KdModelData>();
	m_model->Load("Asset/tmp/Crystal.gltf");
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("GroundCollision", m_model, KdCollider::TypeGround);
}

void SideGround::Update()
{
	/*
	if (m_gameScene->GetNowChara() != GameScene::NowChara::Side)
	{
		m_isExpired = true;
		std::shared_ptr<TopGround> topGround = std::make_shared<TopGround>();
		topGround->SetGameScene(m_gameScene);
		topGround->SetPos(GetPos());
		topGround->Init();
		m_gameScene->AddObject(topGround);
	}
	m_pos += m_dir * speed;
	m_gravity += m_gravityacc;
	m_dir.y -= m_gravity;
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix rotMat = Math::Matrix::CreateRotationY(m_rot.y);

	m_mWorld = rotMat * transMat;*/
}

void SideGround::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}

void SideGround::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}
