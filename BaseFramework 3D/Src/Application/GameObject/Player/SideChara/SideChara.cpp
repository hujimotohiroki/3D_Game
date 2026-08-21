#include "SideChara.h"
#include "../../../Scene/GameScene/GameScene.h"
#include "../../../Scene/SceneManager.h"

void SideChara::Init()
{
	m_model = std::make_shared<KdModelData>();
	m_model->Load("Asset/tmp/Crystal.gltf");

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
			m_dir.y = 1.75;
		}
		m_jumpFlg = false;
	}
	m_pos += m_dir * speed;
	m_gravity += m_gravityacc;
	m_dir.y -= m_gravity;
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix rotMat = Math::Matrix::CreateRotationY(m_rot.y);
	
	m_mWorld = rotMat*transMat;
	if (m_pos.x > 10.0f)//右端に到達したら
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
