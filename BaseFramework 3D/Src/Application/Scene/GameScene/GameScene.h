#pragma once

#include"../BaseScene/BaseScene.h"

class TPSCamera;
class FPSCamera;
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
	void SetNextStage() { NextStageFlg = true; }

	void SetResetFlg() { ResetFlg = true; }
private:

	void Event() override;
	void Init()  override;

	KdModelData m_model;

	std::shared_ptr<FPSCamera>		Fcamera = nullptr;
	std::shared_ptr<TPSCamera>		Tcamera = nullptr;
	
	bool isFPSCamera = true;

	bool NextStageFlg = false;

	NowChara chara = NowChara::Side;

	bool shiftFlg = false;

	bool ResetFlg = false;

	std::shared_ptr<TopChara>		top = nullptr;
	std::shared_ptr<SideChara>		side = nullptr;
};
