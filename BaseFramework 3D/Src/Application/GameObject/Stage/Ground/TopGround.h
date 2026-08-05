#pragma once
#include "../StageObjectBase.h"

class TopGround :public StageObjectBase
{
public:
	void Init() override;
	void Update()override;
	void GenerateDepthMapFromLight()override;
	void DrawLit()override;
private:

};