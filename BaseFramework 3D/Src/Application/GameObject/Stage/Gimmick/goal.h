#pragma once
#include "../StageObjectBase.h"

class Stage;
class goal :public StageObjectBase
{
public:
	void Init() override;
	void Update()override;
	//void GenerateDepthMapFromLight()override;
	void DrawLit()override;
	void SetStage(Stage* stage) { m_stage = stage; }
private:
	bool hitflg = false;
	Stage* m_stage = nullptr;
};