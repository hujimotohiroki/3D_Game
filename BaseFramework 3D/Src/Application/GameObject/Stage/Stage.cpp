#include "Stage.h"
#include "../../Scene/SceneManager.h"
#include "Ground/TopGround.h"
#include "Ground/MidGround.h"
#include "Block/SideBlock.h"
#include "Back/FirstBack.h"
#include "../../Scene/GameScene/GameScene.h"

void Stage::Init()
{
	LoadStage();
}

void Stage::LoadStage()
{
	FILE* fp=NULL;
	std::shared_ptr<TopGround> topGround = std::make_shared<TopGround>();
	std::shared_ptr<TopGround> topGround2 = std::make_shared<TopGround>();
	std::shared_ptr<SideBlock> sideBlock = std::make_shared<SideBlock>();
	switch (m_stage) {
	case 1:	
		topGround->SetGameScene(m_gameScene);
		topGround->SetStage(1);
		topGround->SetStep(1);
		topGround->Init();
		topGround->SetPos(Math::Vector3(-20.0f, 0.0f, 0.0f));
		SceneManager::Instance().AddObject(topGround);
		fp = fopen("StageData/Stage1.txt", "r");

		sideBlock->SetGameScene(m_gameScene);
		sideBlock->Init();
		sideBlock->SetPos(Math::Vector3(-20.0f, 16.0f, 16.0f));
		SceneManager::Instance().AddObject(sideBlock);
		
		topGround2->SetGameScene(m_gameScene);
		topGround2->SetStage(1);
		topGround2->SetStep(2);
		topGround2->Init();
		topGround2->SetPos(Math::Vector3(-20.0f, 100.0f, 0.0f));
		SceneManager::Instance().AddObject(topGround2);
		break;
	case 2:
		topGround->SetGameScene(m_gameScene);
		topGround->SetStage(2);
		topGround->Init();
		topGround->SetPos(Math::Vector3(-20.0f, -8.0f, 0.0f));
		SceneManager::Instance().AddObject(topGround);
		fp = fopen("StageData/Stage2.txt", "r");
		break;
	case 3:
		topGround->SetGameScene(m_gameScene);
		topGround->SetStage(3);
		topGround->Init();
		topGround->SetPos(Math::Vector3(-20.0f, -8.0f, 0.0f));
		SceneManager::Instance().AddObject(topGround);
		fp = fopen("StageData/Stage3.txt", "r");
		break;
	case 4:
		topGround->SetGameScene(m_gameScene);
		topGround->SetStage(3);
		topGround->Init();
		topGround->SetPos(Math::Vector3(-20.0f, -8.0f, 0.0f));
		SceneManager::Instance().AddObject(topGround);
		fp = fopen("StageData/Stage3.txt", "r");
		break;
	case 5:
		fp = fopen("StageData/Stage5.txt", "r");
		break;
	case 6:
		fp = fopen("StageData/Stage6.txt", "r");
		break;
	}

	/*std::shared_ptr<FirstBack> firstBack = std::make_shared<FirstBack>();
	firstBack->SetGameScene(m_gameScene);
	firstBack->SetPos(Math::Vector3(0, 0, 27.0f));
	firstBack->Init();*/
	//SceneManager::Instance().AddObject(firstBack);

	if (fp != nullptr) {
		int x, y, Data;
		fscanf_s(fp, "%d,%d\n", &x, &y);
		int sidex, sidey;
		int topx, topy;
		fscanf_s(fp, "%d,%d\n", &sidex, &sidey);
		fscanf_s(fp, "%d,%d\n", &topx, &topy);
	
		

		
		for (int i = 0; i < y; i++) {
			for (int j = 0; j < x; j++) {
				fscanf_s(fp, "%d,", &Data);
				switch (Data) {
					/*case 0:
						StageData[i][j] = 0;
						break;
					case 1:
						StageData[i][j] = 1;
						if (i != 0 && StageData[i - 1][j] == 0)
						{
							{
								std::shared_ptr<TopGround> topGround = std::make_shared<TopGround>();
								topGround->SetGameScene(m_gameScene);
								topGround->SetPos(Math::Vector3((float)j * 2.0f - 20, (float)i * (-2.0f) + 10, 0.0f));
								topGround->Init();
								SceneManager::Instance().AddObject(topGround);
							}
							 {
								std::shared_ptr<TopGround> topGround = std::make_shared<TopGround>();
								topGround->SetGameScene(m_gameScene);
								topGround->SetPos(Math::Vector3((float)j * (-2.0f) + 20, (float)i * (-2.0f) + 10, -40.0f));
								topGround->Init();
								SceneManager::Instance().AddObject(topGround);
							}
						}
						else
						{
							{
								std::shared_ptr<MidGround> midGround = std::make_shared<MidGround>();
								midGround->SetGameScene(m_gameScene);
								midGround->SetPos(Math::Vector3((float)j * 2.0f - 20, (float)i * (-2.0f) + 10, 0.0f));
								midGround->Init();
								SceneManager::Instance().AddObject(midGround);
							}
							 {
								std::shared_ptr<MidGround> midGround = std::make_shared<MidGround>();
								midGround->SetGameScene(m_gameScene);
								midGround->SetPos(Math::Vector3((float)j * (-2.0f) + 20, (float)i * (-2.0f) + 10, -40.0f));
								midGround->Init();
								SceneManager::Instance().AddObject(midGround);
							}
						}
						
						break;*/
					case 2:
						StageData[i][j] = 2;
						std::shared_ptr<SideBlock> sideBlock = std::make_shared<SideBlock>();
						sideBlock->SetGameScene(m_gameScene);
						sideBlock->SetPos(Math::Vector3((float)j * 2.0f-20, (float)i * (-2.0f)+10, 0.0f));
						//sideBlock->SetTarget(top);
						sideBlock->Init();
						SceneManager::Instance().AddObject(sideBlock);
						break;
				}
			}
		}
		fclose(fp);
	}
}
