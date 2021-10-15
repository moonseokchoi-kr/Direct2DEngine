#include "pch.h"
#include "CSceneManager.h"

#include "CCamera.h"
#include "CGameObject.h"
#include "CMaterial.h"
#include "CMeshRender.h"
#include "CPlayerScript.h"
#include "CResourceManager.h"
#include "CScene.h"
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

	current_scene_->AddGameObject(camera, 0);

	const auto object = new CGameObject;
	object->AddComponent(new CTransform);
	object->AddComponent(new CMeshRender);
	object->AddComponent(new CPlayerScript);
	object->Transform()->SetPos(Vec3(0.f, 0.f, 300.f));
	object->Transform()->SetScale(Vec3(1.f, 1.f, 1.f) * 100);
	object->MeshRender()->SetMesh(CResourceManager::GetInst()->FindRes<CMesh>(L"CircleMesh"));
	object->MeshRender()->SetMaterial(CResourceManager::GetInst()->FindRes<CMaterial>(L"std2DMaterial"));

	Ptr<CMaterial> material = object->MeshRender()->GetMaterial();
	int a = 0;

	material->SetData(SHADER_PARAM::INT_3, &a);
	material->SetData(SHADER_PARAM::TEX_0, CResourceManager::GetInst()->FindRes<CTexture>(L"Background").Get());

	current_scene_->AddGameObject(object, 0);

}

void CSceneManager::Progress()
{
	current_scene_->Update();
	current_scene_->LateUpdate();
	current_scene_->FinalUpdate();
	current_scene_->Render();
}
