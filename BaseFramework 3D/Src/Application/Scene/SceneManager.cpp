#include "SceneManager.h"

#include "BaseScene/BaseScene.h"
#include "TitleScene/TitleScene.h"
#include "StageSelectScene/StageSelectScene.h"
#include "GameScene/GameScene.h"
#include "../GameObject/Stage/Stage.h"

void SceneManager::PreUpdate()
{
	// シーン切替
	if (m_currentSceneType != m_nextSceneType)
	{
		ChangeScene(m_nextSceneType);
	}

	m_currentScene->PreUpdate();
}

void SceneManager::Update()
{
	m_currentScene->Update();
}

void SceneManager::PostUpdate()
{
	m_currentScene->PostUpdate();
}

void SceneManager::PreDraw()
{
	m_currentScene->PreDraw();
}

void SceneManager::Draw()
{
	m_currentScene->Draw();
}

void SceneManager::DrawSprite()
{
	m_currentScene->DrawSprite();
}

void SceneManager::DrawDebug()
{
	m_currentScene->DrawDebug();
}

const std::list<std::shared_ptr<KdGameObject>>& SceneManager::GetObjList()
{
	return m_currentScene->GetObjList();
}

void SceneManager::AddObject(const std::shared_ptr<KdGameObject>& _obj)
{
	m_currentScene->AddObject(_obj);
}

void SceneManager::ChangeScene(SceneType _sceneType)
{
	// 次のシーンを作成し、現在のシーンにする
	switch (_sceneType)
	{
	case SceneType::Title:
		m_currentScene = std::make_shared<TitleScene>();
		break;
	case SceneType::StageSelect:
		m_currentScene = std::make_shared<StageSelectScene>();
		break;
	case SceneType::Game:
		//ここに現在選択しているステージを渡す処理を追加する
		int nowstage=m_currentScene->GetNowSelect();
		m_currentScene = std::make_shared<GameScene>();
		m_currentScene->SetNowStage(nowstage);
		std::shared_ptr<Stage> stage = std::make_shared<Stage>();
		stage->SetStage(nowstage);
		stage->SetGameScene(dynamic_cast<GameScene*>(m_currentScene.get()));
		stage->Init();
		SceneManager::Instance().AddObject(stage);
		break;
	}

	// 現在のシーン情報を更新
	m_currentSceneType = _sceneType;
}
