#pragma once

#include"../BaseScene/BaseScene.h"

class TPSCamera;
class TopChara;
class SideChara;

class GameScene : public BaseScene
{
public :
	
	enum class NowChara
	{
		Top,
		Side,
	};

	GameScene()  { Init(); }
	~GameScene() {}

	NowChara GetNowChara() { return chara; }
	void SetNextStage();
private:

	void Event() override;
	void Init()  override;

	KdModelData m_model;

	std::shared_ptr<TPSCamera>		Tcamera = nullptr;
	std::shared_ptr<TopChara>		top = nullptr;
	std::shared_ptr<SideChara>		side = nullptr;

	NowChara chara = NowChara::Top;

	bool shiftFlg = false;
};
