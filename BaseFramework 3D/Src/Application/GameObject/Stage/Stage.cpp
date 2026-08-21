#include "Stage.h"
#include "../../Scene/SceneManager.h"
#include "Ground/TopGround.h"
#include "Ground/MidGround.h"
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
							topGround->SetPos(Math::Vector3((float)j*2.0f, (float)i*(-2.0f), 0.0f));
							topGround->Init();
							SceneManager::Instance().AddObject(topGround);
						}
						else
						{
							std::shared_ptr<MidGround> midGround = std::make_shared<MidGround>();
							midGround->SetGameScene(m_gameScene);
							midGround->SetPos(Math::Vector3((float)j*2.0f, (float)i*(-2.0f), 0.0f));
							midGround->Init();
							SceneManager::Instance().AddObject(midGround);
						}
						
						break;
				}
			}
		}
		fclose(fp);
	}
}
