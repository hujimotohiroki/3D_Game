#pragma once

class GameScene;

class StageObjectBase : public KdGameObject
{
public:
	void SetGameScene(GameScene* scene) { m_gameScene = scene; }
protected:
	std::shared_ptr<KdModelData> m_model;
	GameScene* m_gameScene = nullptr;
};