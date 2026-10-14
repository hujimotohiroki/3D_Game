#include "Controller.h"

void Controller::Init()
{
	m_model = std::make_shared<KdModelData>();
	m_model->Load("Asset/Models/Tv/Controller.gltf");
	Math::Matrix transMat = Math::Matrix::CreateTranslation(Math::Vector3(0.0f,0.0f, -18.75f));
	Math::Matrix rotMatX = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(-30.0f));
	Math::Matrix rotMatY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(90.0f));
	m_mWorld = rotMatX * rotMatY * transMat;
}

void Controller::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}
