#include "TopChara.h"
#include "../../../Scene/GameScene/GameScene.h"

void TopChara::Init()
{
	m_topmodel = std::make_shared<KdModelData>();
	m_topmodel->Load("Asset/tmp/Crystal.gltf");
	m_sidemodel = std::make_shared<KdModelData>();
	m_sidemodel->Load("Asset/tmp/Crystal.gltf");
}

void TopChara::Update()
{
	if (m_gameScene->GetNowChara() != GameScene::NowChara::Top) return;
	m_dir = Math::Vector3::Zero;
	bool moveFlg=false;
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
	if(!moveFlg)
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
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos * 2);
	Math::Matrix rotMat = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(-90));
	m_mWorld = rotMat * transMat;
}

void TopChara::PostUpdate()
{
	/*if (m_atkFlg == false) return;
	//当たり判定（アイテムとの）
	KdCollider::SphereInfo sphere;
	sphere.m_sphere.Center = GetPos();
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
		KdShaderManager::Instance().m_StandardShader.DrawModel(*m_sidemodel, m_mWorld);
	}
	else if (m_gameScene->GetNowChara() == GameScene::NowChara::Top)
	{
		KdShaderManager::Instance().m_StandardShader.DrawModel(*m_topmodel, m_mWorld);
	}
}

void TopChara::DrawLit()
{
	if (m_gameScene->GetNowChara() == GameScene::NowChara::Side)
	{
		KdShaderManager::Instance().m_StandardShader.DrawModel(*m_sidemodel, m_mWorld);
	}
	else if (m_gameScene->GetNowChara() == GameScene::NowChara::Top)
	{
		KdShaderManager::Instance().m_StandardShader.DrawModel(*m_topmodel, m_mWorld);
	}
}
