#include "StageSelectScene.h"
#include "../SceneManager.h"
#include "../../GameObject/StageSelect/StageSelect.h"
#include "../../GameObject/Stage/Stage.h"
#include "../../Scene/GameScene/GameScene.h"
#include "../../GameObject/TV/TV.h"
#include "../../GameObject/Camera/TPSCamera/TPSCamera.h"
#include "../../GameObject/Stage/Back/FirstBack.h"
#include "../../GameObject/TV/Controller.h"

void StageSelectScene::Event()
{
	if (GetAsyncKeyState('Z') & 0x8000)
	{
		
		SceneManager::Instance().SetNextScene
		(
			SceneManager::SceneType::Game
		);
		
	}
	if(choosetime==10){
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			m_nowStage--;
			if (m_nowStage < 1) m_nowStage = 1;
			else choosetime = 0;
		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			m_nowStage++;
			if (m_nowStage > 3) m_nowStage = 3;
			else choosetime = 0;
		}
		
	}
	if(choosetime==0)
	{
		m_objList.clear();
		std::shared_ptr<TPSCamera> Tcamera = std::make_shared<TPSCamera>();
		Tcamera->Init();
		m_objList.push_back(Tcamera);

		stageselect = std::make_shared<StageSelect>();
		stageselect->Init();
		m_objList.push_back(stageselect);

		std::shared_ptr<Stage> stage = std::make_shared<Stage>();
		stage->SetStage(m_nowStage);
		stage->Init();
		m_objList.push_back(stage);

		std::shared_ptr<FirstBack> firstBack = std::make_shared<FirstBack>();
		firstBack->SetPos(Math::Vector3(0, 0, 54.0f));
		firstBack->Init();
		m_objList.push_back(firstBack);
	}
	if (GetAsyncKeyState('E') & 0x8000)
	{
		SceneManager::Instance().SetNextScene
		(
			SceneManager::SceneType::Title
		);
	}
	choosetime++;
	if (choosetime > 10) choosetime = 10;
}

void StageSelectScene::Init()
{
	choosetime = 0;
	m_nowStage = 1;
	m_objList.clear();
	std::shared_ptr<TPSCamera> Tcamera = std::make_shared<TPSCamera>();
	Tcamera->Init();
	m_objList.push_back(Tcamera);

	stageselect = std::make_shared<StageSelect>();
	stageselect->Init();
	m_objList.push_back(stageselect);

	std::shared_ptr<Stage> stage = std::make_shared<Stage>();
	stage->SetStage(m_nowStage);
	stage->Init();
	m_objList.push_back(stage);

	//std::shared_ptr<FirstBack> firstBack = std::make_shared<FirstBack>();
	//firstBack->SetPos(Math::Vector3(0, 0, 54.0f));
	//firstBack->Init();
	//m_objList.push_back(firstBack);

	std::shared_ptr<TV> tv = std::make_shared<TV>();
	tv->Init();
	tv->SetPos(Math::Vector3(0.0f, -6.0f, -14.0f));
	//m_objList.push_back(tv);
	
	std::shared_ptr<Controller> controller = std::make_shared<Controller>();
	controller->Init();
	//m_objList.push_back(controller);
	
	
}
