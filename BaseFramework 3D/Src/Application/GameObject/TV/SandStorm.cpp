#include "SandStorm.h"
#include "../../Scene/GameScene/GameScene.h"
void SandStorm::Init()
{
	m_model = std::make_shared<KdModelData>();
	m_model->Load("Asset/Models/Tv/SandStorm.gltf");
	
}

void SandStorm::Update()
{
	timer++;
	if (timer > 60)
	{
		m_gameScene->SetResetFlg();
	}
	Math::Matrix transMat = Math::Matrix::CreateTranslation(Math::Vector3(rand()%20-10, 1.0f, -14.0f));
	Math::Matrix rotMat = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(0));
	Math::Matrix scaleMat = Math::Matrix::CreateScale(4.0f);
	m_mWorld = scaleMat * rotMat * transMat;
}

void SandStorm::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}
