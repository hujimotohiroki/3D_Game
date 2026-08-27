#pragma once

class TV :public KdGameObject
{
public:
	void Init() override;
	void DrawLit()override;

	void SetPos(const Math::Vector3& pos) override { m_pos = pos; }
	void SetRotateY(float angle) { m_rot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(angle)); }
private:
	Math::Vector3 m_pos;
	Math::Matrix m_rot;
	std::shared_ptr<KdModelData> m_model;
};