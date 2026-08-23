#include "GameScene.h"
#include "../SceneManager.h"
#include "../../GameObject/Camera/TPSCamera/TPSCamera.h"
#include "../../GameObject/Stage/Stage.h"
#include "../../GameObject/TV/TV.h"
#include "../../GameObject/TV/REC.h"

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
	if (NextStageFlg)
	{
		m_nowStage++;
		m_objList.clear();
		Tcamera = std::make_shared<TPSCamera>();
		Tcamera->Init();
		m_objList.push_back(Tcamera);

		std::shared_ptr<Stage> stage = std::make_shared<Stage>();
		stage->SetStage(m_nowStage);
		stage->SetGameScene(this);
		stage->Init();
		m_objList.push_back(stage);

		std::shared_ptr<TV> tv = std::make_shared<TV>();
		tv->Init();
		m_objList.push_back(tv);

		std::shared_ptr<REC> rec = std::make_shared<REC>();
		rec->Init();
		m_objList.push_back(rec);

		shiftFlg = false;
		NextStageFlg = false;
	}
}

void GameScene::Init()
{
	Tcamera = std::make_shared<TPSCamera>();
	Tcamera->Init();
	m_objList.push_back(Tcamera);

	std::shared_ptr<TV> tv = std::make_shared<TV>();
	tv->Init();
	m_objList.push_back(tv);

	std::shared_ptr<REC> rec = std::make_shared<REC>();
	rec->Init();
	m_objList.push_back(rec);

	shiftFlg = false;
}
