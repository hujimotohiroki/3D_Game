#include "TopChara.h"
#include "../../../Scene/GameScene/GameScene.h"

void TopChara::Init()
{
	m_model = std::make_shared<KdModelData>();
	m_model->Load("Asset/Models/TopChara/3D Gum Bot/3D Gum Bot.gltf");
}

void TopChara::Update()
{
	if (m_gameScene->GetNowChara() != GameScene::NowChara::Top) return;
	m_dir = Math::Vector3::Zero;
	bool moveFlg = false;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		m_dir.x = -1;
		moveFlg = true;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		m_dir.x = 1;
		moveFlg = true;
	}
	if (GetAsyncKeyState(VK_UP) & 0x8000) {
		m_dir.y = 1;
		moveFlg = true;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		m_dir.y = -1;
		moveFlg = true;
	}
	m_dir.Normalize();
	m_pos += m_dir * speed;
	if (!moveFlg)
	{
		Math::Vector3 to_pos = Math::Vector3::Zero;
		to_pos.x = round(m_pos.x);
		to_pos.y = round(m_pos.y);
		m_dir = to_pos - m_pos;
		if (m_dir.Length() < speed) {
			m_pos = to_pos;
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
	Math::Matrix rotMatZ = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(m_angle));

	m_pos.z = 1;
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos * 2);
	Math::Matrix rotMatX = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(-90));
	Math::Matrix scaleMat = Math::Matrix::CreateScale(5);
	m_mWorld = scaleMat * rotMatX * rotMatZ * transMat;
}

void TopChara::PostUpdate()
{
	/*if (m_atkFlg == false) return;
	//当たり判定（アイテムとの）
	KdCollider::SphereInfo sphere;
	sphere.m_sphere.Center = GetPos()*2;
	sphere.m_sphere.Radius = 1.0f;
	sphere.m_type = KdCollider::TypeDamage;

	//デバッグ
	m_pDebugWire->AddDebugSphere(sphere.m_sphere.Center, sphere.m_sphere.Radius, kRedColor);

	//当たったオブジェクト情報を格納するリストは不要

	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		if (obj->Intersects(sphere, nullptr) == true) {
			obj->OnHit();
		}
	}*/
}
void TopChara::GenerateDepthMapFromLight()
{
	if (m_gameScene->GetNowChara() == GameScene::NowChara::Side)
	{
		KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
	}
	else if (m_gameScene->GetNowChara() == GameScene::NowChara::Top)
	{
		KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
	}
}

void TopChara::DrawLit()
{
	if (m_gameScene->GetNowChara() == GameScene::NowChara::Side)
	{
		KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
	}
	else if (m_gameScene->GetNowChara() == GameScene::NowChara::Top)
	{
		KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
	}
}
