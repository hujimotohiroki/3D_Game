#include "Stage Clear.h"

void StageClear::Init()
{
	m_model = std::make_shared<KdModelData>();
	m_model->Load("Asset/Models/Title/Stage Clear.gltf");
	Math::Matrix transMat = Math::Matrix::CreateTranslation(Math::Vector3(0.0f, 0.0f, 0.0f));
	Math::Matrix rotMat = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(-90.0f));
	Math::Matrix scaleMat = Math::Matrix::CreateScale(16.0f);
	m_mWorld = scaleMat * rotMat * transMat;
}

void StageClear::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}
