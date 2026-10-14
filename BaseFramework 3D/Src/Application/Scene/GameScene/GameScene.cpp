#include "GameScene.h"
#include "../SceneManager.h"
#include "../../GameObject/Camera/TPSCamera/TPSCamera.h"
#include "../../GameObject/Camera/FPSCamera/FPSCamera.h"
#include "../../GameObject/Stage/Stage.h"
#include "../../GameObject/TV/TV.h"
#include "../../GameObject/TV/REC.h"
#include "../../GameObject/TV/Controller.h"
#include "../../GameObject/Player/SideChara/SideChara.h"
#include "../../GameObject/Player/TopChara/TopChara.h"


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
			isFPSCamera = !isFPSCamera;
			Tcamera->Setm_spCamera(!isFPSCamera);
			Fcamera->Setm_spCamera(isFPSCamera);
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
		Tcamera->Setm_spCamera(true);
		m_objList.push_back(Tcamera);

		std::shared_ptr<Stage> stage = std::make_shared<Stage>();
		stage->SetStage(m_nowStage);
		stage->SetGameScene(this);
		stage->Init();
		m_objList.push_back(stage);

		side = std::make_shared<SideChara>();
		side->Init();
		side->SetGameScene(this);
		side->SetPos(Math::Vector3(-20.0f, 10.0f, 0.0f));
		m_objList.push_back(side);

		Tcamera->SetTarget(side);
		side->SetTarget(Tcamera);

		top = std::make_shared<TopChara>();
		top->Init();
		top->SetGameScene(this);
		top->SetPos(Math::Vector3(-20.0f, 8.0f, 0.0f));
		m_objList.push_back(top);

		std::shared_ptr<TV> tv = std::make_shared<TV>();
		tv->Init();
		tv->SetPos(Math::Vector3(0.0f, -6.0f, -14.0f));
		m_objList.push_back(tv);

		std::shared_ptr<REC> rec = std::make_shared<REC>();
		rec->Init();
		m_objList.push_back(rec);

		std::shared_ptr<Controller> controller = std::make_shared<Controller>();
		controller->Init();
		m_objList.push_back(controller);

		shiftFlg = false;
		NextStageFlg = false;
	}
	if (ResetFlg)
	{
		m_objList.clear();
	
		Tcamera = std::make_shared<TPSCamera>();
		Tcamera->Init();
		Tcamera->Setm_spCamera(true);
		m_objList.push_back(Tcamera);

		std::shared_ptr<Stage> stage = std::make_shared<Stage>();
		stage->SetStage(m_nowStage);
		stage->SetGameScene(this);
		stage->Init();
		m_objList.push_back(stage);
	
		side = std::make_shared<SideChara>();
		side->Init();
		side->SetGameScene(this);
		side->SetPos(Math::Vector3(-20.0f, 10.0f, 0.0f));
		m_objList.push_back(side);

		Tcamera->SetTarget(side);
		side->SetTarget(Tcamera);

		top = std::make_shared<TopChara>();
		top->Init();
		top->SetGameScene(this);
		top->SetPos(Math::Vector3(-20.0f, 8.0f, 0.0f));
		m_objList.push_back(top);


		std::shared_ptr<TV> tv = std::make_shared<TV>();
		tv->Init();
		tv->SetPos(Math::Vector3(0.0f, -6.0f, -14.0f));
		m_objList.push_back(tv);

		std::shared_ptr<REC> rec = std::make_shared<REC>();
		rec->Init();
		m_objList.push_back(rec);

		std::shared_ptr<Controller> controller = std::make_shared<Controller>();
		controller->Init();
		m_objList.push_back(controller);

		shiftFlg = false;
		ResetFlg = false;
	}
}

void GameScene::Init()
{
	Tcamera = std::make_shared<TPSCamera>();
	Tcamera->Init();
	Tcamera->Setm_spCamera(true);
	m_objList.push_back(Tcamera);

	side = std::make_shared<SideChara>();
	side->Init();
	side->SetGameScene(this);
	side->SetPos(Math::Vector3(- 20.0f, 10.0f, 0.0f));
	m_objList.push_back(side);

	Tcamera->SetTarget(side);
	side->SetTarget(Tcamera);
	
	top = std::make_shared<TopChara>();
	top->Init();
	top->SetGameScene(this);
	top->SetPos(Math::Vector3(- 20.0f, 8.0f, 0.0f));
	m_objList.push_back(top);

	{
		std::shared_ptr<TV> tv = std::make_shared<TV>();
		tv->Init();
		tv->SetPos(Math::Vector3(0.0f, -6.0f, -14.0f));
		tv->SetRotateY(0);
		m_objList.push_back(tv);
	}

	/* {
		std::shared_ptr<TV> tv = std::make_shared<TV>();
		tv->Init();
		tv->SetPos(Math::Vector3(0.0f, -6.0f, -26.0f));
		tv->SetRotateY(180);
		m_objList.push_back(tv);
	}*/

	std::shared_ptr<REC> rec = std::make_shared<REC>();
	rec->Init();
	m_objList.push_back(rec);

	std::shared_ptr<Controller> controller = std::make_shared<Controller>();
	controller->Init();
	m_objList.push_back(controller);

	shiftFlg = false;
}
