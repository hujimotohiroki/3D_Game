#include "TV.h"

void TV::Init()
{
	m_model = std::make_shared<KdModelData>();
	m_model->Load("Asset/Models/Tv/TV.gltf");
	
}

void TV::DrawLit()
{
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix rotMat = m_rot;
	m_mWorld = Math::Matrix::CreateScale(10.0f) * rotMat * transMat;
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}
