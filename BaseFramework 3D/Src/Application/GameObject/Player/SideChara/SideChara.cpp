#include "SideChara.h"
#include "../../../Scene/GameScene/GameScene.h"
#include "../../../Scene/SceneManager.h"
#include "../../Camera/FPSCamera/FPSCamera.h"
#include "../../Stage/Block/SideBlock.h"
#include "../../Stage/Block/SideBlock2.h"
void SideChara::Init()
{
	m_model = std::make_shared<KdModelWork>();
	//m_model->SetModelData("Asset/Models/Human/low_poly_humans.gltf");
	m_model->SetModelData("Asset/Models/Human/wand.gltf");
	//m_spAnimator = std::make_shared<KdAnimator>();
	//m_spAnimator->SetAnimation(m_model->GetAnimation("Swing"));
	/*m_spAnimator2 = std::make_shared<KdAnimator>();
	m_spAnimator2->SetAnimation(m_model->GetAnimation("Stop"));*/
	m_pCollider = std::make_unique<KdCollider>();
	// プレイヤーがブロックのBox/球いずれの判定にも反応するように
	// TypeBump と TypeGround を両方登録する（ビットフラグの合成）
	m_pCollider->RegisterCollisionShape("CharaCollision", m_model, KdCollider::TypeBump);
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
	m_mWorld = Math::Matrix::CreateTranslation(Math::Vector3{ 0, 0, -100 });

	paperFlg = false;
}

void SideChara::Update()
{
	if (m_gameScene->GetNowChara() != GameScene::NowChara::Side) return;
	Math::Matrix cameraRotMat;
	if (m_wpCamera.expired() == false)
	{
		cameraRotMat = m_wpCamera.lock()->GetRotationMatrix();
	}
	bool moveFlg = false;
	m_dir.x = 0;
	m_dir.z = 0;
	if (GetAsyncKeyState('W') & 0x8000) {
		//前
		m_dir += Math::Vector3::TransformNormal(Math::Vector3(0, 0, 1), cameraRotMat);
		moveFlg = true;
	}
	if (GetAsyncKeyState('A') & 0x8000) {
		//左
		m_dir += Math::Vector3::TransformNormal(Math::Vector3(-1, 0, 0), cameraRotMat);
		moveFlg = true;
	}
	if (GetAsyncKeyState('S') & 0x8000) {
		//後
		m_dir += Math::Vector3::TransformNormal(Math::Vector3(0, 0, -1), cameraRotMat);
		moveFlg = true;
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		//右
		m_dir += Math::Vector3::TransformNormal(Math::Vector3(1, 0, 0), cameraRotMat);
		moveFlg = true;
	}
	//Eキーを押したらレイをとばしてそれがブロックに当たった場合ブロックを押す処理が発生する
	if (GetAsyncKeyState('E') & 0x8000)
	{
		if (ETimer == 0)
		{
			KdCollider::RayInfo ray;

			ray.m_pos = m_pos;
			ray.m_pos.y += 8.0f;
			ray.m_dir = cameraRotMat.Backward();
			ray.m_range = 40.0f;

			//当たり判定をしたいタイプを設定
			ray.m_type = KdCollider::TypeBump;

			// 各オブジェクトごとに当たり判定を行い、ヒットした中で一番近いオブジェクトを探す
			float maxOverLap = 0.0f;
			Math::Vector3 groundPos = {};    // レイが遮断された(Hitした)座標
			std::shared_ptr<KdGameObject> hitObj = nullptr;    // ヒットしたオブジェクト（最も近いもの）

			for (auto& obj : SceneManager::Instance().GetObjList()) {
				if (obj.get() == this)
				{
					continue;
				}

				// オブジェクトごとに結果リストを作る
				std::list<KdCollider::CollisionResult> tmpResults;
				obj->Intersects(ray, &tmpResults);

				// そのオブジェクトへのヒット結果の中で一番重なり（=近い）ものをチェック
				for (auto& r : tmpResults) {
					if (maxOverLap < r.m_overlapDistance) {
						maxOverLap = r.m_overlapDistance;
						groundPos = r.m_hitPos;
						hitObj = obj; // 最も近いオブジェクトを保持
					}
				}
			}
			if (hitObj) {
				SideBlock* pSideBlock = dynamic_cast<SideBlock*>(hitObj.get());
				Math::Vector3 BlockSize = pSideBlock->GetBlockSize();
				if (pSideBlock)
				{
					Math::Vector3 p_pos = pSideBlock->GetPos();
					int dirFlg;
					float xdiff =groundPos.x - p_pos.x;
					float ydiff =groundPos.y - p_pos.y;
					float zdiff =groundPos.z - p_pos.z;
					if (abs(BlockSize.x - abs(xdiff)) > abs(BlockSize.z - abs(zdiff))) 
					{
						if (abs(BlockSize.z - abs(zdiff)) > abs(BlockSize.y - abs(ydiff))) 
						{
							dirFlg = -1;
						}
						else 
						{
							if (zdiff > 0) dirFlg = 1;
							else dirFlg = 3;
						}
					}
					else 
					{
						if (abs(BlockSize.x - abs(xdiff)) > abs(BlockSize.y - abs(ydiff))) 
						{
							dirFlg = -1;
						}
						else 
						{
							if (xdiff > 0) dirFlg = 0;
							else dirFlg = 2;
						}
					}
					if (dirFlg != -1)
					{
						pSideBlock->PushBlock(dirFlg);
					}
				}
			}
			// デバッグ表示（ヒットしたら赤、していなければ緑）
			bool hit = (hitObj != nullptr);
			Math::Color col = hit ? Math::Color(1, 0, 0, 1) : Math::Color(0, 1, 0, 1);
			//m_pDebugWire->AddDebugLine(ray.m_pos, ray.m_dir, ray.m_range, col);
			//m_spAnimator->AdvanceTime(m_model->WorkNodes());
			ETimer = 60;
		}
	}
	if (ETimer != 0) ETimer--;
	if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
		if (m_gravity == 0) {
			m_gravity = -0.5f;
		}
	}
		//方向ベクトルを強制的に長さ１にする
		//正規化
	if(moveFlg){
		m_dir.Normalize();

		Math::Vector3 nowDir = m_mWorld.Backward();
		//これだけで正面ベクトルが取れる

		//向きたい方向
		Math::Vector3 toDir = Math::Vector3::TransformNormal(Math::Vector3(0, 0, 1), cameraRotMat);
		nowDir.Normalize();
		toDir.Normalize();

		// 内積とは＝ベクトルA×ベクトルB×cos(なす角)
		float dot = nowDir.Dot(toDir);
		//角度に変換
		float angle = DirectX::XMConvertToDegrees(acos(dot));
		if (angle >= 1.0f) {
			if (angle > 5.0f) {
				angle = 5.0f;
			}
			//外積を求める：２本のベクトルに対し垂直なベクトル
			Math::Vector3 cross = nowDir.Cross(toDir);
			if (cross.y >= 0) {
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
	}
	m_dir.y = 0;
	Math::Matrix rotMat = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle));
	Math::Matrix localrotMat = cameraRotMat;
	m_pos += m_dir * speed;
	m_gravity += m_gravityacc;
	m_pos.y -= m_gravity;
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix localposMat = Math::Matrix::CreateTranslation(localPos);
	Math::Matrix localposMatY = Math::Matrix::CreateTranslation(localPosY);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(2.00f);
	
	m_mWorld = scaleMat * rotMat * transMat;
	m_mlocalWorld = scaleMat * localposMat* localrotMat  * localposMatY * transMat;
	
	/*if (m_jumpFlg && moveFlg)
	{
		m_spAnimator->AdvanceTime(m_model->WorkNodes());
		m_model->CalcNodeMatrices();
	}
	else
	{
		m_spAnimator2->AdvanceTime(m_model->WorkNodes());
		m_model->CalcNodeMatrices();
	}*/
	
}

