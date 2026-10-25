#include "goal.h"
#include "../../../Scene/GameScene/GameScene.h"
#include "../../../Scene/SceneManager.h"
#include "../Stage.h"
void goal::Init()
{
	m_model = std::make_shared<KdModelData>();
	m_model->Load("Asset/Models/Goal/goal.gltf");
	/*m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("GroundCollision", m_model, KdCollider::TypeGround );*/
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void goal::Update()
{
	DirectX::BoundingBox goalBox;
	goalBox.Center= m_pos;
	goalBox.Extents = Math::Vector3(8.0f, 8.0f, 8.0f);
	KdCollider::BoxInfo boxInfo(
		KdCollider::TypeBump,
		goalBox
	);

	bool tmp = false;
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		if (obj.get() == this) continue;
		bool hit = obj->Intersects(boxInfo, nullptr);
		if (!hit)continue;
		tmp = true;
		if (!hitflg) 
		{
			hitflg = true;
			m_stage->AddGoal();
			KdDebugGUI::Instance().AddLog("GoalHit");
		}
	}
	if (!tmp&&hitflg)
	{
		hitflg = false;
		m_stage->SubtractGoal();
	}
	//m_pDebugWire->AddDebugBox(Math::Matrix::CreateTranslation(m_pos), Math::Vector3(8.0f, 8.0f, 8.0f), Math::Vector3::Zero, true, Math::Color(0, 1, 0, 1));
	m_mWorld = Math::Matrix::CreateScale(4) * Math::Matrix::CreateTranslation(m_pos);
	//m_pDebugWire->AddDebugBox(Math::Matrix::CreateTranslation(groundBox.Center), Math::Vector3(40.0f, 4.0f, 40.0f), Math::Vector3::Zero, true, Math::Color(0, 1, 0, 1));
}

/*void TopGround::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}*/

void goal::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}
