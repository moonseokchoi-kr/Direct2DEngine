#include "pch.h"
#include "CSceneManager.h"

#include "CCamera.h"
#include "CGameObject.h"
#include "CMaterial.h"
#include "CCollider2D.h"
#include "CMeshRender.h"

#include "CPlayerScript.h"
#include "CMonsterScript.h"

#include "CResourceManager.h"
#include "CCollisionManager.h"
#include "CScene.h"
#include "CLayer.h"
#include "CTransform.h"

void CreatePrefabs();

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

	CreatePrefabs();

	const auto background = new CGameObject;
	background->AddComponent(new CTransform);
	background->AddComponent(new CMeshRender);
	background->Transform()->SetPosition(Vec3(0.f, 0.f, 500.f));
	background->Transform()->SetScale(Vec3(10000.f, 10000.f, 1.f));
	background->MeshRender()->SetMesh(CResourceManager::GetInst()->FindRes<CMesh>(L"RectMesh"));
	CMaterial* backgroundMaterial = new CMaterial;
	backgroundMaterial->SetShader(CResourceManager::GetInst()->FindRes<CGraphicsShader>(L"std2DShader"));
	backgroundMaterial->SetData(SHADER_PARAM::TEX_0, CResourceManager::GetInst()->FindRes<CTexture>(L"background").Get());

	CResourceManager::GetInst()->AddResource(L"backgoundMaterial", backgroundMaterial);
	background->MeshRender()->SetMaterial(backgroundMaterial);
	//0�� ���
	current_scene_->AddGameObject(background,0,true);

	const auto camera = new CGameObject;
	camera->AddComponent(new CTransform);
	camera->AddComponent(new CCamera);
	camera->Transform()->SetPosition(Vec3(0.f, 0.f, 0.f));
	//ī�޶� 1��
	current_scene_->AddGameObject(camera, 1,true);

	const auto object = new CGameObject;
	object->SetName(L"player");
	object->AddComponent(new CTransform);
	object->AddComponent(new CMeshRender);
	object->AddComponent(new CPlayerScript);
	object->AddComponent(new CCollider2D);
	object->Transform()->SetPosition(Vec3(0.f, 0.f, 300.f));
	object->Transform()->SetScale(Vec3(100.f, 100.f, 1.f));
	object->Collider2D()->SetOffsetScale(Vec2(0.20f, 0.30f));
	object->MeshRender()->SetMesh(CResourceManager::GetInst()->FindRes<CMesh>(L"RectMesh"));
	object->MeshRender()->SetMaterial(CResourceManager::GetInst()->FindRes<CMaterial>(L"std2DMaterial"));

	Ptr<CMaterial> material = object->MeshRender()->GetMaterial();
	material->SetData(SHADER_PARAM::TEX_0, CResourceManager::GetInst()->FindRes<CTexture>(L"player").Get());
	//�÷��̾� 2��
	current_scene_->AddGameObject(object, 2,true);

	const auto monster = new CGameObject;
	monster->AddComponent(new CTransform);
	monster->AddComponent(new CMeshRender);
	monster->AddComponent(new CCollider2D);
	monster->AddComponent(new CMonsterScript);
	monster->SetName(L"monster");
	monster->Transform()->SetPosition(Vec3(0, 200.f, 300.f));
	monster->Transform()->SetScale(Vec3(100.f, 150.f, 1.f));
	monster->Collider2D()->SetOffsetScale(Vec2(0.75f, 0.80f));
	monster->MeshRender()->SetMesh(CResourceManager::GetInst()->FindRes<CMesh>(L"RectMesh"));
	CMaterial* monsterMaterial = new CMaterial;
	monsterMaterial->SetShader(CResourceManager::GetInst()->FindRes<CGraphicsShader>(L"std2DShader"));
	monsterMaterial->SetData(SHADER_PARAM::TEX_0, CResourceManager::GetInst()->FindRes<CTexture>(L"monster").Get());

	CResourceManager::GetInst()->AddResource(L"monsterMaterial", backgroundMaterial);
	monster->MeshRender()->SetMaterial(monsterMaterial);

	//���� 3��
	current_scene_->AddGameObject(monster, 3, true);

	CCollisionManager::GetInst()->CheckLayer(2, 3);

}

void CSceneManager::Progress()
{
	current_scene_->Update();
	current_scene_->LateUpdate();
	current_scene_->FinalUpdate();
	CCollisionManager::GetInst()->Update();
	current_scene_->Render();
}

#include "CBulletScript.h"
#include "CPathManager.h"
void CreatePrefabs()
{
	//Bullet

	CGameObject* playerBullet = new CGameObject;
	playerBullet->SetName(L"player_bullet");
	playerBullet->AddComponent(new CTransform);
	playerBullet->AddComponent(new CMeshRender);
	playerBullet->AddComponent(new CCollider2D);
	playerBullet->AddComponent(new CBulletScript);

	playerBullet->Transform()->SetScale(Vec3(25.f, 50.f, 1.f));
	playerBullet->Transform()->SetRotation(Vec3(0.f, 0.f, 0.f));

	playerBullet->Collider2D()->SetOffsetScale(Vec2(1.f, 1.f));

	playerBullet->MeshRender()->SetMesh(CResourceManager::GetInst()->FindRes<CMesh>(L"RectMesh"));

	Ptr<CTexture> bulletTex = new CTexture;
	wstring strPath = CPathManager::GetInst()->GetContentPath();
	strPath += L"texture\\player_bullet.png";
	bulletTex->Load(strPath);
	CResourceManager::GetInst()->AddResource(L"player_bullet", bulletTex.Get());


	CMaterial* bulletMaterial = new CMaterial;
	bulletMaterial->SetShader(CResourceManager::GetInst()->FindRes<CGraphicsShader>(L"std2DShader"));
	bulletMaterial->SetData(SHADER_PARAM::TEX_0, bulletTex.Get());
	CResourceManager::GetInst()->AddResource(L"bulletMaterial", bulletMaterial);
	playerBullet->MeshRender()->SetMaterial(bulletMaterial);

	playerBullet->ReigsterAsPrefab(L"player_bullet_prefab");
}