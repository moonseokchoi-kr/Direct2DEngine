#include "pch.h"
#include "CSceneManager.h"

#include "CCamera.h"
#include "CGameObject.h"
#include "CMaterial.h"
#include "CCollider2D.h"
#include "CMeshRender.h"

#include "CPlayerScript.h"
#include "CMonsterScript.h"
#include "CMonsterHpBar.h"

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
	//배경
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
	//boss hp ui
	current_scene_->AddGameObject(background, 0, true);


	const auto hpBar = new CGameObject;
	hpBar->AddComponent(new CTransform);
	hpBar->AddComponent(new CMeshRender);
	hpBar->AddComponent(new CMonsterHpBar);
	hpBar->SetName(L"hp_ui");
	hpBar->Transform()->SetPosition(Vec3(0.f, 400.f, 300.f));
	hpBar->Transform()->SetScale(Vec3(1000.f, 10.f, 1.f));
	hpBar->MeshRender()->SetMesh(CResourceManager::GetInst()->FindRes<CMesh>(L"RectMesh"));
	CMaterial* hpBarMaterial = new CMaterial;
	hpBarMaterial->SetShader(CResourceManager::GetInst()->FindRes<CGraphicsShader>(L"monster_hp_shader"));
	CResourceManager::GetInst()->AddResource(L"monster_hp_material", hpBarMaterial);
	hpBar->MeshRender()->SetMaterial(CResourceManager::GetInst()->FindRes<CMaterial>(L"monster_hp_material"));

	//4번 UI
	current_scene_->AddGameObject(hpBar, 4, true);


	//카메라
	const auto camera = new CGameObject;
	camera->AddComponent(new CTransform);
	camera->AddComponent(new CCamera);
	camera->Transform()->SetPosition(Vec3(0.f, 0.f, 0.f));
	//카메라 1번
	current_scene_->AddGameObject(camera, 1,true);
	//플레이어 생성
	const auto object = new CGameObject;
	object->SetName(L"player");
	object->AddComponent(new CTransform);
	object->AddComponent(new CMeshRender);
	object->AddComponent(new CPlayerScript);
	object->AddComponent(new CCollider2D);
	object->Transform()->SetPosition(Vec3(0.f, -300.f, 300.f));
	object->Transform()->SetScale(Vec3(100.f, 100.f, 1.f));
	object->Collider2D()->SetOffsetScale(Vec2(0.20f, 0.30f));
	object->MeshRender()->SetMesh(CResourceManager::GetInst()->FindRes<CMesh>(L"RectMesh"));
	object->MeshRender()->SetMaterial(CResourceManager::GetInst()->FindRes<CMaterial>(L"std2DMaterial"));

	Ptr<CMaterial> material = object->MeshRender()->GetMaterial();
	material->SetData(SHADER_PARAM::TEX_0, CResourceManager::GetInst()->FindRes<CTexture>(L"player").Get());
	//플레이어 2번
	current_scene_->AddGameObject(object, 2,true);
	//몬스터 생성
	const auto monster = new CGameObject;
	monster->AddComponent(new CTransform);
	monster->AddComponent(new CMeshRender);

	monster->SetName(L"monster");
	monster->Transform()->SetPosition(Vec3(0, 300.f, 300.f));
	monster->Transform()->SetScale(Vec3(100.f, 150.f, 1.f));

	monster->MeshRender()->SetMesh(CResourceManager::GetInst()->FindRes<CMesh>(L"RectMesh"));
	CMaterial* monsterMaterial = new CMaterial;
	monsterMaterial->SetShader(CResourceManager::GetInst()->FindRes<CGraphicsShader>(L"std2DShader"));
	monsterMaterial->SetData(SHADER_PARAM::TEX_0, CResourceManager::GetInst()->FindRes<CTexture>(L"monster").Get());

	CResourceManager::GetInst()->AddResource(L"monsterMaterial", backgroundMaterial);
	monster->MeshRender()->SetMaterial(monsterMaterial);

	const auto monsterLeftShooter = monster->Clone();
	monsterLeftShooter->AddComponent(new CMonsterScript(0.3f));
	monsterLeftShooter->SetName(L"monster_left_shooter");
	monsterLeftShooter->Transform()->SetPosition(Vec3(-70.f, 300.f, 300.f));
	monsterLeftShooter->MeshRender()->SetMesh(CResourceManager::GetInst()->FindRes<CMesh>(L"RectMesh"));
	monsterLeftShooter->MeshRender()->SetMaterial(CResourceManager::GetInst()->FindRes<CMaterial>(L"collider2DMaterial_none"));

	const auto monsterRightShooter = monsterLeftShooter->Clone();
	monsterRightShooter->Transform()->SetPosition(Vec3(70.f, 300.f, 300.f));
	monsterRightShooter->SetName(L"monster_right_shooter");

	monster->AddComponent(new CCollider2D);
	monster->Collider2D()->SetOffsetScale(Vec2(0.75f, 0.80f));
	monster->AddComponent(new CMonsterScript);
	//몬스터 3번
	current_scene_->AddGameObject(monster, 3, true);
	current_scene_->AddGameObject(monsterLeftShooter, 3, true);
	current_scene_->AddGameObject(monsterRightShooter, 3, true);

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