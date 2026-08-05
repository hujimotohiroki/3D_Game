#include "Stage.h"
#include "Ground/TopGround.h"
#include "../../Scene/GameScene/GameScene.h"
void Stage::Init()
{
	LoadStage();
}

void Stage::LoadStage()
{
	FILE* fp;
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
						break;
					case 1:
						std::shared_ptr<TopGround> topGround = std::make_shared<TopGround>();
						topGround->SetGameScene(m_gameScene);
						topGround->SetPos(Math::Vector3((float)j, 0.0f, (float)i));
						topGround->Init();
						m_gameScene->AddObject(topGround);
						break;
				}
			}
		}
		fclose(fp);
	}
}
