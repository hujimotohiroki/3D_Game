#pragma once

class Controller :public KdGameObject
{
public:
	void Init() override;
	void DrawLit()override;
private:
	std::shared_ptr<KdModelData> m_model;
};