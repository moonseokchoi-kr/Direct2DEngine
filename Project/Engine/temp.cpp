#include "pch.h"
#include "temp.h"
#include "CDevice.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CPathManager.h"
#include "CResourceManager.h"

#include "CMesh.h"
#include "CTexture.h"
#include "CConstBuffer.h"
#include "CShader.h"

/// <summary>
/// 붉은 사각형 그리기
/// </summary>


Vec4 g_moveDir = Vec4();
Vec4 g_angle = Vec4();
void init()
{

}


void Update()
{
	if (KEY_HOLD(KEY::UP))
	{
		g_moveDir.y += fDT * 0.5f;
	}
	if (KEY_HOLD(KEY::DOWN))
	{
		g_moveDir.y -= fDT * 0.5f;
	}
	if (KEY_HOLD(KEY::LEFT))
	{
		g_moveDir.x -= fDT * 0.5f;
	}
	if (KEY_HOLD(KEY::RIGHT))
	{
		g_moveDir.x += fDT * 0.5f;
	}

	CConstBuffer* cb = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
	cb->SetData(&g_moveDir, sizeof(Vec4));
}

void Render()
{
	CDevice::GetInst()->ClearTarget();

	
	CConstBuffer* cb = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
	cb->SetPipelineState(PS_VERTEX);
	cb->UpdateData();


	CMesh* mesh = CResourceManager::GetInst()->FindRes<CMesh>(L"RectMesh");
	CShader* stdShader = CResourceManager::GetInst()->FindRes<CShader>(L"std2DShader");

	CTexture* tex = CResourceManager::GetInst()->FindRes<CTexture>(L"Background");
	tex->SetPipelineStage(PS_PIXEL, 0);
	stdShader->UpdateData();
	tex->UpdateData();
	mesh->Render();
	CDevice::GetInst()->Present();
}
