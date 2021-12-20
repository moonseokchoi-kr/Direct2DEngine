#include "pch.h"
#include "CTestScene.h"

#include<Engine/CScene.h>

#include <Engine/CCamera.h>
#include <Engine/CTransform.h>
#include <Engine/CGameObject.h>
#include <Engine/CMaterial.h>
#include <Engine/CCollider2D.h>
#include <Engine/CMeshRender.h>
#include <Engine/CLight2D.h>
#include <Engine/CAnimator2D.h>


#include <Engine/CParticleSystem.h>
#include <Engine/CTileMap.h>
#include <Engine/CTestComputeShader.h>



#include <Engine/CPathManager.h>
#include <Engine/CCollisionManager.h>
#include <Engine/CSceneManager.h>

#include <Script/CMonsterHpBar.h>
#include <Script/CBulletScript.h>
#include <Script/CPlayerScript.h>
#include <Script/CMoveScript.h>
#include <Script/CMonsterScript.h>





void CTestScene::CreateTestScene()
{
	CScene* scene = new CScene;

	CCollisionManager::GetInst()->CheckLayer(2, 3);

	CSceneManager::GetInst()->ChangeScene(scene);

	return;

	CreatePrefab();

	//카메라
	const auto camera = new CGameObject;
	camera->SetName(L"camera");
	camera->AddComponent(new CTransform);
	camera->AddComponent(new CCamera);
	camera->Camera()->CheckAllLayout();
	camera->Camera()->SetMainCamera();
	camera->Transform()->SetPosition(Vec3(0.f, 0.f, 0.f));
	camera->Camera()->SetProjectionType(PROJECTION_TYPE::ORTHO);
	//카메라 1번
	scene->AddGameObject(camera, 1, true);

	const auto background = new CGameObject;
	background->SetName(L"parent");
	background->AddComponent(new CTransform);
	background->AddComponent(new CMeshRender);
	background->Transform()->SetPosition(Vec3(0.f, 0.f, 2000.f));
	background->Transform()->SetScale(Vec3(1600.f, 900.f, 1.f));
	background->MeshRender()->SetMesh(CResourceManager::GetInst()->FindRes<CMesh>(L"RectMesh"));
	
	Ptr<CMaterial> backgroundMaterial = CResourceManager::GetInst()->CreateMaterial(L"backgound_material", CResourceManager::GetInst()->FindRes<CGraphicsShader>(L"std2DShader"));
	int a = 0;
	backgroundMaterial->SetData(SHADER_PARAM::TEX_0, CResourceManager::GetInst()->FindRes<CTexture>(L"background").Get());
	background->MeshRender()->SetMaterial(backgroundMaterial);

	const auto backgroundChild = new CGameObject;
	backgroundChild->SetName(L"child");
	backgroundChild->AddComponent(new CTransform);
	backgroundChild->AddComponent(new CMeshRender);
	backgroundChild->Transform()->SetPosition(Vec3(1.2f, 0.f, 0.f));
	backgroundChild->Transform()->SetScale(Vec3(0.5f, 0.5f, 1.f));
	backgroundChild->MeshRender()->SetMesh(CResourceManager::GetInst()->FindRes<CMesh>(L"RectMesh"));
	backgroundChild->MeshRender()->SetMaterial(backgroundMaterial);

	background->AddChild(backgroundChild);
	//boss hp ui
	scene->AddGameObject(background, 0, true);




	// 	const auto tileMap = new CGameObject;
	// 	tileMap->SetName(L"tile_map");
	// 	tileMap->AddComponent(new CTransform);
	// 	tileMap->AddComponent(new CTileMap);
	// 	tileMap->Transform()->SetPosition(Vec3(0.f, 0.f, 200.f));
	// 	tileMap->Transform()->SetScale(Vec3(64.f * 8.f, 64.f * 6, 1.f));
	// 	Ptr<CTexture> pTileAtlas = CResourceManager::GetInst()->LoadRes<CTexture>(L"tile_map_atlas_texture", L"texture\\tilemap\\TILE.bmp");
	// 	tileMap->TileMap()->SetAtlasTexture(pTileAtlas);
	// 	scene->AddGameObject(tileMap, 3, true);

	const auto light = new CGameObject;
	light->AddComponent(new CTransform);
	light->AddComponent(new CLight2D);
	light->SetName(L"light_test");
	light->Transform()->SetPosition(Vec3(0.f, -200.f, 0.f));
	light->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	light->Light2D()->SetLightRange(300.f);
	light->Light2D()->SetLightColor(Vec4(1.f, 1.f, 1.f, 1.f));

	scene->AddGameObject(light, 0, true);
	//CreatePrefabs();
	//InitTestMap();

// 	CGameObject* particleObject = new CGameObject;
// 
// 	particleObject->SetName(L"particle");
// 	particleObject->AddComponent(new CTransform);
// 	particleObject->AddComponent(new CParticleSystem);
// 	particleObject->Transform()->SetPosition(Vec3(0.f, 0.f, 1000.f));
// 	scene->AddGameObject(particleObject, 2, true);
// 
// 	CGameObject* postEffectObject = new CGameObject;
// 	postEffectObject->SetName(L"post_effect");
// 	postEffectObject->AddComponent(new CTransform);
// 	postEffectObject->AddComponent(new CMeshRender);
// 	postEffectObject->AddComponent(new CMoveScript);
// 	postEffectObject->Transform()->SetPosition(Vec3(0.f, 0.f, 400.f));
// 	postEffectObject->Transform()->SetScale(Vec3(300.f, 300.f, 0.f));
// 	postEffectObject->MeshRender()->SetMesh(CResourceManager::GetInst()->FindRes<CMesh>(L"RectMesh"));
// 	postEffectObject->MeshRender()->SetMaterial(CResourceManager::GetInst()->FindRes<CMaterial>(L"post_effect_material"));
// 	postEffectObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, CResourceManager::GetInst()->FindRes<CTexture>(L"heat_distortion_effet_noise").Get());
// 	postEffectObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_2, CResourceManager::GetInst()->FindRes<CTexture>(L"radial").Get());
// 
// 	scene->AddGameObject(postEffectObject, 0, true);

	//플레이어 생성
	const auto player = new CGameObject;
	player->SetName(L"player");
	player->AddComponent(new CTransform);
	player->AddComponent(new CMeshRender);
	player->AddComponent(new CAnimator2D);
	player->AddComponent(new CPlayerScript);
	player->AddComponent(new CCollider2D);
	player->Transform()->SetPosition(Vec3(0.f, -300.f, 100.f));
	player->Transform()->SetScale(Vec3(100.f, 100.f, 1.f));
	player->Collider2D()->SetOffsetScale(Vec2(0.10f, 0.15f));
	player->MeshRender()->SetMesh(CResourceManager::GetInst()->FindRes<CMesh>(L"RectMesh"));
	player->MeshRender()->SetMaterial(CResourceManager::GetInst()->FindRes<CMaterial>(L"std2DMaterial"));

	Ptr<CMaterial> material = player->MeshRender()->GetCurrentMaterial();
	material->SetData(SHADER_PARAM::TEX_0, CResourceManager::GetInst()->FindRes<CTexture>(L"player").Get());
	Ptr<CTexture> playerTex = CResourceManager::GetInst()->LoadRes<CTexture>(L"player_tex", L"texture\\anim_texture\\sakuya_player.png");
	player->Animator2D()->CreateAnimation(L"FLY", playerTex, 0, 0, 32, 48, 4, 0.07f);
	player->Animator2D()->CreateAnimation(L"FLY_LEFT", playerTex, 0, 48, 32, 48, 7, 0.07f);
	player->Animator2D()->Play(L"FLY_LEFT", 0, true);

	//플레이어 2번
	scene->AddGameObject(player, 2, true);


}


