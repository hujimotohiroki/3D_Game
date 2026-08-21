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
	
	

	std::shared_ptr<StageSelect> stageselect;
};
