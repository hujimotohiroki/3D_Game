#include "SideBlock2.h"
#include "TopBlock.h"
#include "../../../Scene/GameScene/GameScene.h"
#include "../../../Scene/SceneManager.h"
#include <cmath>

void SideBlock2::Init()
{
	m_model = std::make_shared<KdModelData>();
	m_model->Load("Asset/Models/Block/SideBlock2.gltf");
	blockBox.Center = m_pos;
	blockBox.Extents = Math::Vector3(1.99f, 5.97f,1.99f);
	blockBox.Orientation = Math::Quaternion::Identity;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape(
		"BlockCollision",
		blockBox,
		KdCollider::TypeBump
	);
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
	RotMat = Math::Matrix::Identity;
	
}

void SideBlock2::Update()
{
	
	m_gravity += m_gravityacc;
	m_pos.y -= m_gravity;

	if(rollFlg!=0)
	{
		switch (rollFlg)
		{
		case 1:
			m_pos.x -= 0.8f;
			RotMat *= Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(4.5f));
			break;
		case 2:
			m_pos.z -= 0.8f;
			RotMat *= Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(-4.5f));
			break;
		case 3:
			m_pos.x += 0.8f;
			RotMat *= Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(-4.5f));
			break;
		case 4:
			m_pos.z += 0.8f;
			RotMat *= Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(4.5f));
			break;
		case -1:
			m_pos.x += 0.8f;
			RotMat *= Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(-4.5f));
			break;
		case -2:
			m_pos.z += 0.8f;
			RotMat *= Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(4.5f));
			break;
		case -3:
			m_pos.x -= 0.8f;
			RotMat *= Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(4.5f));
			break;
		case -4:
			m_pos.z -= 0.8f;
			RotMat *= Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(-4.5f));
			break;
		}
		rollCount+=rollFlg/abs(rollFlg);
		if (rollCount == 20|| rollCount == 0) {
			rollFlg = 0;
			rollCount = 0;
		}
		
		KdCollider::RayInfo ray;

		ray.m_pos = m_pos;
		static const float enableStepHigh = 0.3f;
		static const float m_adjustHeight = 4.0f;
		ray.m_pos.y += enableStepHigh;
		ray.m_pos.y -= m_adjustHeight;
		ray.m_dir = Math::Vector3(0, 1, 0);
		ray.m_range = 12.0f;

		//当たり判定をしたいタイプを設定
		ray.m_type = KdCollider::TypeGround;

		//デバッグ用
		//m_pDebugWire->AddDebugLine(ray.m_pos, ray.m_dir, ray.m_range, Math::Color(1, 0, 0, 1));

		//レイに当たったオブジェクト情報を格納するリスト
		std::list<KdCollider::CollisionResult> retRayList;
		for (auto& obj : SceneManager::Instance().GetObjList()) {
			if (obj.get() == this)
			{
				continue;
			}
			obj->Intersects(ray, &retRayList);

		}

		for (auto& ret : retRayList)
		{
			rollFlg *= -1;
		}
	}
	//KdDebugGUI::Instance().AddLog("%d\n", rollFlg);
	
	m_mWorld = Math::Matrix::CreateScale(4)*RotMat* Math::Matrix::CreateTranslation(m_pos);
}

void SideBlock2::PostUpdate()
{
	blockBox.Center = m_pos;
	blockBox.Extents = m_collisionHalfSize;
	blockBox.Orientation =
		Math::Quaternion::CreateFromRotationMatrix(RotMat);
	
	
	KdCollider::BoxInfo boxInfo(
		KdCollider::TypeGround|KdCollider::TypeBump,
		blockBox
	);
	
	std::list<KdCollider::CollisionResult> results;
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		if (obj.get() == this) continue;
		 obj->Intersects(boxInfo, &results);
	}
	float maxOverlap = 0.0f;
	Math::Vector3 pushDirection = Math::Vector3::Zero;

	for (const auto& result : results)
	{
		if (result.m_overlapDistance > maxOverlap)
		{
			maxOverlap = result.m_overlapDistance;
			pushDirection = result.m_hitDir;
			//KdDebugGUI::Instance().AddLog("%f\n", maxOverlap);
		}
	}

	if (maxOverlap > 0.0f)
	{
		m_pos += pushDirection * maxOverlap;
		m_gravity = 0.0f;

		m_mWorld =
			Math::Matrix::CreateScale(4.0f) *
			RotMat *
			Math::Matrix::CreateTranslation(m_pos);
	}
	
}

/*void SideBlock::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}*/

void SideBlock2::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}

void SideBlock2::PushBlock(int Flg)
{
	KdCollider::RayInfo ray;

	ray.m_pos = m_pos;
	static const float enableStepHigh = 0.3f;
	ray.m_pos.y += enableStepHigh;
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
	//m_pDebugWire->AddDebugLine(ray.m_pos, ray.m_dir, ray.m_range, Math::Color(1, 0, 0, 1));

	//レイに当たったオブジェクト情報を格納するリスト
	std::list<KdCollider::CollisionResult> retRayList;
	for (auto& obj : SceneManager::Instance().GetObjList()) {
		if (obj.get() == this)
		{
			continue;
		}
		obj->Intersects(ray, &retRayList);
	}
	//KdDebugGUI::Instance().AddLog("%d\n",retRayList.size());
	// 当たっていなかったら
	if (retRayList.empty()&&rollFlg==0)
	{
		rollFlg = Flg+1;
		rollCount = 0;
	}
	
}
//縦移動距離と横移動距離の合計が常に一致するからそれでうまいこと行けないか？
//Boxの最下点からレイを飛ばして、当らなかったらその高さに合わせる。で、横移動はそのまま。
//当たったら一個上のマスから飛ばす
