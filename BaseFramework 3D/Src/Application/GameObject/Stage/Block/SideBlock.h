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
	void SetBlockSize(int size) {m_blockSize = size;}
	Math::Vector3 GetBlockSize() { return Math::Vector3::TransformNormal(m_collisionHalfSize, RotMat); }
	void PushBlock(int Flg);
	void SetStage(Stage* stage) { m_pStage = stage; }
	void SetStagePos(int x, int y, int z) { StageX = x; StageY = y; StageZ = z; }
	void SetStageSize(int size) { StageSize = size; }

	void SetStageDataptr(array<array<array<char, 10>, 10>, 10>* stageData) { m_pStageData = stageData; }
private:
	Stage* m_pStage = nullptr;
	std::weak_ptr<KdGameObject> m_wpTarget;
	Math::Vector3 Targetpos = Math::Vector3::Zero;
	Math::Matrix RotMat = Math::Matrix::Identity;
	Math::Matrix localRotMat = Math::Matrix::Identity;
	Math::Matrix localPosMat = Math::Matrix::Identity;
	int StageX, StageY, StageZ;
	int StageSize;
	DirectX::BoundingOrientedBox blockBox;
	int m_blockSize = 1;
	Math::Vector3 m_collisionHalfSize = Math::Vector3(8.0f, 8.0f, 8.0f);
	int rollFlg = 0;
	int rollCount = 0;
	array<array<array<char, 10>, 10>, 10>* m_pStageData = { nullptr };
};