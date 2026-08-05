#include "StageSelectScene.h"
#include "../SceneManager.h"
#include "../../GameObject/StageSelect/StageSelect.h"
#include "../../GameObject/Stage/Stage.h"

void StageSelectScene::Event()
{
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		SceneManager::Instance().SetNextScene
		(
			SceneManager::SceneType::Game
		);
		std::shared_ptr<Stage> stage = std::make_shared<Stage>();
		stage->SetStage(stageselect->GetStage());
		stage->Init();
		SceneManager::Instance().AddObject(stage);
	}
	
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
	{
		SceneManager::Instance().SetNextScene
		(
			SceneManager::SceneType::Title
		);
	}
}

void StageSelectScene::Init()
{
	stageselect = std::make_shared<StageSelect>();
	stageselect->Init();
	m_objList.push_back(stageselect);
}
