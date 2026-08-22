#pragma once

class GameScene;

class StageObjectBase : public KdGameObject
{
public:
	void SetGameScene(GameScene* scene) { m_gameScene = scene; }
	void SetPos(const Math::Vector3& pos) { m_pos = pos; }
protected:
	std::shared_ptr<KdModelData> m_model;
	GameScene* m_gameScene = nullptr;
	Math::Vector3 m_pos = Math::Vector3::Zero;
};