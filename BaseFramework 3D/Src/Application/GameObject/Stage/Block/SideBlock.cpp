#include "SideBlock.h"
#include "TopBlock.h"
#include "../../../Scene/GameScene/GameScene.h"
#include "../../../Scene/SceneManager.h"
#include <cmath>

void SideBlock::Init()
{
	m_model = std::make_shared<KdModelData>();
	m_model->Load("Asset/Models/Block/SideBlock.gltf");
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("BlockCollision", m_model, KdCollider::TypeBump);
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void SideBlock::Update()
{
	
	m_gravity += m_gravityacc;
	m_pos.y -= m_gravity;
	/*if (m_gameScene->GetNowChara() == GameScene::NowChara::Top)
	{
		m_isExpired = true;
		std::shared_ptr<TopBlock> topblock = std::make_shared<TopBlock>();
		topblock->Init();
		topblock->SetPos(m_pos);
		topblock->SetGameScene(m_gameScene);
		topblock->SetTarget(m_wpTarget);
		SceneManager::Instance().AddObject(topblock);
		return;
	}*/

	m_mWorld = Math::Matrix::CreateScale(4)*Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(-90)) * Math::Matrix::CreateTranslation(m_pos);
}

void SideBlock::PostUpdate()
{
	KdCollider::RayInfo ray;

	ray.m_pos = m_pos;
	static const float enableStepHigh = 0.3f;
	static const float m_adjustHeight = 8.0f;
	ray.m_pos.y += enableStepHigh;
	ray.m_pos.y -= m_adjustHeight;
	ray.m_dir = Math::Vector3(0, -1, 0);
	ray.m_range = 12.0f;

	//当たり判定をしたいタイプを設定
	ray.m_type = KdCollider::TypeGround;

	//デバッグ用
	//m_pDebugWire->AddDebugLine(ray.m_pos, ray.m_dir, ray.m_range, Math::Color(1, 0, 0, 1));

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

	/*
	// 立方体の当たり判定を行う
	// DirectX::BoundingOrientedBox を作成して BoxInfo のコンストラクタで初期化する
	DirectX::BoundingOrientedBox obb;
	obb.Center = m_pos;
	obb.Extents = Math::Vector3(8.0f, 8.0f, 8.0f);
	// 地面判定対象（TypeGround）と衝突して重ならないようにする
	KdCollider::BoxInfo box(KdCollider::TypeGround|KdCollider::TypeBump, obb);

	// デバッグ可視化: ボックス描画はヒット判定後に色を変えて描画する


	// 立方体に当たったオブジェクト情報を格納するリスト
	std::list<KdCollider::CollisionResult> retBoxList;
	for (auto& obj : SceneManager::Instance().GetObjList()) {
		obj->Intersects(box, &retBoxList);
	}

	// 立方体に当たったリストから一番重なりが大きいオブジェクトを探す
	float maxBoxOverlap = 0;
	bool boxHit = false;
	Math::Vector3 boxHitDir; // 当たった方向
	for (auto& ret : retBoxList) {
		KdDebugGUI::Instance().AddLog("%f,%f,%f\n", ret.m_hitDir.x, ret.m_hitDir.y, ret.m_hitDir.z);
		KdDebugGUI::Instance().AddLog("%f\n", ret.m_overlapDistance);
		if (maxBoxOverlap < ret.m_overlapDistance) {
			maxBoxOverlap = ret.m_overlapDistance;
			boxHitDir = ret.m_hitDir;

			// 縦横方向（XZ）のみで押し戻す。上下の補正はレイによる着地処理で行う
			boxHitDir.y = 0;
			boxHit = true;
		}
	}
	if (boxHit == true) {
		if (boxHitDir.LengthSquared() > 0.0f) {
			boxHitDir.Normalize();
			m_pos += boxHitDir * maxBoxOverlap;
		}
	}

	// モデルベースのコライダーは BOX 判定が未実装の場合があるため、球でも補助判定を行う
	KdCollider::SphereInfo sphere;
	sphere.m_sphere.Center = m_pos;
	sphere.m_sphere.Center.y -= 0.0f;
	sphere.m_sphere.Radius = 8.0f;
	sphere.m_type = KdCollider::TypeBump;

	std::list<KdCollider::CollisionResult> retSphereList;
	for (auto& obj : SceneManager::Instance().GetObjList()) {
		obj->Intersects(sphere, &retSphereList);
	}

	// 球に当たったリストから一番重なりが大きいものを探す
	float maxSphereOverlap = 0;
	bool sphereHit = false;
	Math::Vector3 sphereHitDir;
	for (auto& ret : retSphereList) {
		
		if (maxSphereOverlap < ret.m_overlapDistance) {
			maxSphereOverlap = ret.m_overlapDistance;
			sphereHitDir = ret.m_hitDir;
			
			sphereHit = true;
		}
	}
	
	// 球で当たっているが BOX では検出できていない場合は球の結果で押し戻す
	if (!boxHit && sphereHit) {
		if (sphereHitDir.LengthSquared() > 0.0f) {
			sphereHitDir.y = 0;
			sphereHitDir.Normalize();
			m_pos += sphereHitDir * maxSphereOverlap;
			boxHit = true; // 色表示のためヒットと見なす
		}
	}
	if (m_pDebugWire) {
		Math::Color col = boxHit ? Math::Color(1, 0, 0, 1) : Math::Color(0, 1, 0, 1);
		//m_pDebugWire->AddDebugBox(Math::Matrix::CreateTranslation(m_pos), Math::Vector3(8.0f, 8.0f, 8.0f), Math::Vector3::Zero, true, col);
	}
	*/
	/*KdCollider::SphereInfo sphere;
	sphere.m_sphere.Center = m_pos;
	sphere.m_sphere.Center.y -= 0.2f;
	sphere.m_sphere.Radius = 6.8f;
	sphere.m_type = KdCollider::TypeBump;

	std::list<KdCollider::CollisionResult> retSphereList;
	for (auto& obj : SceneManager::Instance().GetObjList()) {
		obj->Intersects(sphere, &retSphereList);
	}

	// 球に当たったリストから一番重なりが大きいものを探す
	float maxSphereOverlap = 0;
	bool sphereHit = false;
	Math::Vector3 sphereHitDir;
	for (auto& ret : retSphereList) {
		if (maxSphereOverlap < ret.m_overlapDistance) {
			maxSphereOverlap = ret.m_overlapDistance;
			sphereHitDir = ret.m_hitDir;
			sphereHitDir.y = 0;
			sphereHit = true;
		}
	}

	if (sphereHit) {
		if (sphereHitDir.LengthSquared() > 0.0f) {
			if( abs(sphereHitDir.x)>abs(sphereHitDir.z))
			{
				sphereHitDir.x = 0;
			}
			else if(abs(sphereHitDir.x) < abs(sphereHitDir.z))
			{
				sphereHitDir.z = 0;
			}
			else {
				sphereHitDir = { 0,1,0 };
			}
			sphereHitDir.Normalize();
			m_pos += sphereHitDir * maxSphereOverlap;
		}
	}*/
	//m_pDebugWire->AddDebugSphere(sphere.m_sphere.Center, sphere.m_sphere.Radius, sphereHit ? kRedColor : kGreenColor);
}

