#pragma once

class StageSelect :public KdGameObject
{
public:
	void Init()override;
	void Update()override;
	void DrawSprite()override;

	int GetStage() { return nowselect; }
private:
	int nowselect;
};