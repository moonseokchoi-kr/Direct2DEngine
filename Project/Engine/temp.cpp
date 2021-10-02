#include "pch.h"
#include "temp.h"
#include "CDevice.h"
#include "CResourceManager.h"
#include "CKeyManager.h"


#include "CMesh.h"
#include "CTexture.h"
#include "CConstBuffer.h"
#include "CShader.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"
/// <summary>
/// 붉은 사각형 그리기
/// </summary>


CGameObject* g_obj = nullptr;

void init()
{
	g_obj = new CGameObject;
	g_obj->AddComponent(new CTransform);
	g_obj->AddComponent(new CMeshRender);
	g_obj->MeshRender()->SetMesh(CResourceManager::GetInst()->FindRes<CMesh>(L"RectMesh"));
	g_obj->MeshRender()->SetTexture(CResourceManager::GetInst()->FindRes<CTexture>(L"Background"));
	g_obj->MeshRender()->SetShader(CResourceManager::GetInst()->FindRes<CShader>(L"std2DShader"));
}


void Update()
{


	if (KEY_HOLD(KEY::ALT))
	{
		if (KEY_HOLD(KEY::ENTER))
		{
			if (CDevice::GetInst()->IsWindow())
				CDevice::GetInst()->OnReSize(Vec2(1920, 1080));
			else
				CDevice::GetInst()->OnReSize(Vec2(1600, 900));
		}
	}
	g_obj->Update();

}

void Render()
{
	CDevice::GetInst()->ClearTarget();

	g_obj->Render();
	
	CDevice::GetInst()->Present();
}
