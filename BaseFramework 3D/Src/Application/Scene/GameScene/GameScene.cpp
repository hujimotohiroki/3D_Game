#include "GameScene.h"
#include "../SceneManager.h"
#include "../../GameObject/Camera/TPSCamera/TPSCamera.h"
#include "../../GameObject/Player/TopChara/TopChara.h"
#include "../../GameObject/Player/SideChara/SideChara.h"

void GameScene::Event()
{
	if (GetAsyncKeyState('T') & 0x8000)
	{
		SceneManager::Instance().SetNextScene
		(
			SceneManager::SceneType::Title
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
	//Tcamera->SetTarget(top);

	shiftFlg = false;
}
