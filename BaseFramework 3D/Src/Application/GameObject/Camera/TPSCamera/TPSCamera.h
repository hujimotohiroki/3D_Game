#pragma once
#include "../CameraBase.h"

class TPSCamera : public CameraBase
{
public:
	TPSCamera()							{}
	~TPSCamera()			override	{}

	void LookTopChara() { m_mRotation=Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(180)); }
	void LookSideChara() { m_mRotation=Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(0)); }

	void Init()				override;
	void PostUpdate()		override;
};