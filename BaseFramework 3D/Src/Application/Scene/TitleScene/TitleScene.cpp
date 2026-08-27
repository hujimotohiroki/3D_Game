#include "TitleScene.h"
#include "../SceneManager.h"
#include "../../GameObject/TV/TV.h"
#include "../../GameObject/TV/REC.h"
#include "../../GameObject/Title/Title.h"
#include "../../GameObject/Title/Press Enter.h"
#include "../../GameObject/Camera/TPSCamera/TPSCamera.h"
#include "../../GameObject/Stage/Back/FirstBack.h"

void TitleScene::Event()
{
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		SceneManager::Instance().SetNextScene
		(
			SceneManager::SceneType::StageSelect
		);
	}
}

void TitleScene::Init()
{
	std::shared_ptr<TPSCamera> Tcamera = std::make_shared<TPSCamera>();
	Tcamera->Init();
	m_objList.push_back(Tcamera);

	std::shared_ptr<FirstBack> firstBack = std::make_shared<FirstBack>();
	firstBack->SetPos(Math::Vector3(0, 0, 27.0f));
	firstBack->Init();
	m_objList.push_back(firstBack);

	std::shared_ptr<TV> tv = std::make_shared<TV>();
	tv->Init();
	tv->SetPos(Math::Vector3(0.0f, -6.0f, -14.0f));
	m_objList.push_back(tv);

	std::shared_ptr<Title> title = std::make_shared<Title>();
	title->Init();
	m_objList.push_back(title);

	std::shared_ptr<PressEnter> pressEnter = std::make_shared<PressEnter>();
	pressEnter->Init();
	m_objList.push_back(pressEnter);

	std::shared_ptr<REC> rec = std::make_shared<REC>();
	rec->Init();
	m_objList.push_back(rec);
}
