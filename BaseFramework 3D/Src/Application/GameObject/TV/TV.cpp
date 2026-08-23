#include "TV.h"

void TV::Init()
{
	m_model = std::make_shared<KdModelData>();
	m_model->Load("Asset/Models/Tv/TV.gltf");
	Math::Matrix transMat = Math::Matrix::CreateTranslation(Math::Vector3(0.0f, -6.0f, -14.0f));
	m_mWorld = Math::Matrix::CreateScale(10.0f)*transMat;
}

void TV::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}
