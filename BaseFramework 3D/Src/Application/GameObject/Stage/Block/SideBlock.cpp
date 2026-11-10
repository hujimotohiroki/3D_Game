#include "SideBlock.h"
#include "TopBlock.h"
#include "../../../Scene/GameScene/GameScene.h"
#include "../../../Scene/SceneManager.h"
#include "../Stage.h"
#include <cmath>

void SideBlock::Init()
{
	m_model = std::make_shared<KdModelData>();
	switch (m_blockSize) {
		case 1:
			m_model->Load("Asset/Models/Block/SideBlock.gltf");
			break;
		case 2:
			m_model->Load("Asset/Models/Block/SideBlock2.gltf");
			break;
		case 3:
			m_model->Load("Asset/Models/Block/SideBlock3.gltf");
			break;
	}
	m_model->Load("Asset/Models/Block/SideBlock.gltf");
	blockBox.Center = m_pos;
	blockBox.Extents = Math::Vector3(1.99f, 1.99f*(float)m_blockSize, 1.99f);
	blockBox.Orientation = Math::Quaternion::Identity;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape(
		"BlockCollision",
		blockBox,
		KdCollider::TypeBump
	);
	//m_pCollider->RegisterCollisionShape("BlockCollision", m_model, KdCollider::TypeBump);
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
	m_collisionHalfSize.y *= (float)m_blockSize;
	RotMat = Math::Matrix::Identity; //Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(-90));
	
}

void SideBlock::Update()
{
	if(rollFlg!=0)
	{
		switch (rollFlg)
		{
		case 1:
			m_pos.x -= 0.8f;
			RotMat *= Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(4.5f));
			break;
		case 2:
			m_pos.z -= 0.8f;
			RotMat *= Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(-4.5f));
			break;
		case 3:
			m_pos.x += 0.8f;
			RotMat *= Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(-4.5f));
			break;
		case 4:
			m_pos.z += 0.8f;
			RotMat *= Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(4.5f));
			break;
		}
		rollCount+=rollFlg/abs(rollFlg);
		if (rollCount == 20|| rollCount == 0) {
			rollFlg = 0;
			rollCount = 0;
		}
	}
	//KdDebugGUI::Instance().AddLog("%d\n", rollFlg);
	
	m_mWorld = Math::Matrix::CreateScale(4)*RotMat* Math::Matrix::CreateTranslation(m_pos);

	
}

void SideBlock::PostUpdate()
{
	
}

/*void SideBlock::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}*/

void SideBlock::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}

void SideBlock::PushBlock(int Flg)
{
	bool ishit = false;
	Math::Vector3 moveDir = Math::Vector3::Zero;
	switch (Flg)
	{
	case 0:
		if (StageX == 0) ishit = true; // プレイエリア範囲外に出る場合は判定しない
		if (ishit) break;

		for (int x = 0;x < StageX+1;x++) 
		{
			for (int y = StageY;y < StageSize;y++) 
			{
				
				char a = m_pStageData->at(x).at(y).at(StageZ);
				if (a == '0' || a == 'G') continue; // 虚無とゴールは当たり判定なし
				if (x == StageX && y == StageY) continue; // 自分自身は当たり判定なし
				if (y == StageY) { 
					if (StageX - x <= m_blockSize) ishit = true;
				}
				else
				{
					int xdiff = max(0, StageX - x - 1);
					int ydiff = StageY - y;
					if (sqrt(pow(xdiff, 2) + pow(ydiff, 2)) < sqrt(m_blockSize * 2))
					{
						KdDebugGUI::Instance().AddLog("[%d][%d][%d] %c Hit\n", x, y, StageZ, a);
						ishit = true;
					}
				}
			}
		}

		if (!ishit)
		{
			KdDebugGUI::Instance().AddLog("0OK\n");
			m_pStage->SetStageData(StageX, StageY, StageZ, '0');
			moveDir = Math::Vector3(-1, 0, 0);
			StageX--;
		}
		break;
	case 1:
		if (StageZ == StageSize - 1) ishit = true; 
		if (ishit) break;

		for (int z = StageZ;z < StageSize;z++) 
		{
			for (int y = StageY;y < StageSize;y++) 
			{
				
				char a = m_pStageData->at(StageX).at(y).at(z);
				if (a == '0' || a == 'G') continue;
				if (z == StageZ && y == StageY) continue;
				if (y == StageY) {
					if (z - StageZ <= m_blockSize) ishit = true;
				}
				else
				{
					int zdiff = max(0, z - StageZ - 1);
					int ydiff = StageY - y;
					if (sqrt(pow(zdiff, 2) + pow(ydiff, 2)) < sqrt(m_blockSize * 2))
					{
						KdDebugGUI::Instance().AddLog("[%d][%d][%d] %c Hit\n", StageX, y, z, a);
						ishit = true;
					}
				}
			}
		}
		if (!ishit)
		{
			KdDebugGUI::Instance().AddLog("1OK\n");
			m_pStage->SetStageData(StageX, StageY, StageZ, '0');
			moveDir = Math::Vector3(0, 0, -1);
			StageZ++;
		}
		break;
	case 2:
		if (StageX == StageSize - 1) ishit = true; 
		if (ishit) break;

		for (int x = StageX;x < StageSize;x++) 
		{
			for (int y = StageY;y < StageSize;y++) 
			{
				
				char a = m_pStageData->at(x).at(y).at(StageZ);
				if (a == '0' || a == 'G') continue;
				if (x == StageX && y == StageY) continue;
				if (y == StageY) {
					if (x - StageX <= m_blockSize) ishit = true;
				}
				else
				{
					int xdiff = max(0, x - StageX - 1);
					int ydiff = StageY - y;
					if (sqrt(pow(xdiff, 2) + pow(ydiff, 2)) < sqrt(m_blockSize * 2))
					{
						KdDebugGUI::Instance().AddLog("[%d][%d][%d] %c Hit\n",x,y,StageZ,a);
						ishit = true;
					}
				}
			}
		}

		if (!ishit)
		{
			KdDebugGUI::Instance().AddLog("2OK\n");
			m_pStage->SetStageData(StageX, StageY, StageZ, '0');
			moveDir = Math::Vector3(1, 0, 0);
			StageX++;
		}
		break;
	case 3:
		if (StageZ == 0) ishit = true; 
		if (ishit) break;

		for (int z = 0;z < StageZ + 1;z++) 
		{
			for (int y = StageY;y < StageSize;y++) 
			{
				char a = m_pStageData->at(StageX).at(y).at(z);
				if (a == '0' || a == 'G') continue;
				if (z == StageZ && y == StageY) continue;
				if (y == StageY) {
					if (StageZ - z <= m_blockSize)
					{
						KdDebugGUI::Instance().AddLog("[%d][%d][%d] %c Hit\n", StageX, y, z, a);
						ishit = true;
					}
				}
				else
				{
					int zdiff = max(0, StageZ - z - 1);
					int ydiff = StageY - y;
					if (sqrt(pow(zdiff, 2) + pow(ydiff, 2)) < sqrt(m_blockSize * 2)) ishit = true;
				}
			}
		}

		if (!ishit)
		{
			KdDebugGUI::Instance().AddLog("3OK\n");
			m_pStage->SetStageData(StageX, StageY, StageZ, '0');
			moveDir = Math::Vector3(0, 0, 1);
			StageZ--;
		}
		break;
	}
	if (!ishit) {//当たってないなら
		m_pStage->SetStageData(StageX,StageY,StageZ,'B');
		m_pos += moveDir * 16.0f;
	}
	
}

