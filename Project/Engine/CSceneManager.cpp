#include "pch.h"
#include "CSceneManager.h"

#include "CCamera.h"
#include "CGameObject.h"
#include "CMaterial.h"
#include "CCollider2D.h"
#include "CMeshRender.h"
#include "CLight2D.h"
#include "CAnimator2D.h"
#include "CPlayerScript.h"
#include "CMoveScript.h"
#include "CMonsterScript.h"
#include "CMonsterHpBar.h"
#include "CParticleSystem.h"

#include "CResourceManager.h"
#include "CCollisionManager.h"

#include "CTestComputeShader.h"

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
	//카메라
	const auto camera = new CGameObject;
	camera->AddComponent(new CTransform);
	camera->AddComponent(new CCamera);
	camera->Camera()->CheckAllLayout();
	camera->Camera()->SetMainCamera();
	camera->Transform()->SetPosition(Vec3(0.f, 0.f, 0.f));
	camera->Camera()->SetProjectionType(PROJECTION_TYPE::ORTHO);
	//카메라 1번
	current_scene_->AddGameObject(camera, 1, true);

	const auto background = new CGameObject;
	background->AddComponent(new CTransform);
	background->AddComponent(new CMeshRender);
	background->Transform()->SetPosition(Vec3(0.f, 0.f, 2000.f));
	background->Transform()->SetScale(Vec3(1600.f, 900.f, 1.f));
	background->MeshRender()->SetMesh(CResourceManager::GetInst()->FindRes<CMesh>(L"RectMesh"));
	CMaterial* backgroundMaterial = new CMaterial;
	int a = 0;
	backgroundMaterial->SetShader(CResourceManager::GetInst()->FindRes<CGraphicsShader>(L"std2DShader"));
	backgroundMaterial->SetData(SHADER_PARAM::TEX_0, CResourceManager::GetInst()->FindRes<CTexture>(L"background").Get());
	CResourceManager::GetInst()->AddResource(L"backgoundMaterial", backgroundMaterial);
	background->MeshRender()->SetMaterial(backgroundMaterial);
	//boss hp ui
	current_scene_->AddGameObject(background, 0, true);

	const auto light = new CGameObject;
	light->AddComponent(new CTransform);
	light->AddComponent(new CLight2D);
	light->SetName(L"light_test");
	light->Transform()->SetPosition(Vec3(0.f, -200.f, 0.f));
	light->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	light->Light2D()->SetLightRange(300.f);
	light->Light2D()->SetLightColor(Vec4(1.f, 1.f, 1.f, 1.f));

	current_scene_->AddGameObject(light, 0, true);
	//CreatePrefabs();
	//InitTestMap();
	
	CGameObject* particleObject = new CGameObject;

	particleObject->SetName(L"particle");
	particleObject->AddComponent(new CTransform);
	particleObject->AddComponent(new CParticleSystem);
	particleObject->Transform()->SetPosition(Vec3(0.f, 0.f, 1000.f));
	current_scene_->AddGameObject(particleObject, 2, true);

	CGameObject* postEffectObject = new CGameObject;
	postEffectObject->SetName(L"post_effect");
	postEffectObject->AddComponent(new CTransform);
	postEffectObject->AddComponent(new CMeshRender);
	postEffectObject->AddComponent(new CMoveScript);
	postEffectObject->Transform()->SetPosition(Vec3(0.f, 0.f, 400.f));
	postEffectObject->Transform()->SetScale(Vec3(300.f, 300.f, 0.f));
	postEffectObject->MeshRender()->SetMesh(CResourceManager::GetInst()->FindRes<CMesh>(L"RectMesh"));
	postEffectObject->MeshRender()->SetMaterial(CResourceManager::GetInst()->FindRes<CMaterial>(L"post_effect_material"));
	postEffectObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, CResourceManager::GetInst()->FindRes<CTexture>(L"heat_distortion_effet_noise").Get());
	postEffectObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_2, CResourceManager::GetInst()->FindRes<CTexture>(L"radial").Get());

	current_scene_->AddGameObject(postEffectObject, 0, true);

	//플레이어 생성
	const auto player = new CGameObject;
	player->SetName(L"player");
	player->AddComponent(new CTransform);
	player->AddComponent(new CMeshRender);
	player->AddComponent(new CAnimator2D);
	//player->AddComponent(new CPlayerScript);
	player->AddComponent(new CCollider2D);
	player->Transform()->SetPosition(Vec3(0.f, -300.f, 300.f));
	player->Transform()->SetScale(Vec3(100.f, 100.f, 1.f));
	player->Collider2D()->SetOffsetScale(Vec2(0.10f, 0.15f));
	player->MeshRender()->SetMesh(CResourceManager::GetInst()->FindRes<CMesh>(L"RectMesh"));
	player->MeshRender()->SetMaterial(CResourceManager::GetInst()->FindRes<CMaterial>(L"std2DMaterial"));

	Ptr<CMaterial> material = player->MeshRender()->GetMaterial();
	material->SetData(SHADER_PARAM::TEX_0, CResourceManager::GetInst()->FindRes<CTexture>(L"player").Get());
	Ptr<CTexture> playerTex = CResourceManager::GetInst()->LoadRes<CTexture>(L"player_tex", L"texture\\anim_texture\\sakuya_player.png");
	player->Animator2D()->CreateAnimation(L"FLY", playerTex, 0, 0, 32, 46, 4, 0.07f);
	player->Animator2D()->CreateAnimation(L"FLY_LEFT", playerTex, 0, 46, 32, 46, 7, 0.07f);
	player->Animator2D()->Play(L"FLY", 0, true);

	//플레이어 2번
	current_scene_->AddGameObject(player, 2, true);
}

