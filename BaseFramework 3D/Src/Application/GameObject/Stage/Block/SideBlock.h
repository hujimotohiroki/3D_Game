#pragma once
#include "../StageObjectBase.h"

using namespace std;

class Stage;

class SideBlock :public StageObjectBase
{
public:
	void Init() override;
	void Update()override;
	void PostUpdate()override;
	//void GenerateDepthMapFromLight()override;
	void DrawLit()override;
	void SetTarget(std::weak_ptr<KdGameObject> target) { m_wpTarget = target; }
	void SetBlockType(int type) {m_blockType = type;}
	void SetBlockSize(Math::Vector3 size) { m_blockSize = size; m_collisionHalfSize = m_blockSize * 8.0f; }
	Math::Vector3 GetBlockSize() { return m_blockSize*squaresize/2.0f; }
	void PushBlock(int Flg);
	void SetStage(Stage* stage) { m_pStage = stage; }
	void SetStagePos(int x, int y, int z) { StageX = x; StageY = y; StageZ = z; }
	void SetStageSize(int size) { StageSize = size; }

	

	void SetStageDataptr(array<array<array<char, 10>, 10>, 10>* stageData) { m_pStageData = stageData; }
private:
	const float squaresize = 16.0f;

	Stage* m_pStage = nullptr;
	std::weak_ptr<KdGameObject> m_wpTarget;
	Math::Vector3 Targetpos = Math::Vector3::Zero;
	Math::Matrix RotMat = Math::Matrix::Identity;
	Math::Vector3 m_localPos = Math::Vector3::Zero;
	Math::Vector3 m_localRot = Math::Vector3::Zero;
	Math::Matrix localRotMat = Math::Matrix::Identity;
	
	int StageX, StageY, StageZ;
	int StageSize;
	DirectX::BoundingOrientedBox blockBox;
	int m_blockType = 1;
	Math::Vector3 m_blockSize = {1, 1, 1};
	Math::Vector3 m_collisionHalfSize = {8,8,8};
	int rollFlg = -1;
	int rollCount = 0;
	array<array<array<char, 10>, 10>, 10>* m_pStageData = { nullptr };
};