#include "Title.h"

void Title::Init()
{
	m_model = std::make_shared<KdModelData>();
	m_model->Load("Asset/Models/Title/Title.gltf");
	Math::Matrix transMat = Math::Matrix::CreateTranslation(Math::Vector3(-0.5f, 2.0f, -14.0f));
	Math::Matrix rotMat = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(-90.0f));
	m_mWorld = rotMat * transMat;
}

void Title::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}
