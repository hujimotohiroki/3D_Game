#include "SideChara.h"
#include "../../../Scene/GameScene/GameScene.h"
#include "../../../Scene/SceneManager.h"

void SideChara::Init()
{
	m_model = std::make_shared<KdModelWork>();
	m_model->SetModelData("Asset/Models/Human/low_poly_humans.gltf");
	m_spAnimator = std::make_shared<KdAnimator>();
	m_spAnimator->SetAnimation(m_model->GetAnimation("Walk"));
	m_spAnimator2 = std::make_shared<KdAnimator>();
	m_spAnimator2->SetAnimation(m_model->GetAnimation("Stop"));

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void SideChara::Update()
{
	if (m_gameScene->GetNowChara() != GameScene::NowChara::Side) return;
	bool moveFlg = false;
	m_dir.x = 0;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		m_dir.x = -0.5;
		m_rot.y = 90;
		moveFlg = true;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		m_dir.x = 0.5;
		m_rot.y = -90;
		moveFlg = true;
	}
	if (GetAsyncKeyState(VK_UP) & 0x8000) {
		if(m_jumpFlg)
		{
			m_dir.y = 1.2;
		}
		m_jumpFlg = false;
	}
	m_pos += m_dir * speed;
	m_gravity += m_gravityacc;
	m_pos.y -= m_gravity;
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix rotMat = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_rot.y));
	Math::Matrix scaleMat = Math::Matrix::CreateScale(0.75f);
	
	m_mWorld = scaleMat * rotMat * transMat;
	
	if(m_jumpFlg&&moveFlg)
	{
		m_spAnimator->AdvanceTime(m_model->WorkNodes());
		m_model->CalcNodeMatrices();
	}
	else
	{
		m_spAnimator2->AdvanceTime(m_model->WorkNodes());
		m_model->CalcNodeMatrices();
	}
	
}

void SideChara::PostUpdate()
{

	// 立方体の当たり判定を行う
	// DirectX::BoundingOrientedBox を作成して BoxInfo のコンストラクタで初期化する
	DirectX::BoundingOrientedBox obb;
	obb.Center = m_pos;
	obb.Extents = Math::Vector3(0.5f, 1.5f, 0.5f);
	// 地面判定対象（TypeGround）と衝突して重ならないようにする
	KdCollider::BoxInfo box(KdCollider::TypeBump, obb);

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
		if (maxBoxOverlap < ret.m_overlapDistance) {
			maxBoxOverlap = ret.m_overlapDistance;
			boxHitDir = ret.m_hitDir;
			// 横方向（X）のみで押し戻す。上下の補正はレイによる着地処理で行う
			boxHitDir.y = 0;
			boxHitDir.z = 0; // Z軸固定
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
	sphere.m_sphere.Center.y -= 0.9f;
	sphere.m_sphere.Radius = 0.5f;
	sphere.m_type = KdCollider::TypeGround;

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
			sphereHitDir.z = 0;
			sphereHit = true;
		}
	}

	// 球で当たっているが BOX では検出できていない場合は球の結果で押し戻す
	if (!boxHit && sphereHit) {
		if (sphereHitDir.LengthSquared() > 0.0f) {
			sphereHitDir.Normalize();
			m_pos += sphereHitDir * maxSphereOverlap;
			boxHit = true; // 色表示のためヒットと見なす
		}
	}

	// デバッグ可視化: ヒットしている場合は赤、していない場合は緑でボックス表示
	/*
	if (m_pDebugWire) {
		Math::Color col = boxHit ? Math::Color(1, 0, 0, 1) : Math::Color(0, 1, 0, 1);
		m_pDebugWire->AddDebugBox(Math::Matrix::CreateTranslation(m_pos), Math::Vector3(0.5f, 1.0f, 0.5f), Math::Vector3::Zero, true, col);
	}
	KdCollider::SphereInfo sphere;

	//球の中心座標を設定
	sphere.m_sphere.Center = m_pos;
	sphere.m_sphere.Center.y -= 0.9f;

	//球の半径を設定
	sphere.m_sphere.Radius = 0.4f;

	//当たり判定をしたいタイプを設定
	sphere.m_type = KdCollider::TypeGround;
	
	//デバッグ用
	//m_pDebugWire->AddDebugSphere(sphere.m_sphere.Center, sphere.m_sphere.Radius, kRedColor);

	//Z軸固定忘れずに
	// 球に当たったオブジェクト情報を格納するリスト
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
		
	}*/
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
	if (m_pos.x > 20.0f)//右端に到達したら
	{
		m_gameScene->SetNextStage();
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
		KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
	}
	else if (m_gameScene->GetNowChara() == GameScene::NowChara::Side)
	{
		KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
	}
	
}
