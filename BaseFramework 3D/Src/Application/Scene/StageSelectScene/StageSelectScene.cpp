#include "StageSelectScene.h"
#include "../SceneManager.h"
#include "../../GameObject/StageSelect/StageSelect.h"
#include "../../GameObject/Stage/Stage.h"
#include "../../Scene/GameScene/GameScene.h"
#include "../../GameObject/TV/TV.h"
#include "../../GameObject/Camera/TPSCamera/TPSCamera.h"
#include "../../GameObject/Stage/Back/FirstBack.h"

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
	std::shared_ptr<TPSCamera> Tcamera = std::make_shared<TPSCamera>();
	Tcamera->Init();
	m_objList.push_back(Tcamera);

	stageselect = std::make_shared<StageSelect>();
	stageselect->Init();
	m_objList.push_back(stageselect);

	std::shared_ptr<FirstBack> firstBack = std::make_shared<FirstBack>();
	firstBack->SetPos(Math::Vector3(0, 0, 27.0f));
	firstBack->Init();
	m_objList.push_back(firstBack);

	std::shared_ptr<TV> tv = std::make_shared<TV>();
	tv->Init();
	tv->SetPos(Math::Vector3(0.0f, -6.0f, -14.0f));
	m_objList.push_back(tv);
}