void CTestScene::CreatePrefab()
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

	Ptr<CMaterial> bulletMaterial = CResourceManager::GetInst()->CreateMaterial(L"bulletMaterial", CResourceManager::GetInst()->FindRes<CGraphicsShader>(L"std2DShader"));
	bulletMaterial->SetData(SHADER_PARAM::TEX_0, bulletTex.Get());

	playerBullet->MeshRender()->SetMaterial(bulletMaterial);

	playerBullet->ReigsterAsPrefab(L"player_bullet_prefab");
}

void CTestScene::InitTestMap(CScene* scene)
{
	//배경
	const auto background = new CGameObject;
	background->AddComponent(new CTransform);
	background->AddComponent(new CMeshRender);
	background->Transform()->SetPosition(Vec3(0.f, 0.f, 500.f));
	background->Transform()->SetScale(Vec3(1600.f, 900.f, 1.f));
	background->MeshRender()->SetMesh(CResourceManager::GetInst()->FindRes<CMesh>(L"RectMesh"));
	Ptr<CMaterial> backgroundMaterial = CResourceManager::GetInst()->CreateMaterial(L"backgound_material", CResourceManager::GetInst()->FindRes<CGraphicsShader>(L"std2DShader"));
	int a = 0;
	backgroundMaterial->SetData(SHADER_PARAM::TEX_0, CResourceManager::GetInst()->FindRes<CTexture>(L"background").Get());
	background->MeshRender()->SetMaterial(backgroundMaterial);
	//boss hp ui
	scene->AddGameObject(background, 0, true);


	const auto hpBar = new CGameObject;
	hpBar->AddComponent(new CTransform);
	hpBar->AddComponent(new CMeshRender);
	hpBar->AddComponent(new CMonsterHpBar);
	hpBar->SetName(L"hp_ui");
	hpBar->Transform()->SetPosition(Vec3(0.f, 400.f, 300.f));
	hpBar->Transform()->SetScale(Vec3(1000.f, 10.f, 1.f));
	hpBar->MeshRender()->SetMesh(CResourceManager::GetInst()->FindRes<CMesh>(L"RectMesh"));
	Ptr<CMaterial> hpBarMaterial = CResourceManager::GetInst()->CreateMaterial(L"monster_hp_material", CResourceManager::GetInst()->FindRes<CGraphicsShader>(L"monster_hp_shader"));
	hpBar->MeshRender()->SetMaterial(hpBarMaterial);

	//4번 UI
	scene->AddGameObject(hpBar, 4, true);





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

	Ptr<CMaterial> material = player->MeshRender()->GetCurrentMaterial();
	material->SetData(SHADER_PARAM::TEX_0, CResourceManager::GetInst()->FindRes<CTexture>(L"player").Get());
	Ptr<CTexture> playerTex = CResourceManager::GetInst()->LoadRes<CTexture>(L"player_tex", L"texture\\anim_texture\\sakuya_player.png");
	player->Animator2D()->CreateAnimation(L"FLY", playerTex, 0, 0, 32, 46, 4, 0.07f);
	player->Animator2D()->CreateAnimation(L"FLY_LEFT", playerTex, 0, 46, 32, 46, 7, 0.07f);
	player->Animator2D()->Play(L"FLY", 0, true);

	//플레이어 2번
	scene->AddGameObject(player, 2, true);
	//몬스터 생성
	const auto monster = new CGameObject;
	monster->AddComponent(new CTransform);
	monster->AddComponent(new CMeshRender);

	monster->SetName(L"monster");
	monster->Transform()->SetPosition(Vec3(0, 300.f, 300.f));
	monster->Transform()->SetScale(Vec3(100.f, 150.f, 1.f));

	monster->MeshRender()->SetMesh(CResourceManager::GetInst()->FindRes<CMesh>(L"RectMesh"));
	Ptr<CMaterial> monsterMaterial = CResourceManager::GetInst()->CreateMaterial(L"monster_material", CResourceManager::GetInst()->FindRes<CGraphicsShader>(L"std2DShader"));
	monsterMaterial->SetData(SHADER_PARAM::TEX_0, CResourceManager::GetInst()->FindRes<CTexture>(L"monster").Get());
	monster->MeshRender()->SetMaterial(monsterMaterial);

	const auto monsterLeftShooter = monster->Clone();
	monsterLeftShooter->AddComponent(new CMonsterScript(10.f));
	monsterLeftShooter->SetName(L"monster_left_shooter");
	monsterLeftShooter->Transform()->SetPosition(Vec3(-70.f, 300.f, 300.f));
	monsterLeftShooter->MeshRender()->SetMesh(CResourceManager::GetInst()->FindRes<CMesh>(L"RectMesh"));
	monsterLeftShooter->MeshRender()->SetMaterial(CResourceManager::GetInst()->FindRes<CMaterial>(L"collider2DMaterial_none"));

	scene->AddGameObject(monsterLeftShooter, 3, true);

	monster->AddComponent(new CCollider2D);
	monster->Collider2D()->SetOffsetScale(Vec2(0.75f, 0.80f));
	monster->AddComponent(new CMonsterScript);
	//몬스터 3번
	scene->AddGameObject(monster, 3, true);

}