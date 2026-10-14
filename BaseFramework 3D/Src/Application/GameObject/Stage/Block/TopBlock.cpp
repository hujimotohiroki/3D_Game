#include "TopBlock.h"
#include "SideBlock.h"
#include "../../../Scene/GameScene/GameScene.h"
#include "../../../Scene/SceneManager.h"
void TopBlock::Init()
{
	m_model = std::make_shared<KdModelData>();
	m_model->Load("Asset/Models/Block/SideBlock.gltf");
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("GroundCollision", m_model, KdCollider::TypeGround);
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
		if (m_dir.Length() < 5)
		{
			m_dir.Normalize();
			m_dir.z = 0;
			m_pos += m_dir * speed;
		}
	}
	m_gravity += m_gravityacc;
	m_pos.z += m_gravity;
	m_mWorld = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(90))*Math::Matrix::CreateTranslation(m_pos);
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
	//m_pDebugWire->AddDebugSphere(sphere.m_sphere.Center, sphere.m_sphere.Radius, kRedColor);

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
	KdCollider::RayInfo ray;

	ray.m_pos = m_pos;
	static const float m_adjustHeight = 1.0f;
	ray.m_pos.z -= enableStepHigh;
	ray.m_pos.z += m_adjustHeight;
	ray.m_dir = Math::Vector3(0, 0, 1);
	ray.m_range = 0.2f + m_gravity;

	//当たり判定をしたいタイプを設定
	ray.m_type = KdCollider::TypeGround;

	//デバッグ用
	//m_pDebugWire->AddDebugLine(ray.m_pos, ray.m_dir, ray.m_range, Math::Color(1, 0, 0, 1));

	// レイに当たったオブジェクト情報を格納するリスト
	std::list<KdCollider::CollisionResult> retRayList;
	for (auto& obj : SceneManager::Instance().GetObjList()) {
		obj->Intersects(ray, &retRayList);
	}

	// レイに当たったリストから一番近いオブジェクトを検出
	hit = false;
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
		m_pos.z -= m_adjustHeight;	// 地面から補正値分上げる
		m_gravity = 0;
	}
}

/*void TopBlock::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}*/

void TopBlock::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}
