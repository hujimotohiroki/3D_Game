#pragma once

using namespace std;

class GameScene;

class Stage :public KdGameObject
{
public:
	
	void Init()override;
	void Update()override;

	Math::Vector3 GetPos() { return m_pos; }
	void SetStage(int stage) { m_stage = stage; }
	void SetGameScene(GameScene* scene) { m_gameScene = scene; }

	void SetStageData(int x, int y, int z, char data) {
		StageData[x][y][z] = data;
	}

	void AddGoal() { Goal++; }
	void SubtractGoal() { Goal--; }
private:

	GameScene* m_gameScene = nullptr;

	void LoadStage();

	Math::Vector3 m_pos = Math::Vector3::Zero;
	
	array<array<array<char,10>,10>,10> StageData;//[x][y][z]

	char m_stage;

	int Goal = 0;
	int GoalCount = 1;

	int nextstagecount = 0;
};