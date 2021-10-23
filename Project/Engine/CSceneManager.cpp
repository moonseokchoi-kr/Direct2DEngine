#include "pch.h"
#include "CSceneManager.h"

#include "CCamera.h"
#include "CGameObject.h"
#include "CMaterial.h"
#include "CCollider2D.h"
#include "CMeshRender.h"
#include "CPlayerScript.h"
#include "CResourceManager.h"
#include "CCollisionManager.h"
#include "CScene.h"
#include "CLayer.h"
#include "CTransform.h"

CSceneManager::CSceneManager()
	:current_scene_(nullptr)
{
	
}

CSceneManager::~CSceneManager()
{
	SafeDelete(current_scene_);
}


void CSceneManager::Init()
{
	current_scene_ = new CScene;


	const auto camera = new CGameObject;
	camera->AddComponent(new CTransform);
	camera->AddComponent(new CCamera);
	camera->Transform()->SetPos(Vec3(0.f, 0.f, 0.f));

	current_scene_->AddGameObject(camera, 0,true);

	const auto object = new CGameObject;
	object->SetName(L"Player");
	object->AddComponent(new CTransform);
	object->AddComponent(new CMeshRender);
	object->AddComponent(new CCollider2D);
	object->Transform()->SetPos(Vec3(0.f, 0.f, 300.f));
	object->Transform()->SetScale(Vec3(100.f, 100.f, 1.f));
	object->Collider2D()->SetOffsetScale(Vec2(0.85f, 0.85f));
	object->MeshRender()->SetMesh(CResourceManager::GetInst()->FindRes<CMesh>(L"CircleMesh"));
	object->MeshRender()->SetMaterial(CResourceManager::GetInst()->FindRes<CMaterial>(L"std2DMaterial"));

	Ptr<CMaterial> material = object->MeshRender()->GetMaterial();
	int a = 0;

	material->SetData(SHADER_PARAM::INT_3, &a);
	material->SetData(SHADER_PARAM::TEX_0, CResourceManager::GetInst()->FindRes<CTexture>(L"Background").Get());
	const auto copyObject = object->Clone();
	object->AddComponent(new CPlayerScript);
// 	const auto childObject = new CGameObject;
// 	childObject->SetName(L"Child");
// 	childObject->AddComponent(new CTransform);
// 	childObject->AddComponent(new CMeshRender);
// 
// 	childObject->Transform()->SetPos(Vec3(2.f, 0.f, 0.f));
// 	childObject->Transform()->SetScale(Vec3(1.f, 1.f, 1.f));
// 	childObject->MeshRender()->SetMesh(CResourceManager::GetInst()->FindRes<CMesh>(L"RectMesh"));
// 	childObject->MeshRender()->SetMaterial(CResourceManager::GetInst()->FindRes<CMaterial>(L"std2DMaterial"));
// 
// 
// 	object->AddChild(childObject);

	current_scene_->AddGameObject(object, 0,true);


	
	copyObject->SetName(L"Monster");
	copyObject->Transform()->SetPos(Vec3(-200.f, -200.f, 400.f));
	current_scene_->AddGameObject(copyObject, 1, true);

	CCollisionManager::GetInst()->CheckLayer(0, 1);

}

void CSceneManager::Progress()
{
	current_scene_->Update();
	current_scene_->LateUpdate();
	current_scene_->FinalUpdate();
	CCollisionManager::GetInst()->Update();
	current_scene_->Render();
}
