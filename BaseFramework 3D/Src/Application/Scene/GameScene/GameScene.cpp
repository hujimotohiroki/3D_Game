#include "GameScene.h"
#include "../SceneManager.h"
#include "../../GameObject/Camera/TPSCamera/TPSCamera.h"
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

	shiftFlg = false;
}
