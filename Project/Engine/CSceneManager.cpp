#include "pch.h"
#include "CSceneManager.h"

#include "CCamera.h"
#include "CGameObject.h"
#include "CMeshRender.h"
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
	object->Transform()->SetPos(Vec3(0.f, 0.f, 0.f));
	object->MeshRender()->SetMesh(CResourceManager::GetInst()->FindRes<CMesh>(L"RectMesh"));
	object->MeshRender()->SetTexture(CResourceManager::GetInst()->FindRes<CTexture>(L"Background"));
	object->MeshRender()->SetShader(CResourceManager::GetInst()->FindRes<CShader>(L"std2DShader"));

	current_scene_->AddGameObject(object, 0);

}

void CSceneManager::Progress()
{
	current_scene_->Update();
	current_scene_->LateUpdate();
	current_scene_->FinalUpdate();
	current_scene_->Render();
}
