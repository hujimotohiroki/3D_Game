#include "GameScene.h"
#include "../SceneManager.h"
#include "../../GameObject/Camera/TPSCamera/TPSCamera.h"
#include "../../GameObject/Player/TopChara/TopChara.h"
#include "../../GameObject/Player/SideChara/SideChara.h"
#include "../../GameObject/Stage/Ground/TopGround.h"
#include "../../GameObject/Stage/Stage.h"

void GameScene::SetNextStage()
{
	m_objList.clear();
	Tcamera = std::make_shared<TPSCamera>();
	Tcamera->Init();
	m_objList.push_back(Tcamera);

	std::shared_ptr<Stage> stage = std::make_shared<Stage>();
	stage->SetStage(m_nowStage);
	stage->Init();
	m_objList.push_back(stage);

	top = std::make_shared<TopChara>();
	top->Init();
	top->SetGameScene(this);
	m_objList.push_back(top);

	side = std::make_shared<SideChara>();
	side->Init();
	side->SetGameScene(this);
	m_objList.push_back(side);

	std::shared_ptr<TopGround> topGround = std::make_shared<TopGround>();
	topGround->SetGameScene(this);
	topGround->SetPos(Math::Vector3(0, 0, -5.0f));
	topGround->Init();
	m_objList.push_back(topGround);

	shiftFlg = false;
}

void GameScene::Event()
{
	if (GetAsyncKeyState('Q') & 0x8000)
	{
		SceneManager::Instance().SetNextScene
		(
			SceneManager::SceneType::StageSelect
		);
	}
	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		if(!shiftFlg)
		{
			chara = (NowChara)(((int)chara + 1) % 2);
			shiftFlg = true;
		}
	}
	else
	{
		shiftFlg = false;
	}
}

void GameScene::Init()
{
	Tcamera = std::make_shared<TPSCamera>();
	Tcamera->Init();
	m_objList.push_back(Tcamera);

	top= std::make_shared<TopChara>();
	top->Init();
	top->SetGameScene(this);
	m_objList.push_back(top);

	side = std::make_shared<SideChara>();
	side->Init();
	side->SetGameScene(this);
	m_objList.push_back(side);
	
	std::shared_ptr<TopGround> topGround = std::make_shared<TopGround>();
	topGround->SetGameScene(this);
	topGround->SetPos(Math::Vector3(0, 0, -5.0f));
	topGround->Init();
	m_objList.push_back(topGround);

	shiftFlg = false;
}