/*void SideBlock::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}*/

void SideBlock::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}

void SideBlock::PushBlock(int Flg)
{
	KdCollider::RayInfo ray;

	ray.m_pos = m_pos;
	static const float enableStepHigh = 0.3f;
	static const float m_adjustHeight = 8.0f;
	ray.m_pos.y += enableStepHigh;
	ray.m_pos.y += m_adjustHeight;
	switch (Flg)
	{
	case 0:
		ray.m_dir = Math::Vector3(-1, 0, 0);
		break;
	case 1:
		ray.m_dir = Math::Vector3(0, 0, -1);
		break;
	case 2:
		ray.m_dir = Math::Vector3(1, 0, 0);
		break;
	case 3:
		ray.m_dir = Math::Vector3(0, 0, 1);
		break;
	}
	ray.m_range = 23.0f;

	//当たり判定をしたいタイプを設定
	ray.m_type = KdCollider::TypeGround|KdCollider::TypeBump;

	//デバッグ用
	m_pDebugWire->AddDebugLine(ray.m_pos, ray.m_dir, ray.m_range, Math::Color(1, 0, 0, 1));

	//球に当たったオブジェクト情報を格納するリスト
	std::list<KdCollider::CollisionResult> retRayList;
	for (auto& obj : SceneManager::Instance().GetObjList()) {
		obj->Intersects(ray, &retRayList);
	}

	// 当たっていなかったら
	if (retRayList.empty())
	{
		switch (Flg)
		{
		case 0:
			m_pos.x -= 16.0f;
			break;
		case 1:
			m_pos.z -= 16.0f;
			break;
		case 2:
			m_pos.x += 16.0f;
			break;
		case 3:
			m_pos.z += 16.0f;
			break;
		}
	}
	
}
