#pragma once

class GameScene;

class Stage :public KdGameObject
{
public:
	
	void Init()override;
	void Update()override;

	Math::Vector3 GetPos() { return m_pos; }
	void SetStage(int stage) { m_stage = stage; }
	void SetGameScene(GameScene* scene) { m_gameScene = scene; }

	void AddGoal() { Goal++; }
	void SubtractGoal() { Goal--; }
private:

	GameScene* m_gameScene = nullptr;

	void LoadStage();

	Math::Vector3 m_pos = Math::Vector3::Zero;
	
	int StageData[10][20];//[y][x]

	int m_stage;

	int Goal = 0;
	int GoalCount = 1;

	int nextstagecount = 0;
};