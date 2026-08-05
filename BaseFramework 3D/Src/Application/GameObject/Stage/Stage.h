#pragma once

class GameScene;

class Stage :public KdGameObject
{
public:
	
	void Init()override;

	Math::Vector3 GetPos() { return m_pos; }
	void SetStage(int stage) { m_stage = stage; }
	void SetGameScene(GameScene* scene) { m_gameScene = scene; }
private:

	GameScene* m_gameScene = nullptr;

	void LoadStage();

	Math::Vector3 m_pos = Math::Vector3::Zero;

	int m_stage;
};