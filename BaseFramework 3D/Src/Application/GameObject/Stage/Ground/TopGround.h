#pragma once
#include "../StageObjectBase.h"

class TopGround :public StageObjectBase
{
public:
	void Init() override;
	void Update()override;
	//void GenerateDepthMapFromLight()override;
	void DrawLit()override;
	void SetStage(int stage) { m_stage = stage; }
	void SetStep(int step) { m_step = step; }
private:
	int m_stage;
	int m_step;
};