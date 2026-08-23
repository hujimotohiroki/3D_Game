#include "REC.h"

void REC::Init()
{
	m_model = std::make_shared<KdModelData>();
	m_model->Load("Asset/Models/Tv/REC.gltf");
	Math::Matrix transMat = Math::Matrix::CreateTranslation(Math::Vector3(-4.5f, 3.5f, -14.0f));
	Math::Matrix rotMat = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(-90.0f));
	m_mWorld = rotMat * transMat;
}

void REC::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}
