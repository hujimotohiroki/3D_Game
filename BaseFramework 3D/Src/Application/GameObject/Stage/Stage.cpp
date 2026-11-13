#include "Stage.h"
#include "../../Scene/SceneManager.h"
#include "Ground/TopGround.h"
#include "Ground/MidGround.h"
#include "Block/SideBlock.h"
#include "Block/SideBlock2.h"
#include "Back/FirstBack.h"
#include "Gimmick/goal.h"
#include "Ground/Scaffold.h"
#include "../Title/Stage Clear.h"
#include "../../Scene/GameScene/GameScene.h"



void Stage::Init()
{
	nextstagecount = 0;
	Goal = 0;
	LoadStage();
}

void Stage::Update()
{
	KdDebugGUI::Instance().AddLog("Goal:%d GoalCount:%d\n", Goal, GoalCount);
	if(nextstagecount==0){
		if (Goal == GoalCount){
			nextstagecount = 1;
		}
	}
	else {
		nextstagecount++;
		if (nextstagecount > 180){
			nextstagecount = 0;
			m_gameScene->SetNextStage();
		}
		else if (nextstagecount > 30) {
			std::shared_ptr<StageClear> stageclear = std::make_shared<StageClear>();
			stageclear->Init();
			stageclear->SetPos(Math::Vector3(0.0f, 32.0f, 40.0f));
			SceneManager::Instance().AddObject(stageclear);
		}
	}
	
}

void Stage::LoadStage()
{
	FILE* fp=NULL;
	fp = fopen(("StageData/Stage" + std::to_string(m_stage) + ".txt").c_str(), "r");

	if (fp != nullptr) {
		std::shared_ptr<TopGround> topGround = std::make_shared<TopGround>();
		int StageSize;
		fscanf_s(fp, "%d\n", &StageSize);
		topGround->SetGameScene(m_gameScene);
		topGround->SetStage(m_stage);
		topGround->SetStep(1);
		topGround->SetStageSize((float)StageSize);
		topGround->Init();
		topGround->SetPos(Math::Vector3(0.0f, 0.0f, 0.0f));
		SceneManager::Instance().AddObject(topGround);
		
		float center = (float)(StageSize - 1) /2.0f;
		fscanf_s(fp, "%d\n", &GoalCount);
		std::shared_ptr<SideBlock> sideBlock;
		for (int i = 0;i < GoalCount;i++) {
			
			int x, y, z, size;
			fscanf_s(fp, "%d,%d,%d,%d\n", &x, &y, &z, &size);
			sideBlock = std::make_shared<SideBlock>();
			sideBlock->SetGameScene(m_gameScene);
			sideBlock->SetStage(this);
			sideBlock->SetBlockType(size);
			sideBlock->SetStageDataptr(&StageData);
			sideBlock->Init();
			sideBlock->SetPos(Math::Vector3(squaresize*(float)(x-center), squaresize * (float)(y + 1), -squaresize* (float)(z - center)));
			sideBlock->SetStagePos(x, y, z);
			sideBlock->SetStageSize(StageSize);
			SceneManager::Instance().AddObject(sideBlock);
		}
		std::shared_ptr<goal> goalObj;
		for (int i = 0;i < GoalCount;i++) {
			
			int x, y, z;
			fscanf_s(fp, "%d,%d,%d\n", &x, &y, &z);
			goalObj = std::make_shared<goal>();
			goalObj->SetGameScene(m_gameScene);
			goalObj->SetStage(this);
			goalObj->Init();
			goalObj->SetPos(Math::Vector3(squaresize*(float)(x-center), squaresize * (float)(y + 1), -squaresize* (float)(z - center)));
			SceneManager::Instance().AddObject(goalObj);
		}
		std::shared_ptr<Scaffold> scaffold = std::make_shared<Scaffold>();
		int ScaffoldSize;
		fscanf_s(fp, "%d\n", &ScaffoldSize);
		for (int i = 0;i < ScaffoldSize;i++) {
			int x, y, z;
			fscanf_s(fp, "%d,%d,%d\n", &x, &y, &z);
			scaffold = std::make_shared<Scaffold>();
			scaffold->SetGameScene(m_gameScene);
			scaffold->Init();
			scaffold->SetPos(Math::Vector3(squaresize * (float)(x - center), squaresize * (float)(y + 1)+8.0f, -squaresize * (float)(z - center)));
			SceneManager::Instance().AddObject(scaffold);
		}
		for (int y = 0;y < StageSize;y++) {
			fscanf_s(fp, "\n");
			for (int z = 0;z < StageSize;z++) {
				for (int x = 0;x < StageSize;x++) {
					fscanf_s(fp, "%c", &StageData[x][StageSize - 1 - y][z]);
				}
				fscanf_s(fp, "\n");
			}
		}
		fclose(fp);
	}
}