void CSceneManager::Progress()
{
	current_scene_->Update();
	current_scene_->LateUpdate();
	current_scene_->FinalUpdate();
	CCollisionManager::GetInst()->Update();
}

#include "CBulletScript.h"
#include "CPathManager.h"
void CreatePrefabs()
{


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

	
	Ptr<CTexture> bulletTex = CResourceManager::GetInst()->LoadRes<CTexture>(L"player_bullet", L"texture\\player_bullet.png");

	CMaterial* bulletMaterial = new CMaterial;
	bulletMaterial->SetShader(CResourceManager::GetInst()->FindRes<CGraphicsShader>(L"std2DShader"));
	bulletMaterial->SetData(SHADER_PARAM::TEX_0, bulletTex.Get());

	CResourceManager::GetInst()->AddResource(L"bulletMaterial", bulletMaterial);
	playerBullet->MeshRender()->SetMaterial(bulletMaterial);

	playerBullet->ReigsterAsPrefab(L"player_bullet_prefab");
}

void CSceneManager::InitTestMap()
{

	//배경
	const auto background = new CGameObject;
	background->AddComponent(new CTransform);
	background->AddComponent(new CMeshRender);
	background->Transform()->SetPosition(Vec3(0.f, 0.f, 500.f));
	background->Transform()->SetScale(Vec3(1600.f, 900.f, 1.f));
	background->MeshRender()->SetMesh(CResourceManager::GetInst()->FindRes<CMesh>(L"RectMesh"));
	CMaterial* backgroundMaterial = new CMaterial;
	backgroundMaterial->SetShader(CResourceManager::GetInst()->FindRes<CGraphicsShader>(L"std2DMaterial_lights"));
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
	delete hpBarMaterial;
	//4번 UI
	current_scene_->AddGameObject(hpBar, 4, true);





	//플레이어 생성
	const auto player = new CGameObject;
	player->SetName(L"player");
	player->AddComponent(new CTransform);
	player->AddComponent(new CMeshRender);
	player->AddComponent(new CAnimator2D);
	player->AddComponent(new CPlayerScript);
	player->AddComponent(new CCollider2D);
	player->Transform()->SetPosition(Vec3(0.f, -300.f, 300.f));
	player->Transform()->SetScale(Vec3(100.f, 100.f, 1.f));
	player->Collider2D()->SetOffsetScale(Vec2(0.10f, 0.15f)); 
	player->MeshRender()->SetMesh(CResourceManager::GetInst()->FindRes<CMesh>(L"RectMesh"));
	player->MeshRender()->SetMaterial(CResourceManager::GetInst()->FindRes<CMaterial>(L"std2DMaterial"));

	Ptr<CMaterial> material = player->MeshRender()->GetMaterial();
	material->SetData(SHADER_PARAM::TEX_0, CResourceManager::GetInst()->FindRes<CTexture>(L"player").Get());
	Ptr<CTexture> playerTex = CResourceManager::GetInst()->LoadRes<CTexture>(L"player_tex", L"texture\\anim_texture\\sakuya_player.png");
	player->Animator2D()->CreateAnimation(L"FLY", playerTex, 0, 0, 32, 46, 4, 0.07f);
	player->Animator2D()->CreateAnimation(L"FLY_LEFT", playerTex, 0, 46, 32, 46, 7, 0.07f);
	player->Animator2D()->Play(L"FLY", 0, true);

	//플레이어 2번
	current_scene_->AddGameObject(player, 2, true);
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

	CResourceManager::GetInst()->AddResource(L"monster_material", monsterMaterial);
	monster->MeshRender()->SetMaterial(monsterMaterial);

	const auto monsterLeftShooter = monster->Clone();
	monsterLeftShooter->AddComponent(new CMonsterScript(10.f));
	monsterLeftShooter->SetName(L"monster_left_shooter");
	monsterLeftShooter->Transform()->SetPosition(Vec3(-70.f, 300.f, 300.f));
	monsterLeftShooter->MeshRender()->SetMesh(CResourceManager::GetInst()->FindRes<CMesh>(L"RectMesh"));
	monsterLeftShooter->MeshRender()->SetMaterial(CResourceManager::GetInst()->FindRes<CMaterial>(L"collider2DMaterial_none"));

	current_scene_->AddGameObject(monsterLeftShooter, 3, true);

	monster->AddComponent(new CCollider2D);
	monster->Collider2D()->SetOffsetScale(Vec2(0.75f, 0.80f));
	monster->AddComponent(new CMonsterScript);
	//몬스터 3번
	current_scene_->AddGameObject(monster, 3, true);


	CCollisionManager::GetInst()->CheckLayer(2, 3);
}

CGameObject* CSceneManager::FindObjectByName(const wstring& name)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		const auto layer = current_scene_->GetLayer(i);

		const auto& object_vector = layer->GetLayerObjects();

		for (const auto object : object_vector)
		{
			if (object->GetName() == name)
			{
				return object;
			}
		}
	}
	return nullptr;
}

void CSceneManager::FindObjectByName(const wstring& name, vector<CGameObject*>& object_vector)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* layer = current_scene_->GetLayer(i);

		const vector<CGameObject*>& objectVector = layer->GetLayerObjects();

		for (const auto object : objectVector)
		{
			if (object->GetName() == name)
			{
				object_vector.push_back(object);
			}
		}
	}
}
