#include "StageSelectScene.h"
#include "../SceneManager.h"
#include "../../GameObject/StageSelect/StageSelect.h"
#include "../../GameObject/Stage/Stage.h"
#include "../../Scene/GameScene/GameScene.h"

void StageSelectScene::Event()
{
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		SceneManager::Instance().SetNextScene
		(
			SceneManager::SceneType::Game
		);
		
	}
	
	if (GetAsyncKeyState('E') & 0x8000)
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
