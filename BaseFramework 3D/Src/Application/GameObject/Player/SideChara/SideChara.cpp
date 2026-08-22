#include "SideChara.h"
#include "../../../Scene/GameScene/GameScene.h"
#include "../../../Scene/SceneManager.h"

void SideChara::Init()
{
	m_model = std::make_shared<KdModelData>();
	m_model->Load("Asset/Models/TopChara/3D Gum Bot/3D Gum Bot.gltf");

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void SideChara::Update()
{
	if (m_gameScene->GetNowChara() != GameScene::NowChara::Side) return;
	m_dir.x = 0;
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
		if(m_jumpFlg)
		{
			m_dir.y = 1;
		}
		m_jumpFlg = false;
	}
	m_pos += m_dir * speed;
	m_gravity += m_gravityacc;
	m_pos.y -= m_gravity;
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix rotMat = Math::Matrix::CreateRotationY(m_rot.y);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(5.0f);
	
	m_mWorld = scaleMat * rotMat * transMat;
	if (m_pos.x > 100.0f)//右端に到達したら
	{
		m_gameScene->SetNextStage();
	}
}

void SideChara::PostUpdate()
{
	//KdCollider::BoxInfo box; 
	KdCollider::SphereInfo sphere;

	//球の中心座標を設定
	sphere.m_sphere.Center = m_pos;

	//球の半径を設定
	sphere.m_sphere.Radius = 0.8f;

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
	static const float enableStepHigh = 0.1f;
	static const float m_adjustHeight = 1.0f;
	ray.m_pos.y += enableStepHigh;
	ray.m_pos.y -= m_adjustHeight;
	ray.m_dir = Math::Vector3(0, -1, 0);
	ray.m_range = 0.1f+m_gravity;

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
		m_pos.y += m_adjustHeight;	// 地面から補正値分上げる
		m_gravity = 0;
		m_dir.y = 0;
		m_jumpFlg = true;
	}
}

void SideChara::GenerateDepthMapFromLight()
{
	if (m_gameScene->GetNowChara() == GameScene::NowChara::Top) 
	{
		KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
	}
	else if (m_gameScene->GetNowChara() == GameScene::NowChara::Side) 
	{
		KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
	}
}

void SideChara::DrawLit()
{
	if (m_gameScene->GetNowChara() == GameScene::NowChara::Top)
	{
		KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
	}
	else if (m_gameScene->GetNowChara() == GameScene::NowChara::Side)
	{
		KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
	}
	
}
