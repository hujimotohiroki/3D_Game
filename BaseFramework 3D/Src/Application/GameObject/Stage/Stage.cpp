#include "Stage.h"
#include "../../Scene/SceneManager.h"
#include "Ground/TopGround.h"
#include "Ground/MidGround.h"
#include "Block/SideBlock.h"
#include "../Player/SideChara/SideChara.h"
#include "../Player/TopChara/TopChara.h"
#include "../../Scene/GameScene/GameScene.h"

void Stage::Init()
{
	LoadStage();
}

void Stage::LoadStage()
{
	FILE* fp=NULL;
	switch (m_stage) {
	case 1:
		fp = fopen("StageData/Stage1.txt", "r");
		break;
	case 2:
		fp = fopen("StageData/Stage2.txt", "r");
		break;
	case 3:
		fp = fopen("StageData/Stage3.txt", "r");
		break;
	case 4:
		fp = fopen("StageData/Stage4.txt", "r");
		break;
	case 5:
		fp = fopen("StageData/Stage5.txt", "r");
		break;
	case 6:
		fp = fopen("StageData/Stage6.txt", "r");
		break;
	}



	if (fp != nullptr) {
		int x, y, Data;
		fscanf_s(fp, "%d,%d\n", &x, &y);
		int sidex, sidey;
		int topx, topy;
		fscanf_s(fp, "%d,%d\n", &sidex, &sidey);
		fscanf_s(fp, "%d,%d\n", &topx, &topy);
	
		side = std::make_shared<SideChara>();
		side->Init();
		side->SetGameScene(m_gameScene);
		side->SetPos(Math::Vector3((float)sidex-20.0f, (float)sidey*(-2.0f)+10.0f, 0.0f));
		SceneManager::Instance().AddObject(side);
		
		top = std::make_shared<TopChara>();
		top->Init();
		top->SetGameScene(m_gameScene);
		top->SetPos(Math::Vector3((float)topx-10.0f, (float)topy*(-1.0f)+5.0f, 1.0f));
		SceneManager::Instance().AddObject(top);

		
		for (int i = 0; i < y; i++) {
			for (int j = 0; j < x; j++) {
				fscanf_s(fp, "%d,", &Data);
				switch(Data) {
					case 0:
						StageData[i][j] = 0;
						break;
					case 1:
						StageData[i][j] = 1;
						if (i != 0 && StageData[i - 1][j] == 0)
						{
							std::shared_ptr<TopGround> topGround = std::make_shared<TopGround>();
							topGround->SetGameScene(m_gameScene);
							topGround->SetPos(Math::Vector3((float)j*2.0f-20, (float)i*(-2.0f)+10, 0.0f));
							topGround->Init();
							SceneManager::Instance().AddObject(topGround);
						}
						else
						{
							std::shared_ptr<MidGround> midGround = std::make_shared<MidGround>();
							midGround->SetGameScene(m_gameScene);
							midGround->SetPos(Math::Vector3((float)j*2.0f-20, (float)i*(-2.0f)+10, 0.0f));
							midGround->Init();
							SceneManager::Instance().AddObject(midGround);
						}
						
						break;
					case 2:
						StageData[i][j] = 2;
						std::shared_ptr<SideBlock> sideBlock = std::make_shared<SideBlock>();
						sideBlock->SetGameScene(m_gameScene);
						sideBlock->SetPos(Math::Vector3((float)j * 2.0f-20, (float)i * (-2.0f)+10, 0.0f));
						sideBlock->SetTarget(top);
						sideBlock->Init();
						SceneManager::Instance().AddObject(sideBlock);
						break;
				}
			}
		}
		fclose(fp);
	}
}
