#include "Press Enter.h"

void PressEnter::Init()
{
	m_model = std::make_shared<KdModelData>();
	m_model->Load("Asset/Models/Title/Press Enter.gltf");
	Math::Matrix transMat = Math::Matrix::CreateTranslation(Math::Vector3(0.0f, 0.0f, -14.0f));
	Math::Matrix rotMat = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(-90.0f));
	m_mWorld = rotMat * transMat;
}

void PressEnter::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}
