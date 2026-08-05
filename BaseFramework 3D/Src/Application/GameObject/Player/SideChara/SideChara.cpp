#include "SideChara.h"
#include "../../../Scene/GameScene/GameScene.h"

void SideChara::Init()
{
	m_topmodel = std::make_shared<KdModelData>();
	m_topmodel->Load("Asset/tmp/Crystal.gltf");
	m_sidemodel = std::make_shared<KdModelData>();
	m_sidemodel->Load("Asset/tmp/Crystal.gltf");

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void SideChara::Update()
{
	if (m_gameScene->GetNowChara() != GameScene::NowChara::Side) return;
	m_dir = Math::Vector3::Zero;
	m_rot = Math::Vector3::Zero;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		m_dir.x = -1;
		m_rot.y = 180;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		m_dir.x = 1;
		m_rot.y = 0;
	}
	if (GetAsyncKeyState(VK_UP) & 0x8000) {
		/*
		if(m_jumpFlg)
		{
			m_rot.y = 1;
		}
		*/
	}
	m_pos += m_dir * speed;
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos * 2);
	Math::Matrix rotMat = Math::Matrix::CreateRotationY(m_rot.y);
	
	m_mWorld = rotMat*transMat;
}

void SideChara::PostUpdate()
{
	//スフィアでよくね？？？？？？？？？？
	KdCollider::RayInfo ray;
	ray.m_pos = m_pos*2 - Math::Vector3(0, 2, 0); // 足元に合わせたい
	ray.m_dir = Math::Vector3(0, -1, 0);
	ray.m_range = 1;//テキトー
	
	//当たり判定をするタイプ
	ray.m_type = KdCollider::TypeGround;
	
	//デバッグ用
	m_pDebugWire->AddDebugLine(ray.m_pos, ray.m_dir, ray.m_range, kRedColor);
}

void SideChara::GenerateDepthMapFromLight()
{
	if (m_gameScene->GetNowChara() == GameScene::NowChara::Top) 
	{
		KdShaderManager::Instance().m_StandardShader.DrawModel(*m_topmodel, m_mWorld);
	}
	else if (m_gameScene->GetNowChara() == GameScene::NowChara::Side) 
	{
		KdShaderManager::Instance().m_StandardShader.DrawModel(*m_sidemodel, m_mWorld);
	}
}

void SideChara::DrawLit()
{
	if (m_gameScene->GetNowChara() == GameScene::NowChara::Top)
	{
		KdShaderManager::Instance().m_StandardShader.DrawModel(*m_topmodel, m_mWorld);
	}
	else if (m_gameScene->GetNowChara() == GameScene::NowChara::Side)
	{
		KdShaderManager::Instance().m_StandardShader.DrawModel(*m_sidemodel, m_mWorld);
	}
	
}
