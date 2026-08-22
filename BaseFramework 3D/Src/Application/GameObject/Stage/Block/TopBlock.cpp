#include "TopBlock.h"
#include "SideBlock.h"
#include "../../../Scene/GameScene/GameScene.h"
#include "../../../Scene/SceneManager.h"
void TopBlock::Init()
{
	m_model = std::make_shared<KdModelData>();
	m_model->Load("Asset/Models/Ground/topground.gltf");
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("GroundCollision", m_model, KdCollider::TypeDamage);
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void TopBlock::Update()
{
	if (m_gameScene->GetNowChara() == GameScene::NowChara::Side)
	{
		m_isExpired = true;
		std::shared_ptr<SideBlock> sideblock = std::make_shared<SideBlock>();
		sideblock->Init();
		sideblock->SetPos(m_pos);
		sideblock->SetGameScene(m_gameScene);
		sideblock->SetTarget(m_wpTarget);
		SceneManager::Instance().AddObject(sideblock);
		return;
	}
	
	if (m_wpTarget.expired() == false) {
		Math::Vector3 TargetPos;
		TargetPos = m_wpTarget.lock()->GetPos();
		m_dir = TargetPos - m_pos;
		if (m_dir.Length() < 10)
		{
			m_dir.Normalize();
			m_dir.z = 0;
			m_pos += m_dir * speed;
		}
	}

	m_mWorld = Math::Matrix::CreateTranslation(m_pos);
}

void TopBlock::PostUpdate()
{
	KdCollider::SphereInfo sphere;

	//球の中心座標を設定
	sphere.m_sphere.Center = m_pos;
	static const float enableStepHigh = 0.1f;
	sphere.m_sphere.Center.y += enableStepHigh;

	//球の半径を設定
	sphere.m_sphere.Radius = 0.9f;

	//当たり判定をしたいタイプを設定
	sphere.m_type = KdCollider::TypeGround;

	//デバッグ用
	m_pDebugWire->AddDebugSphere(sphere.m_sphere.Center, sphere.m_sphere.Radius, kRedColor);

	//Z軸固定忘れずに
	//球に当たったオブジェクト情報を格納するリスト
	std::list<KdCollider::CollisionResult> retSphereList;
	for (auto& obj : SceneManager::Instance().GetObjList()) {
		obj->Intersects(sphere, &retSphereList);
	}

	//球に当たったリストから一番近いオブジェクトを探す
	float maxOverlap = 0;
	bool hit = false;
	Math::Vector3 hitDir;//当たった方向
	for (auto& ret : retSphereList) {
		if (maxOverlap < ret.m_overlapDistance) {
			maxOverlap = ret.m_overlapDistance;
			hitDir = ret.m_hitDir;
			hitDir.z = 0;
			hit = true;
		}
	}
	if (hit == true) {
		//方向ベクトルを長さ1にする
		hitDir.Normalize();
		m_pos += hitDir * maxOverlap;
	}
}

void TopBlock::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}

void TopBlock::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}
