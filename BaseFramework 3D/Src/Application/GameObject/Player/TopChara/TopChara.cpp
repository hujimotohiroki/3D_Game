#include "TopChara.h"
#include "../../../Scene/GameScene/GameScene.h"
#include "../../../Scene/SceneManager.h"
#include "../../TV/SandStorm.h"

void TopChara::Init()
{
	m_model = std::make_shared<KdModelWork>();
	m_model->SetModelData("Asset/Models/Fairy/fairy.gltf");
	
	m_mWorld = Math::Matrix::CreateTranslation(Math::Vector3{ 0, 0, -100});
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void TopChara::Update()
{
	if (m_gameScene->GetNowChara() != GameScene::NowChara::Top) return;
	m_dir = Math::Vector3::Zero;
	bool moveFlg = false;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		/*
		x--;
		if(x < 0) {
			x = 0;
		}
		*/
		m_dir.x = -1;
		moveFlg = true;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		/*
		x++;
		if(x > ?) {
			x =?;
		}
		*/
		m_dir.x = 1;
		moveFlg = true;
	}
	if (GetAsyncKeyState(VK_UP) & 0x8000) {
		/*
		y--;
		if(y < 0) {
			y = 0;
		}
		*/
		m_dir.y = 1;
		moveFlg = true;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		/*
		y++;
		if(y > ?) {
			y =?;
		}
		*/
		m_dir.y = -1;
		moveFlg = true;
	}
	m_dir.Normalize();
	m_pos += m_dir * speed;
	m_gravity += m_gravityacc;
	//m_pos.z += m_gravity;
	if (!moveFlg)
	{
		Math::Vector3 to_pos = Math::Vector3::Zero;
		to_pos.x = round(m_pos.x/2.0f)*2.0f;
		to_pos.y = round(m_pos.y/2.0f)*2.0f;
		m_dir = to_pos - m_pos;
		m_dir.z = 0;
		if (m_dir.Length() < speed) {
			m_pos.x = to_pos.x;
			m_pos.y = to_pos.y;
		}
		else {
			m_dir.Normalize();
			m_pos += m_dir * speed;
		}
	}

	if (moveFlg) {
		toDir = m_dir;
	}
		//簡単にオブジェクトの向きを取得する方法
		Math::Vector3 nowDir = m_mWorld.Backward();
		KdDebugGUI::Instance().AddLog("nowDir : x:%f y:%f z:%f\n", nowDir.x, nowDir.y, nowDir.z);
		nowDir.Normalize();
		//向きたい方向
		
		KdDebugGUI::Instance().AddLog("toDir : x:%f y:%f z:%f\n", toDir.x, toDir.y, toDir.z);
		// 内積とは＝ベクトルA×ベクトルB×cos(なす角)
		float dot = nowDir.Dot(toDir);
		//角度に変換
		float angle = DirectX::XMConvertToDegrees(acos(dot));

		if (angle >= 0.1f) {
			if (angle > 20) {
				angle = 20;
			}
			//外積を求める：２本のベクトルに対し垂直なベクトル
			Math::Vector3 cross = nowDir.Cross(toDir);
			if (cross.z >= 0) {
				m_angle += angle;
				if (m_angle > 360) {
					m_angle -= 360;
				}
			}
			else {
				m_angle -= angle;
				if (m_angle < -360) {
					m_angle += 360;
				}
			}
		}
	KdDebugGUI::Instance().AddLog("angle : %f\n", m_angle);
	//Math::Matrix rotMatZ = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(m_angle));
	//m_pos.z = 0;
	//m_pos.z = -20.5f;
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix rotMatX = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(-90));
	Math::Matrix scaleMat = Math::Matrix::CreateScale(1);
	m_mWorld = scaleMat * rotMatX * transMat;
	if (m_pos.z > 2)
	{
		std::shared_ptr<SandStorm> sandstorm = std::make_shared<SandStorm>();
		sandstorm->Init();
		sandstorm->SetGameScene(m_gameScene);
		SceneManager::Instance().AddObject(sandstorm);
	}
}

void TopChara::PostUpdate()
{
	/*if (m_atkFlg == false) return;
	//当たり判定（アイテムとの）
	KdCollider::SphereInfo sphere;
	sphere.m_sphere.Center = GetPos()*2;
	sphere.m_sphere.Radius = 0.5f;
	sphere.m_type = KdCollider::TypeGround;

	//デバッグ
	m_pDebugWire->AddDebugSphere(sphere.m_sphere.Center, sphere.m_sphere.Radius, kRedColor);

	//当たったオブジェクト情報を格納するリストは不要

	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		if (obj->Intersects(sphere, nullptr) == true) {
			obj->OnHit();
		}
	}*/
	KdCollider::RayInfo ray;

	ray.m_pos = m_pos;
	static const float enableStepHigh = 1.0f;
	static const float m_adjustHeight = 1.0f;
	ray.m_pos.z -= enableStepHigh;
	ray.m_pos.z += m_adjustHeight;
	ray.m_dir = Math::Vector3(0, 0, 1);
	ray.m_range = 1.0f + m_gravity;

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
		m_pos.z -= m_adjustHeight;	// 地面から補正値分上げる
		m_gravity = 0;
	}
}
void TopChara::GenerateDepthMapFromLight()
{
	/*if (m_gameScene->GetNowChara() == GameScene::NowChara::Side)
	{
		KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
	}
	else if (m_gameScene->GetNowChara() == GameScene::NowChara::Top)
	{
		KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
	}*/
}

void TopChara::DrawLit()
{
	/*if (m_gameScene->GetNowChara() == GameScene::NowChara::Side)
	{
		KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
	}
	else if (m_gameScene->GetNowChara() == GameScene::NowChara::Top)
	{
		KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
	}*/
}
