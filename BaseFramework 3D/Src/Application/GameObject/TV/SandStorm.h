#pragma once
class GameScene;
class SandStorm :public KdGameObject
{
public:
	void Init() override;
	void Update() override;
	void DrawLit() override;

	void SetGameScene(GameScene* scene) { m_gameScene = scene; }
private:
	std::shared_ptr<KdModelData> m_model;
	int timer = 0;
	GameScene* m_gameScene = nullptr;
};