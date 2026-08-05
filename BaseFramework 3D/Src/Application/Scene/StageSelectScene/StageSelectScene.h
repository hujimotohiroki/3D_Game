#pragma once

#include"../BaseScene/BaseScene.h"

class StageSelect;

class StageSelectScene : public BaseScene
{
public:

	StageSelectScene() { Init(); }
	~StageSelectScene() {}

private:

	void Event() override;
	void Init()  override;
	
	int nowselect = 1;

	std::shared_ptr<StageSelect> stageselect;
};
