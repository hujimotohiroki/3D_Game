#pragma once
class GameScene;
class PlayerBase :public KdGameObject 
{
public:
	Math::Vector3 GetPos() { return m_pos; }
	void SetGameScene(GameScene* scene) { m_gameScene = scene; }
protected:
	GameScene* m_gameScene = nullptr;
	std::shared_ptr<KdModelData> m_model;
	Math::Vector3 m_pos = Math::Vector3::Zero;
	Math::Vector3 m_dir = Math::Vector3::Zero;
	Math::Vector3 m_rot = Math::Vector3::Zero;
};