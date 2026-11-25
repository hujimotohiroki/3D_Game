#include "SideBlock.h"
#include "TopBlock.h"
#include "../../../Scene/GameScene/GameScene.h"
#include "../../../Scene/SceneManager.h"
#include "../Stage.h"
#include <cmath>

void SideBlock::Init()
{
	m_model = std::make_shared<KdModelData>();
	switch (m_blockType) {
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
	//m_model->Load("Asset/Models/Block/SideBlock.gltf");
	blockBox.Center = m_pos;
	blockBox.Extents = Math::Vector3(squaresize/8.0f, squaresize/8.0f*(float)m_blockType, squaresize/8.0f);
	blockBox.Orientation = Math::Quaternion::Identity;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape(
		"BlockCollision",
		blockBox,
		KdCollider::TypeBump
	);
	
	m_blockHalfSize.y = (float)m_blockType / 2.0f;

	//m_pCollider->RegisterCollisionShape("BlockCollision", m_model, KdCollider::TypeBump);
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
	m_collisionHalfSize = m_blockHalfSize * squaresize;
	RotMat = Math::Matrix::Identity; //Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(-90));
	
}

void SideBlock::Update()
{
	if(rollFlg!=-1)
	{
		switch (rollFlg)
		{
		case 0:
			
			localRotMat *= Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(4.5f));
			break;
		case 1:
			
			localRotMat *= Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(-4.5f));
			break;
		case 2:
			
			localRotMat *= Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(-4.5f));
			break;
		case 3:
			
			localRotMat *= Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(4.5f));
			break;
		}
		rollCount++;
		if (rollCount == 20) {

			if (rollFlg == 0 || rollFlg == 2) 
			{
				swap(m_blockHalfSize.x,m_blockHalfSize.y);
			}
			else if (rollFlg == 1 || rollFlg == 3)
			{
				swap(m_blockHalfSize.z, m_blockHalfSize.y);
			}
			m_collisionHalfSize = m_blockHalfSize * squaresize;

			for (int x = ceil(StageX - m_blockHalfSize.x);x < StageX + m_blockHalfSize.x;x++)
			{
				for (int y = ceil(StageY - m_blockHalfSize.y);y < StageY + m_blockHalfSize.y;y++)
				{
					for (int z = ceil(StageZ - m_blockHalfSize.z);z < StageZ + m_blockHalfSize.z;z++)
					{
						if (m_pStageData->at(x).at(y).at(z) == 'G')
						{
							m_pStage->AddGoal();
							m_pStage->SetStageData(x, y, z, 'b');
							KdDebugGUI::Instance().AddLog("GoalHit");
						}
						else
						{
							m_pStage->SetStageData(x, y, z, 'B');
						}
					}
				}
			}
			
			rollFlg = -1;
			rollCount = 0;
		}
		// 回し終わったらデータにブロックを再設定する
	}
	//KdDebugGUI::Instance().AddLog("%d\n", rollFlg);
	Math::Matrix localPosMat = Math::Matrix::CreateTranslation(m_localPos);
	
	m_mWorld = Math::Matrix::CreateScale(4) * RotMat * localPosMat * localRotMat * Math::Matrix::CreateTranslation(m_pos);

	
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
	if (rollFlg != -1)return;//もし動いている最中なら押されない
	/*
	1.軸になる場所を調べる
	2.軸との位置関係でブロックを二つに分ける
	3.衝突判定をする(この時、分けたブロックのサイズと位置、軸の位置を使い4象限の内のどれで判定するかを決める)
	4.動かす
	*/
	// 矩形当たり判定と円での当たり判定で分ける
	// 円での当たり判定をする範囲は軸とブロックの位置関係とブロックのサイズによって決まる
	// 軸を原点、x軸をx軸とz軸の内進む方の軸として座標平面を構築した時、
	// 第一象限の場合は軸との距離がブロックの対角線より近いマスの中から
	// ブロックの横の方が大きければ動かすブロックの上端より上のマスかつ動かした後のブロックの左端より右のマス
	// そうでなければ動かした後のブロックの上端より上のマスかつ動かすブロックの右端より左のマスが円判定の対象となる。
	// 第二象限の場合は軸との距離がブロックの対角線より近いマスの中から
	// ブロックの横の方が大きければ動かすブロックの下端より上のマスかつ動かした後のブロックの右端より右のマス
	// そうでなければ動かした後のブロックの上端より下のマスかつ動かすブロックの右端より右のマスが円判定の対象となる。
	// 第三象限は存在し得ない	
	// 第四象限の場合は軸との距離がブロックの対角線より近いマスの中から
	// ブロックの横の方が大きければ動かすブロックの上端より下のマスかつ動かした後のブロックの左端より左のマス
	// そうでなければ動かした後のブロックの下端より上のマスかつ動かすブロックの左端より左のマスが円判定の対象となる。
	// 矩形の当たり判定は動かした後のブロックの位置全てに判定を行う。
	bool ishit_up = false, ishit_down = false, ishit = false;
	Math::Vector3 moveDir = Math::Vector3::Zero;
	Math::Vector3 shaftPos = Math::Vector3::Zero;
	shaftPos.z = StageZ;
	float blockCutSize_1 = 1.0f; //軸で割ったときの一方のブロックの進行方向側、もしくは上側の大きさ
	float blockCutSize_2 = m_blockHalfSize.y * 2.0f - 1.0f; 
	KdDebugGUI::Instance().AddLog("now[%f],[%f],[%f]\n", StageX, StageY, StageZ);
	switch (Flg)
	{
		case 0:
			// X軸-方向に動く場合

			
			KdDebugGUI::Instance().AddLog("0START\n");
			
			// 1.軸になる場所を調べる
			for (int y = StageY + m_blockHalfSize.y - 1.0f;y > ceil(StageY - m_blockHalfSize.y-1.0f);y--) 
			// ブロックのY軸の上端の一つ下のブロックから下端の一つ下のブロックまでを調べる
			{
				for (int z = ceil(StageZ - m_blockHalfSize.z);z < StageZ + m_blockHalfSize.z;z++) 
					// ブロックのZ軸のマイナス方向の端のブロックからプラス方向の端のブロックまでを調べる
				{
					if (!ishit_up
					&& m_pStageData->at((int)(StageX - m_blockHalfSize.x)).at(y).at(z) != '0' 
					&& m_pStageData->at((int)(StageX - m_blockHalfSize.x)).at(y).at(z) != 'G') 
					{
						ishit_up = true;
						shaftPos.x = StageX - m_blockHalfSize.x;
						shaftPos.y = (float)(y)+0.5f;
					}
				}
				if(!ishit_up)
				{
					blockCutSize_1+=1.0f;
					blockCutSize_2-=1.0f;
				}
			}
			
			if (!ishit_up) //動かそうとするブロックの前方にブロックが存在しなかった場合
			{
				blockCutSize_1 = 0.0f; 
				blockCutSize_2 = m_blockHalfSize.x * 2.0f; 
				for (int x = ceil(StageX - m_blockHalfSize.x);x < StageX + m_blockHalfSize.x;x++)
				{
					for (int z = ceil(StageZ - m_blockHalfSize.z);z < StageZ + m_blockHalfSize.z;z++)
						// ブロックのZ軸のマイナス方向の端のブロックからプラス方向の端のブロックまでを調べる
					{
						if (!ishit_down
							&& m_pStageData->at(x).at((int)(StageY-m_blockHalfSize.y)).at(z) != '0'
							&& m_pStageData->at(x).at((int)(StageY - m_blockHalfSize.y)).at(z) != 'G')
						{
							ishit_down = true;
							shaftPos.x = (float)(x)-0.5f;
							shaftPos.y = StageY-m_blockHalfSize.y;
						}
					}
					if (!ishit_down)
					{
						blockCutSize_1 += 1.0f;
						blockCutSize_2 -= 1.0f;
					}
				}
			}

			// 動かそうとするブロックの前方一つ下にだけブロックがあった場合の対応をどちらにするかを考えなければいけない

			KdDebugGUI::Instance().AddLog("blockCutSize_1: %f, blockCutSize_2: %f\n", blockCutSize_1, blockCutSize_2);

			
			// 2.衝突判定をする
			for (int x = 0;x < StageSize;x++) 
			{
				for (int y = 0;y < StageSize;y++)
				{
					for (int z = ceil(StageZ - m_blockHalfSize.z);z < StageZ + m_blockHalfSize.z;z++)
					{
						//KdDebugGUI::Instance().AddLog("[%d][%d][%d]\n", x, y, z);
						char a = m_pStageData->at(x).at(y).at(z);
						if (a == '0' || a == 'G') continue; // 虚無とゴールは当たり判定なし
						
						if (x < StageX + m_blockHalfSize.x && x > StageX - m_blockHalfSize.x && 
							y < StageY + m_blockHalfSize.y && y > StageY - m_blockHalfSize.y) continue; 
						// 自分自身は当たり判定なし
						
						if (ishit_up)
						{
							// この時1つ目のブロックが第一象限、2つ目のブロックが第二象限に存在する。
							// 2つ目のブロックは存在しないこともあるが、その時はblockCutSize_2が0になるので、1つ目のブロックの判定のみ行われる。
							// 1つ目のブロックとの当たり判定
							// ブロックの横の方が大きければ動かすブロックの上端より上のマスかつ動かした後のブロックの左端より右のマス
							if(m_blockHalfSize.x*2.0f>blockCutSize_1){
								if (y > ceil(StageY + m_blockHalfSize.y) &&
									x > shaftPos.x - blockCutSize_1)
									// 動かした後のブロックの左端は軸を軸で割ったブロックの高さだけ左に動かした位置と同じ
								{
									int xdiff = (int)(trunc(shaftPos.x - (float)(x))); // 軸とのX軸方向の距離
									int ydiff = (int)(trunc(shaftPos.y - (float)(y))); // 軸とのY軸方向の距離
									if (sqrt(pow(xdiff, 2) + pow(ydiff, 2)) < sqrt(pow(m_blockHalfSize.x, 2) + pow(blockCutSize_1, 2)))
										// 軸との距離が、軸で割ったブロックの対角線より近い場合は当たり判定あり
									{
										KdDebugGUI::Instance().AddLog("[%d][%d][%d] %c hit1\n", x, y, z, a);
										ishit = true;
									}
								}
							}
							// そうでなければ動かした後のブロックの上端より上のマスかつ動かすブロックの右端より左のマスが円判定の対象となる。
							else if (y > shaftPos.y + m_blockHalfSize.x * 2.0f && 
								// 動かした後のブロックの上端は軸で割ったブロックの下端を軸で割ったブロックの横の長さだけ上に動かした位置と同じ
								x < StageX+m_blockHalfSize.x)
								
							{
								int xdiff = (int)(trunc(shaftPos.x - (float)(x))); // 軸とのX軸方向の距離
								int ydiff = (int)(trunc(shaftPos.y - (float)(y))); // 軸とのY軸方向の距離
								if (sqrt(pow(xdiff, 2) + pow(ydiff, 2)) < sqrt(pow(m_blockHalfSize.x, 2) + pow(blockCutSize_1, 2)))
									// 軸との距離が、軸で割ったブロックの対角線より近い場合は当たり判定あり
								{
									KdDebugGUI::Instance().AddLog("[%d][%d][%d] %c hit2\n", x, y, z, a);
									ishit = true;
								}
							}

							// 2つ目のブロックとの当たり判定
							if (blockCutSize_2 > 0.5f) {
								// ブロックの横の方が大きければ動かすブロックの下端より上のマスかつ動かした後のブロックの右端より右のマス
								if (m_blockHalfSize.x * 2.0f > blockCutSize_2) {
									if (y > StageY - m_blockHalfSize.y &&
										x > shaftPos.x + blockCutSize_2)
										// 動かした後のブロックの右端は軸を軸で割ったブロックの高さだけ右に動かした位置と同じ
									{
										int xdiff = (int)(trunc(shaftPos.x - (float)(x))); // 軸とのX軸方向の距離
										int ydiff = (int)(trunc(shaftPos.y - (float)(y))); // 軸とのY軸方向の距離
										if (sqrt(pow(xdiff, 2) + pow(ydiff, 2)) < sqrt(pow(m_blockHalfSize.x, 2) + pow(blockCutSize_2, 2)))
											// 軸との距離が、軸で割ったブロックの対角線より近い場合は当たり判定あり
										{
											KdDebugGUI::Instance().AddLog("[%d][%d][%d] %c hit3\n", x, y, z, a);
											ishit = true;
										}
									}
								}
								// そうでなければ動かした後のブロックの上端より下のマスかつ動かすブロックの右端より右のマスが円判定の対象となる。
								else if (y < shaftPos.y + m_blockHalfSize.x * 2.0f &&
									// 動かした後のブロックの上端は動かす前のブロックの上端をブロックの横の長さだけ上に動かした位置と同じ
									x > StageX + m_blockHalfSize.x)
								{
									int xdiff = (int)(trunc(shaftPos.x - (float)(x))); // 軸とのX軸方向の距離
									int ydiff = (int)(trunc(shaftPos.y - (float)(y))); // 軸とのY軸方向の距離
									if (sqrt(pow(xdiff, 2) + pow(ydiff, 2)) < sqrt(pow(m_blockHalfSize.x, 2) + pow(blockCutSize_2, 2)))
										// 軸との距離が、軸で割ったブロックの対角線より近い場合は当たり判定あり
									{
										KdDebugGUI::Instance().AddLog("[%d][%d][%d] %c hit4\n", x, y, z, a);
										ishit = true;
									}
								}
							}
							// 移動先のブロックとの判定
							if( x > shaftPos.x - blockCutSize_1 &&
								x < shaftPos.x + blockCutSize_2 &&
								y > shaftPos.y &&
								y < shaftPos.y + m_blockHalfSize.x * 2.0f)
							{
								KdDebugGUI::Instance().AddLog("[%d][%d][%d] %c hit\n", x, y, z, a);
								ishit = true;
							}
						}
						else if (ishit_down) // 本来ishit_upがfalseの時にishit_downがfalseになることはないが、念のため
						{
							// この時1つ目のブロックが第四象限、2つ目のブロックが第一象限に存在する。
							// 2つ目のブロックは存在しないこともあるが、その時はblockCutSize_1が0になるので、2つ目のブロックの判定のみ行われる。

							if(blockCutSize_1>0.5f){
								// ブロックの横の方が大きければ動かすブロックの上端より下のマスかつ動かした後のブロックの左端より左のマス
								if (m_blockHalfSize.x * 2.0f > blockCutSize_1) {
									if (y < StageY + m_blockHalfSize.y &&
										x < shaftPos.x - m_blockHalfSize.y * 2.0f)
									{
										int xdiff = (int)(trunc(shaftPos.x - (float)(x))); // 軸とのX軸方向の距離
										int ydiff = (int)(trunc(shaftPos.y - (float)(y))); // 軸とのY軸方向の距離
										if (sqrt(pow(xdiff, 2) + pow(ydiff, 2)) < sqrt(pow(blockCutSize_1, 2) + pow(m_blockHalfSize.y, 2)))
											// 軸との距離が、軸で割ったブロックの対角線より近い場合は当たり判定あり
										{
											KdDebugGUI::Instance().AddLog("[%d][%d][%d] %c hit5\n", x, y, z, a);
											ishit = true;
										}
									}
								}
								// そうでなければ動かした後のブロックの下端より上のマスかつ動かすブロックの左端より左のマスが円判定の対象となる。
								else if (y > shaftPos.y - blockCutSize_1 &&
									x < StageX - m_blockHalfSize.x)
								{
									int xdiff = (int)(trunc(shaftPos.x - (float)(x))); // 軸とのX軸方向の距離
									int ydiff = (int)(trunc(shaftPos.y - (float)(y))); // 軸とのY軸方向の距離
									if (sqrt(pow(xdiff, 2) + pow(ydiff, 2)) < sqrt(pow(blockCutSize_1, 2) + pow(m_blockHalfSize.y, 2)))
										// 軸との距離が、軸で割ったブロックの対角線より近い場合は当たり判定あり
									{
										KdDebugGUI::Instance().AddLog("[%d][%d][%d] %c hit6\n", x, y, z, a);
										ishit = true;
									}
								}
							}
							if (m_blockHalfSize.x * 2.0f > blockCutSize_2) {
								// ブロックの横の方が大きければ動かすブロックの上端より上のマスかつ動かした後のブロックの左端より右のマス
								if (y > StageY + m_blockHalfSize.y &&
									x > shaftPos.x - m_blockHalfSize.y * 2.0f)
									// 動かした後のブロックの左端は軸を軸で割ったブロックの高さだけ左に動かした位置と同じ
								{
									int xdiff = (int)(trunc(shaftPos.x - (float)(x))); // 軸とのX軸方向の距離
									int ydiff = (int)(trunc(shaftPos.y - (float)(y))); // 軸とのY軸方向の距離
									if (sqrt(pow(xdiff, 2) + pow(ydiff, 2)) < sqrt(pow(blockCutSize_2, 2) + pow(m_blockHalfSize.y, 2)))
										// 軸との距離が、軸で割ったブロックの対角線より近い場合は当たり判定あり
									{
										KdDebugGUI::Instance().AddLog("[%d][%d][%d] %c hit7\n", x, y, z, a);
										ishit = true;
									}
								}
							}
							// そうでなければ動かした後のブロックの上端より上のマスかつ動かすブロックの右端より左のマスが円判定の対象となる。
							else if (y > shaftPos.y + blockCutSize_2 &&
								// 動かした後のブロックの上端は軸で割ったブロックの下端を軸で割ったブロックの横の長さだけ上に動かした位置と同じ
								x < StageX + m_blockHalfSize.x)

							{
								int xdiff = (int)(trunc(shaftPos.x - (float)(x))); // 軸とのX軸方向の距離
								int ydiff = (int)(trunc(shaftPos.y - (float)(y))); // 軸とのY軸方向の距離
								if (sqrt(pow(xdiff, 2) + pow(ydiff, 2)) < sqrt(pow(blockCutSize_2, 2) + pow(m_blockHalfSize.y, 2)))
									// 軸との距離が、軸で割ったブロックの対角線より近い場合は当たり判定あり
								{
									KdDebugGUI::Instance().AddLog("[%d][%d][%d] %c hit8\n", x, y, z, a);
									ishit = true;
								}
							}
							// 移動先のブロックとの判定
							if (x < shaftPos.x&&
								x > shaftPos.x - m_blockHalfSize.y * 2.0f &&
								y < shaftPos.y + blockCutSize_2 &&
								y > shaftPos.y - blockCutSize_1)
							{
								KdDebugGUI::Instance().AddLog("[%d][%d][%d] %c hit\n", x, y, z, a);
								ishit = true;
							}
						}
					}
				}
			}

			// 4.動かす
			if (!ishit)
			{
				KdDebugGUI::Instance().AddLog("0OK\n");
				for (int x=ceil(StageX-m_blockHalfSize.x);x < StageX + m_blockHalfSize.x;x++)
				{
					for (int y = ceil(StageY-m_blockHalfSize.y);y < StageY+m_blockHalfSize.y;y++)
					{
						for (int z =ceil(StageZ-m_blockHalfSize.z);z < StageZ + m_blockHalfSize.z;z++)
						{
							if (m_pStageData->at(x).at(y).at(z) == 'b')
							{
								m_pStage->SubtractGoal();
								m_pStage->SetStageData(x, y, z, 'G');
							}
							else
							{
								m_pStage->SetStageData(x, y, z, '0');
							}
						}
					}
				}
				float xdiff = shaftPos.x - StageX; // 軸とのX軸方向の距離
				float ydiff = shaftPos.y - StageY; // 軸とのY軸方向の距離
				float center = (float)(StageSize - 1) / 2.0f;
				m_pos = Math::Vector3(squaresize * (shaftPos.x - center), squaresize * shaftPos.y, -squaresize * (shaftPos.z - center));
				m_localPos = Math::Vector3(-xdiff*squaresize, -ydiff * squaresize, 0);
				RotMat *= localRotMat;
				localRotMat = Math::Matrix::Identity;
				//moveDir = Math::Vector3(-1, 0, 0);
				StageX += xdiff + ydiff;
				StageY += ydiff - xdiff;
				
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
						if (z - StageZ <= m_blockType) 
						{
							KdDebugGUI::Instance().AddLog("[%d][%d][%d] %c Hit\n", round(StageX), y, z, a);
							ishit = true;
						}
					}
					else
					{
						int zdiff = max(0, z - (int)(StageZ) - 1);
						int ydiff = StageY - y;
						if (sqrt(pow(zdiff, 2) + pow(ydiff, 2)) < sqrt(m_blockType * 2))
						{
							KdDebugGUI::Instance().AddLog("[%d][%d][%d] %c Hit\n", round(StageX), y, z, a);
							ishit = true;
						}
					}
				}
			}
			if (!ishit)
			{
				KdDebugGUI::Instance().AddLog("1OK\n");
				for (int x = ceil(StageX - m_blockHalfSize.x);x < StageX + m_blockHalfSize.x;x++)
				{
					for (int y = ceil(StageY - m_blockHalfSize.y);y < StageY + m_blockHalfSize.y;y++)
					{
						for (int z = ceil(StageZ - m_blockHalfSize.z);z < StageZ + m_blockHalfSize.z;z++)
						{
							if (m_pStageData->at(x).at(y).at(z) == 'b')
							{
								m_pStage->SubtractGoal();
								m_pStage->SetStageData(x, y, z, 'G');
							}
							else
							{
								m_pStage->SetStageData(x, y, z, '0');
							}
						}
					}
				}
				float center = (float)(StageSize - 1) / 2.0f;
				// これはブロックが1×1の時の処理なので、ブロックが1×2や1×3の時は端座標を計算しなおす必要がある
				m_pos = Math::Vector3(squaresize * (StageX - center), squaresize * StageY - squaresize / 2.0f, -squaresize * (StageZ - center) - squaresize / 2.0f);
				m_localPos = Math::Vector3(0.0f, squaresize / 2.0f, squaresize / 2.0f);
				localRotMat = Math::Matrix::Identity;
				moveDir = Math::Vector3(0, 0, -1);
				StageZ+=1.0f;
			}
			break;
		case 2:
			// X軸+方向に動く場合
			// 今回はX座標を反転させてX軸-方向に動かす場合と同じ処理をする

			KdDebugGUI::Instance().AddLog("2START\n");

			// 1.軸になる場所を調べる
			for (int y = StageY + m_blockHalfSize.y - 1.0f;y > ceil(StageY - m_blockHalfSize.y - 1.0f);y--)
				// ブロックのY軸の上端の一つ下のブロックから下端の一つ下のブロックまでを調べる
			{
				for (int z = ceil(StageZ - m_blockHalfSize.z);z < StageZ + m_blockHalfSize.z;z++)
					// ブロックのZ軸のマイナス方向の端のブロックからプラス方向の端のブロックまでを調べる
				{
					if (!ishit_up
						&& m_pStageData->at((int)(ceil(StageX + m_blockHalfSize.x))).at(y).at(z) != '0'
						&& m_pStageData->at((int)(ceil(StageX + m_blockHalfSize.x))).at(y).at(z) != 'G')
					{
						ishit_up = true;
						shaftPos.x = StageX + m_blockHalfSize.x;
						shaftPos.y = (float)(y)+0.5f;
					}
				}
				if (!ishit_up)
				{
					blockCutSize_1 += 1.0f;
					blockCutSize_2 -= 1.0f;
				}
			}

			if (!ishit_up) //動かそうとするブロックの前方にブロックが存在しなかった場合
			{
				blockCutSize_1 = 0.0f;
				blockCutSize_2 = m_blockHalfSize.x * 2.0f;
				for (int x = (int)(StageX + m_blockHalfSize.x);x > StageX - m_blockHalfSize.x;x--)
				{
					for (int z = ceil(StageZ - m_blockHalfSize.z);z < StageZ + m_blockHalfSize.z;z++)
						// ブロックのZ軸のマイナス方向の端のブロックからプラス方向の端のブロックまでを調べる
					{
						if (!ishit_down
							&& m_pStageData->at(x).at((int)(StageY - m_blockHalfSize.y)).at(z) != '0'
							&& m_pStageData->at(x).at((int)(StageY - m_blockHalfSize.y)).at(z) != 'G')
						{
							ishit_down = true;
							shaftPos.x = (float)(x)+0.5f;
							shaftPos.y = StageY - m_blockHalfSize.y;
						}
					}
					if (!ishit_down)
					{
						blockCutSize_1 += 1.0f;
						blockCutSize_2 -= 1.0f;
					}
				}
			}

			// 動かそうとするブロックの前方一つ下にだけブロックがあった場合の対応をどちらにするかを考えなければいけない

			KdDebugGUI::Instance().AddLog("blockCutSize_1: %f, blockCutSize_2: %f\n", blockCutSize_1, blockCutSize_2);


			// 2.衝突判定をする
			for (int x = 0;x < StageSize;x++)
			{
				for (int y = 0;y < StageSize;y++)
				{
					for (int z = ceil(StageZ - m_blockHalfSize.z);z < StageZ + m_blockHalfSize.z;z++)
					{
						//KdDebugGUI::Instance().AddLog("[%d][%d][%d]\n", x, y, z);
						char a = m_pStageData->at(x).at(y).at(z);
						if (a == '0' || a == 'G') continue; // 虚無とゴールは当たり判定なし

						if (x < StageX + m_blockHalfSize.x && x > StageX - m_blockHalfSize.x &&
							y < StageY + m_blockHalfSize.y && y > StageY - m_blockHalfSize.y) continue;
						// 自分自身は当たり判定なし

						if (ishit_up)
						{
							// この時1つ目のブロックが第一象限、2つ目のブロックが第二象限に存在する。
							// 2つ目のブロックは存在しないこともあるが、その時はblockCutSize_2が0になるので、1つ目のブロックの判定のみ行われる。
							// 1つ目のブロックとの当たり判定
							// ブロックの横の方が大きければ動かすブロックの上端より上のマスかつ動かした後のブロックの右端より左のマス
							if (m_blockHalfSize.x * 2.0f > blockCutSize_1) {
								if (y > ceil(StageY + m_blockHalfSize.y) &&
									x < shaftPos.x + blockCutSize_1)
									// 動かした後のブロックの右端は軸を軸で割ったブロックの高さだけ右に動かした位置と同じ
								{
									int xdiff = (int)(trunc(shaftPos.x - (float)(x))); // 軸とのX軸方向の距離
									int ydiff = (int)(trunc(shaftPos.y - (float)(y))); // 軸とのY軸方向の距離
									if (sqrt(pow(xdiff, 2) + pow(ydiff, 2)) < sqrt(pow(m_blockHalfSize.x, 2) + pow(blockCutSize_1, 2)))
										// 軸との距離が、軸で割ったブロックの対角線より近い場合は当たり判定あり
									{
										KdDebugGUI::Instance().AddLog("[%d][%d][%d] %c hit1\n", x, y, z, a);
										ishit = true;
									}
								}
							}
							// そうでなければ動かした後のブロックの上端より上のマスかつ動かすブロックの左端より右のマスが円判定の対象となる。
							else if (y > shaftPos.y + m_blockHalfSize.x * 2.0f &&
								// 動かした後のブロックの上端は軸で割ったブロックの下端を軸で割ったブロックの横の長さだけ上に動かした位置と同じ
								x > StageX - m_blockHalfSize.x)

							{
								int xdiff = (int)(trunc(shaftPos.x - (float)(x))); // 軸とのX軸方向の距離
								int ydiff = (int)(trunc(shaftPos.y - (float)(y))); // 軸とのY軸方向の距離
								if (sqrt(pow(xdiff, 2) + pow(ydiff, 2)) < sqrt(pow(m_blockHalfSize.x, 2) + pow(blockCutSize_1, 2)))
									// 軸との距離が、軸で割ったブロックの対角線より近い場合は当たり判定あり
								{
									KdDebugGUI::Instance().AddLog("[%d][%d][%d] %c hit2\n", x, y, z, a);
									ishit = true;
								}
							}

							// 2つ目のブロックとの当たり判定
							if (blockCutSize_2 > 0.5f) {
								// ブロックの横の方が大きければ動かすブロックの下端より上のマスかつ動かした後のブロックの左端より左のマス
								if (m_blockHalfSize.x * 2.0f > blockCutSize_2) {
									if (y > StageY - m_blockHalfSize.y &&
										x < shaftPos.x - blockCutSize_2)
										// 動かした後のブロックの左端は軸を軸で割ったブロックの高さだけ左に動かした位置と同じ
									{
										int xdiff = (int)(trunc(shaftPos.x - (float)(x))); // 軸とのX軸方向の距離
										int ydiff = (int)(trunc(shaftPos.y - (float)(y))); // 軸とのY軸方向の距離
										if (sqrt(pow(xdiff, 2) + pow(ydiff, 2)) < sqrt(pow(m_blockHalfSize.x, 2) + pow(blockCutSize_2, 2)))
											// 軸との距離が、軸で割ったブロックの対角線より近い場合は当たり判定あり
										{
											KdDebugGUI::Instance().AddLog("[%d][%d][%d] %c hit3\n", x, y, z, a);
											ishit = true;
										}
									}
								}
								// そうでなければ動かした後のブロックの上端より下のマスかつ動かすブロックの左端より左のマスが円判定の対象となる。
								else if (y < shaftPos.y + m_blockHalfSize.x * 2.0f &&
									// 動かした後のブロックの上端は動かす前のブロックの上端をブロックの横の長さだけ上に動かした位置と同じ
									x < StageX - m_blockHalfSize.x)
								{
									int xdiff = (int)(trunc(shaftPos.x - (float)(x))); // 軸とのX軸方向の距離
									int ydiff = (int)(trunc(shaftPos.y - (float)(y))); // 軸とのY軸方向の距離
									if (sqrt(pow(xdiff, 2) + pow(ydiff, 2)) < sqrt(pow(m_blockHalfSize.x, 2) + pow(blockCutSize_2, 2)))
										// 軸との距離が、軸で割ったブロックの対角線より近い場合は当たり判定あり
									{
										KdDebugGUI::Instance().AddLog("[%d][%d][%d] %c hit4\n", x, y, z, a);
										ishit = true;
									}
								}
							}
							// 移動先のブロックとの判定
							if (x < shaftPos.x + blockCutSize_1 &&
								x > shaftPos.x - blockCutSize_2 &&
								y < shaftPos.y + m_blockHalfSize.x * 2.0f &&
								y > shaftPos.y)
							{
								KdDebugGUI::Instance().AddLog("[%d][%d][%d] %c hit\n", x, y, z, a);
								ishit = true;
							}
						}
						else if (ishit_down) // 本来ishit_upがfalseの時にishit_downがfalseになることはないが、念のため
						{
							// この時1つ目のブロックが第四象限、2つ目のブロックが第一象限に存在する。
							// 2つ目のブロックは存在しないこともあるが、その時はblockCutSize_1が0になるので、2つ目のブロックの判定のみ行われる。

							if (blockCutSize_1 > 0.5f) {
								// ブロックの横の方が大きければ動かすブロックの上端より下のマスかつ動かした後のブロックの右端より右のマス
								if (m_blockHalfSize.x * 2.0f > blockCutSize_1) {
									if (y < StageY + m_blockHalfSize.y &&
										x > shaftPos.x + m_blockHalfSize.y * 2.0f)
									{
										int xdiff = (int)(trunc(shaftPos.x - (float)(x))); // 軸とのX軸方向の距離
										int ydiff = (int)(trunc(shaftPos.y - (float)(y))); // 軸とのY軸方向の距離
										if (sqrt(pow(xdiff, 2) + pow(ydiff, 2)) < sqrt(pow(blockCutSize_1, 2) + pow(m_blockHalfSize.y, 2)))
											// 軸との距離が、軸で割ったブロックの対角線より近い場合は当たり判定あり
										{
											KdDebugGUI::Instance().AddLog("[%d][%d][%d] %c hit5\n", x, y, z, a);
											ishit = true;
										}
									}
								}
								// そうでなければ動かした後のブロックの下端より上のマスかつ動かすブロックの右端より右のマスが円判定の対象となる。
								else if (y > shaftPos.y - blockCutSize_1 &&
									x > StageX + m_blockHalfSize.x)
								{
									int xdiff = (int)(trunc(shaftPos.x - (float)(x))); // 軸とのX軸方向の距離
									int ydiff = (int)(trunc(shaftPos.y - (float)(y))); // 軸とのY軸方向の距離
									if (sqrt(pow(xdiff, 2) + pow(ydiff, 2)) < sqrt(pow(blockCutSize_1, 2) + pow(m_blockHalfSize.y, 2)))
										// 軸との距離が、軸で割ったブロックの対角線より近い場合は当たり判定あり
									{
										KdDebugGUI::Instance().AddLog("[%d][%d][%d] %c hit6\n", x, y, z, a);
										ishit = true;
									}
								}
							}
							if (m_blockHalfSize.x * 2.0f > blockCutSize_2) {
								// ブロックの横の方が大きければ動かすブロックの上端より上のマスかつ動かした後のブロックの右端より左のマス
								if (y > StageY + m_blockHalfSize.y &&
									x < shaftPos.x + m_blockHalfSize.y*2.0f)
									// 動かした後のブロックの右端は軸を軸で割ったブロックの高さだけ右に動かした位置と同じ
								{
									int xdiff = (int)(trunc(shaftPos.x - (float)(x))); // 軸とのX軸方向の距離
									int ydiff = (int)(trunc(shaftPos.y - (float)(y))); // 軸とのY軸方向の距離
									if (sqrt(pow(xdiff, 2) + pow(ydiff, 2)) < sqrt(pow(blockCutSize_2, 2) + pow(m_blockHalfSize.y, 2)))
										// 軸との距離が、軸で割ったブロックの対角線より近い場合は当たり判定あり
									{
										KdDebugGUI::Instance().AddLog("[%d][%d][%d] %c hit7\n", x, y, z, a);
										ishit = true;
									}
								}
							}
							// そうでなければ動かした後のブロックの上端より上のマスかつ動かすブロックの左端より右のマスが円判定の対象となる。
							else if (y > shaftPos.y + blockCutSize_2 &&
								// 動かした後のブロックの上端は軸で割ったブロックの下端を軸で割ったブロックの横の長さだけ上に動かした位置と同じ
								x > StageX - m_blockHalfSize.x)

							{
								int xdiff = (int)(trunc(shaftPos.x - (float)(x))); // 軸とのX軸方向の距離
								int ydiff = (int)(trunc(shaftPos.y - (float)(y))); // 軸とのY軸方向の距離
								if (sqrt(pow(xdiff, 2) + pow(ydiff, 2)) < sqrt(pow(blockCutSize_2, 2) + pow(m_blockHalfSize.y, 2)))
									// 軸との距離が、軸で割ったブロックの対角線より近い場合は当たり判定あり
								{
									KdDebugGUI::Instance().AddLog("[%d][%d][%d] %c hit8\n", x, y, z, a);
									ishit = true;
								}
							}
							// 移動先のブロックとの判定
							if (x > shaftPos.x &&
								x < shaftPos.x + m_blockHalfSize.y * 2.0f &&
								y > shaftPos.y - blockCutSize_1 &&
								y < shaftPos.y + blockCutSize_2)
							{
								KdDebugGUI::Instance().AddLog("[%d][%d][%d] %c hit\n", x, y, z, a);
								ishit = true;
							}
						}
					}
				}
			}

			// 4.動かす
			if (!ishit)
			{
				KdDebugGUI::Instance().AddLog("2OK\n");
				for (int x = ceil(StageX - m_blockHalfSize.x);x < StageX + m_blockHalfSize.x;x++)
				{
					for (int y = ceil(StageY - m_blockHalfSize.y);y < StageY + m_blockHalfSize.y;y++)
					{
						for (int z = ceil(StageZ - m_blockHalfSize.z);z < StageZ + m_blockHalfSize.z;z++)
						{
							if (m_pStageData->at(x).at(y).at(z) == 'b')
							{
								m_pStage->SubtractGoal();
								m_pStage->SetStageData(x, y, z, 'G');
							}
							else
							{
								m_pStage->SetStageData(x, y, z, '0');
							}
						}
					}
				}
				float xdiff = shaftPos.x - StageX; // 軸とのX軸方向の距離
				float ydiff = shaftPos.y - StageY; // 軸とのY軸方向の距離
				float center = (float)(StageSize - 1) / 2.0f;
				m_pos = Math::Vector3(squaresize * (shaftPos.x - center), squaresize * shaftPos.y, -squaresize * (shaftPos.z - center));
				m_localPos = Math::Vector3(-xdiff * squaresize, -ydiff * squaresize, 0);
				RotMat *= localRotMat;
				localRotMat = Math::Matrix::Identity;
				//moveDir = Math::Vector3(-1, 0, 0);
				StageX += xdiff - ydiff;
				StageY += -ydiff - xdiff;

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
						if (StageZ - z <= m_blockType)
						{
							KdDebugGUI::Instance().AddLog("[%d][%d][%d] %c Hit\n", round(StageX), y, z, a);
							ishit = true;
						}
					}
					else
					{
						int zdiff = max(0, (int)(StageZ) - z - 1);
						int ydiff = StageY - y;
						if (sqrt(pow(zdiff, 2) + pow(ydiff, 2)) < sqrt(m_blockType * 2)) 
						{
							KdDebugGUI::Instance().AddLog("[%d][%d][%d] %c Hit\n", round(StageX), y, z, a);
							ishit = true;
						}
					}
				}
			}

			if (!ishit)
			{
				KdDebugGUI::Instance().AddLog("3OK\n");
				for (int x = ceil(StageX - m_blockHalfSize.x);x < StageX + m_blockHalfSize.x;x++)
				{
					for (int y = ceil(StageY - m_blockHalfSize.y);y < StageY + m_blockHalfSize.y;y++)
					{
						for (int z = ceil(StageZ - m_blockHalfSize.z);z < StageZ + m_blockHalfSize.z;z++)
						{
							if (m_pStageData->at(x).at(y).at(z) == 'b')
							{
								m_pStage->SubtractGoal();
								m_pStage->SetStageData(x, y, z, 'G');
							}
							else
							{
								m_pStage->SetStageData(x, y, z, '0');
							}
						}
					}
				}
				float center = (float)(StageSize - 1) / 2.0f;
				// これはブロックが1×1の時の処理なので、ブロックが1×2や1×3の時は端座標を計算しなおす必要がある
				m_pos = Math::Vector3(squaresize * (StageX - center), squaresize * StageY - squaresize / 2.0f, -squaresize * (StageZ - center) + squaresize / 2.0f);
				m_localPos = Math::Vector3(0.0f, squaresize / 2.0f, -squaresize / 2.0f);
				localRotMat = Math::Matrix::Identity;
				moveDir = Math::Vector3(0, 0, 1);
				StageZ-=1.0f;
			}
			break;
	}
	if (!ishit) //当たってないなら
		{
			rollFlg = Flg;
			rollCount = 0;
		}
}

