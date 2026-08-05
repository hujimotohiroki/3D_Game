#include "StageSelect.h"

void StageSelect::Init()
{
	nowselect = 1;
}

void StageSelect::Update()
{
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		nowselect++;
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		nowselect--;
		if (nowselect < 1) nowselect = 1;
	}
}

void StageSelect::DrawSprite()
{
	//KdShaderManager::Instance().m_spriteShader.DrawTex();
}