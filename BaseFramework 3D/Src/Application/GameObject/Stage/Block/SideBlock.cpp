#include "SideBlock.h"
#include "TopBlock.h"
#include "../../../Scene/GameScene/GameScene.h"
#include "../../../Scene/SceneManager.h"

void SideBlock::Init()
{
	m_model = std::make_shared<KdModelData>();
	m_model->Load("Asset/Models/TopChara/3D Gum Bot/3D Gum Bot.gltf");
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("GroundCollision", m_model, KdCollider::TypeGround);
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void SideBlock::Update()
{
	
	m_gravity += m_gravityacc;
	m_pos.y -= m_gravity;
	if (m_gameScene->GetNowChara() == GameScene::NowChara::Top)
	{
		m_isExpired = true;
		std::shared_ptr<TopBlock> topblock = std::make_shared<TopBlock>();
		topblock->Init();
		topblock->SetPos(m_pos);
		topblock->SetGameScene(m_gameScene);
		topblock->SetTarget(m_wpTarget);
		SceneManager::Instance().AddObject(topblock);
		return;
	}



	m_mWorld = Math::Matrix::CreateScale(5) * Math::Matrix::CreateTranslation(m_pos);
}

void SideBlock::PostUpdate()
{
	KdCollider::RayInfo ray;

	ray.m_pos = m_pos;
	static const float enableStepHigh = 0.1f;
	static const float m_adjustHeight = 2.0f;
	ray.m_pos.y += enableStepHigh;
	ray.m_pos.y -= m_adjustHeight;
	ray.m_dir = Math::Vector3(0, -1, 0);
	ray.m_range = 1.5f;

	//当たり判定をしたいタイプを設定
	ray.m_type = KdCollider::TypeGround;

	//デバッグ用
	m_pDebugWire->AddDebugLine(ray.m_pos, ray.m_dir, ray.m_range, Math::Color(1, 0, 0, 1));

	//Z軸固定忘れずに
	//球に当たったオブジェクト情報を格納するリスト
	std::list<KdCollider::CollisionResult> retRayList;
	for (auto& obj : SceneManager::Instance().GetObjList()) {
		obj->Intersects(ray, &retRayList);
	}

	// レイに当たったリストから一番近いオブジェクトを検出
	bool hit = false;
	float maxOverLap = 0;
	Math::Vector3 groundPos = {};	// レイが遮断された(Hitした)座標

	for (auto& ret : retRayList)
	{
		// レイが当たったオブジェクトの中から
		// 「m_overlapDistance = 貫通した長さ」が一番長いものを探す
		// 「m_overlapDistance が一番長い = 一番近くで当たった」と判定できる
		if (maxOverLap < ret.m_overlapDistance)
		{
			maxOverLap = ret.m_overlapDistance;
			groundPos = ret.m_hitPos;
			hit = true;
		}
	}

	// 当たっていたら
	if (hit)
	{
		m_pos = groundPos;	// レイの着弾地点に着地
		m_pos.y += m_adjustHeight;	// 地面から補正値分上げる
		m_gravity = 0.0f;
	}
}

void SideBlock::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}

void SideBlock::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}