void SideChara::PostUpdate()
{

	if (m_gameScene->GetNowChara() != GameScene::NowChara::Side) return;
	Math::Matrix cameraRotMat;
	if (m_wpCamera.expired() == false)
	{
		cameraRotMat = m_wpCamera.lock()->GetRotationYMatrix();
	}

	DirectX::BoundingOrientedBox blockBox;

	blockBox.Center = m_pos;
	blockBox.Extents = Math::Vector3(4.0f, 4.0f, 4.0f);
	blockBox.Orientation =
		Math::Quaternion::CreateFromRotationMatrix(cameraRotMat);


	KdCollider::BoxInfo boxInfo(
		KdCollider::TypeGround | KdCollider::TypeBump,
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

		Math::Matrix rotMat = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle));
		Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
		Math::Matrix scaleMat = Math::Matrix::CreateScale(2.00f);

		m_mWorld = scaleMat * rotMat * transMat;
	}
	
	KdCollider::RayInfo ray;

	ray.m_pos = m_pos;
	static const float enableStepHigh = 0.1f;
	static const float m_adjustHeight = 1.5f;
	ray.m_pos.y += enableStepHigh;
	ray.m_pos.y -= m_adjustHeight;
	ray.m_dir = Math::Vector3(0, -1, 0);
	ray.m_range = 0.2f+m_gravity;

	//当たり判定をしたいタイプを設定
	ray.m_type = KdCollider::TypeGround;

	//デバッグ用
	//m_pDebugWire->AddDebugLine(ray.m_pos, ray.m_dir, ray.m_range, Math::Color(1, 0, 0, 1));

	//Z軸固定忘れずに
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
		m_gravity = 0;
		m_dir.y = 0;
		m_jumpFlg = true;
	}
	
}

void SideChara::GenerateDepthMapFromLight()
{
	if (m_gameScene->GetNowChara() == GameScene::NowChara::Top) 
	{
		//KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
	}
	else if (m_gameScene->GetNowChara() == GameScene::NowChara::Side) 
	{
		//KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
	}
}

void SideChara::DrawLit()
{
	if (m_gameScene->GetNowChara() == GameScene::NowChara::Top)
	{
		KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mlocalWorld);
	}
	else if (m_gameScene->GetNowChara() == GameScene::NowChara::Side)
	{
		KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mlocalWorld);
	}
	
}
