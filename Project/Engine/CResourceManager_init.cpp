#include "pch.h"

#include "CMaterial.h"
#include "CResourceManager.h"
#include "CMesh.h"
#include "CGraphicsShader.h"
#include "CTexture.h"
#include "CPrefab.h"
#include "CTestComputeShader.h"
#include "CParticleUpdateShader.h"
#include "CPathManager.h"

void CResourceManager::Init()
{
	CreateDefaultMesh();
	CreateDefaultShader();
	CreateDefaultTexture();
	CreateDefaultMaterial();
	CreateDefaultComputeShader();
}

void CResourceManager::CreateDefaultMesh()
{
	CMesh* mesh = nullptr;
	vector<VTX> vertexBuffer;
	vector<UINT> indexBuffer;

	VTX vertex;
	//
	// =========
	// Point Mesh
	// =========
	// 

	vertex.pos = Vec3(0.f, 0.f, 0.f);
	vertex.color = Vec4(1.f, 1.f, 1.f, 1.f);
	vertex.uv = Vec2(0.f, 0.f);
	vertexBuffer.push_back(vertex);
	indexBuffer.push_back(0);

	mesh = new CMesh;
	mesh->Create(vertexBuffer.data(), (UINT)vertexBuffer.size(), indexBuffer.data(), (UINT)indexBuffer.size());
	AddResource(L"PointMesh", mesh);
	
	vertexBuffer.clear();
	indexBuffer.clear();


	//
	// =========
	// Rect Mesh
	// =========
	// 
	vertex.pos = Vec3(-0.5f, 0.5f, 0);
	vertex.color = Vec4(1.f, 0.f, 0.f, 1.f);
	vertex.uv = Vec2(0, 0);
	vertexBuffer.push_back(vertex);

	vertex.pos = Vec3(0.5f, 0.5f, 0);
	vertex.color = Vec4(0.f, 1.f, 0.f, 1.f);
	vertex.uv = Vec2(1, 0);
	vertexBuffer.push_back(vertex);

	vertex.pos = Vec3(0.5f, -0.5f, 0);
	vertex.color = Vec4(0.f, 0.f, 1.f, 1.f);
	vertex.uv = Vec2(1, 1);
	vertexBuffer.push_back(vertex);

	vertex.pos = Vec3(-0.5f, -0.5f, 0);
	vertex.color = Vec4(1.f, 0.f, 1.f, 1.f);
	vertex.uv = Vec2(0, 1);
	vertexBuffer.push_back(vertex);

	indexBuffer.push_back(0);
	indexBuffer.push_back(1);
	indexBuffer.push_back(2);
	indexBuffer.push_back(2);
	indexBuffer.push_back(3);
	indexBuffer.push_back(0);

	mesh = new CMesh;
	mesh->Create(vertexBuffer.data(), (UINT)vertexBuffer.size(), indexBuffer.data(), (UINT)indexBuffer.size());
	AddResource(L"RectMesh", mesh);

	vertexBuffer.clear();
	indexBuffer.clear();


	//
	// ===========
	// Circle Mesh
	// ===========
	//
	//원의 중점
	vertex.pos = Vec3(0.f, 0.f, 0.f);
	vertex.uv = Vec2(0.5f, 0.5f);
	vertex.color = Vec4(1.0f, 0.f, 1.0f,1.0f);

	vertexBuffer.push_back(vertex);

	//원의 테두리 정점
	UINT sliceCount = 80;
	float radius = 0.5f;
	float angle = XM_2PI / (float)sliceCount;

	float theta = 0;
	for (UINT i = 0; i < sliceCount+1; ++i)
	{
		vertex.pos = Vec3(radius * cosf(theta), radius * sinf(theta), 0.f);
		vertex.uv = Vec2(vertex.pos.x / (2.f * radius) + 0.5f, 0.5f - vertex.pos.y / (2.f * radius));
		vertex.color = Vec4(1.0f, 0.f, 1.0f, 1.0f);

		vertexBuffer.push_back(vertex);

		theta += angle;
	}

	for (UINT i = 0; i < sliceCount; ++i)
	{
		indexBuffer.push_back(0);
		indexBuffer.push_back(i + 2);
		indexBuffer.push_back(i + 1);
	}

	mesh = new CMesh;
	mesh->Create(vertexBuffer.data(), (UINT)vertexBuffer.size(), indexBuffer.data(), (UINT)indexBuffer.size());
	AddResource(L"CircleMesh", mesh);
	
	vertexBuffer.clear();
	indexBuffer.clear();

}

void CResourceManager::CreateDefaultShader()
{
	CGraphicsShader* stdShader = nullptr;

	//std2DShader
	stdShader = new CGraphicsShader;
	wstring strPath = CPathManager::GetInst()->GetContentPath();
	strPath += L"shader\\std2d.fx";
	stdShader->CreateVertexShader(strPath, "vs_main");
	stdShader->CreatePixelShader(strPath, "ps_main");
	stdShader->SetBlendType(BLEND_TYPE::ALPHA_BLEND);
	stdShader->SetRasterizerType(RASTERIZER_TYPE::CULL_NONE);
	stdShader->SetRenderTiming(RENDER_TIMING::FOWARD);
	stdShader->AddShaderParamInfo(ShaderParameter{ SHADER_PARAM::TEX_0, L"output_texture" });
	stdShader->AddShaderParamInfo(ShaderParameter{ SHADER_PARAM::INT_0, L"test_int" });
	stdShader->AddShaderParamInfo(ShaderParameter{ SHADER_PARAM::FLOAT_0, L"test_float" });
	stdShader->AddShaderParamInfo(ShaderParameter{ SHADER_PARAM::VEC2_0, L"test_vec2" });
	stdShader->AddShaderParamInfo(ShaderParameter{ SHADER_PARAM::VEC4_0, L"test_vec4" });
	
	
	AddResource(L"std2DShader", stdShader);

	//Ptr<CGraphicsShader> ptr = LoadGraphicShader(L"std2DShader", L"shader\\std2d.fx", BLEND_TYPE::ALPHA_BLEND, DEPTH_STENCIL_TYPE::LESS, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//colliderShader
	stdShader = new CGraphicsShader;
	strPath = CPathManager::GetInst()->GetContentPath();
	strPath += L"shader\\collider2dshader.fx";
	stdShader->CreateVertexShader(strPath, "vs_main");
	stdShader->CreatePixelShader(strPath, "ps_main");

	stdShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	stdShader->SetDepthStencilType(DEPTH_STENCIL_TYPE::NO_TEST_NO_WRITE);
	stdShader->SetRenderTiming(RENDER_TIMING::FOWARD);


	AddResource(L"collder2DShader", stdShader);

	//HP bar
	stdShader = new CGraphicsShader;
	strPath = CPathManager::GetInst()->GetContentPath();
	strPath += L"shader\\monsterhpshader.fx";
	stdShader->CreateVertexShader(strPath, "vs_main");
	stdShader->CreatePixelShader(strPath, "ps_main");
	stdShader->SetRenderTiming(RENDER_TIMING::FOWARD);

	stdShader->SetRasterizerType(RASTERIZER_TYPE::CULL_NONE);
	stdShader->SetBlendType(BLEND_TYPE::ALPHA_BLEND);

	AddResource(L"monster_hp_shader", stdShader);

	//light shader
	stdShader = new CGraphicsShader;
	strPath = CPathManager::GetInst()->GetContentPath();
	strPath += L"shader\\light2dshader.fx";
	stdShader->CreateVertexShader(strPath, "vs_main");
	stdShader->CreatePixelShader(strPath, "ps_main");
	stdShader->SetBlendType(BLEND_TYPE::ALPHA_BLEND);
	stdShader->SetRasterizerType(RASTERIZER_TYPE::CULL_NONE);
	stdShader->SetRenderTiming(RENDER_TIMING::FOWARD);
	stdShader->AddShaderParamInfo(ShaderParameter{ SHADER_PARAM::TEX_0, L"output_texture" });

	AddResource(L"light2D_shader", stdShader);

	stdShader = new CGraphicsShader;
	strPath = CPathManager::GetInst()->GetContentPath();
	strPath += L"shader\\particle_shader.fx";

	stdShader->CreateVertexShader(strPath, "vs_main");
	stdShader->CreatePixelShader(strPath, "ps_main");
	stdShader->CreateGeometryShader(strPath, "gs_main");

	stdShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	stdShader->SetBlendType(BLEND_TYPE::ALPHA_BLEND);
	stdShader->SetDepthStencilType(DEPTH_STENCIL_TYPE::NO_WRITE);
	stdShader->SetRenderTiming(RENDER_TIMING::PARTICLE);
	AddResource(L"particle_shader", stdShader);

	stdShader = new CGraphicsShader;
	strPath = CPathManager::GetInst()->GetContentPath();
	strPath += L"shader\\tile_map_shader.fx";
	stdShader->CreateVertexShader(strPath, "vs_main");
	stdShader->CreatePixelShader(strPath, "ps_main");
	
	stdShader->SetBlendType(BLEND_TYPE::ALPHA_BLEND);
	stdShader->SetRasterizerType(RASTERIZER_TYPE::CULL_NONE);
	stdShader->SetRenderTiming(RENDER_TIMING::FOWARD);
	AddResource(L"tile_map_shader", stdShader);

	stdShader = new CGraphicsShader;
	strPath = CPathManager::GetInst()->GetContentPath();
	strPath += L"shader\\post_effect.fx";

	stdShader->CreateVertexShader(strPath, "vs_main");
	stdShader->CreatePixelShader(strPath, "ps_main");

	stdShader->SetBlendType(BLEND_TYPE::ALPHA_BLEND);
	stdShader->SetRasterizerType(RASTERIZER_TYPE::CULL_NONE);
	stdShader->SetDepthStencilType(DEPTH_STENCIL_TYPE::NO_TEST_NO_WRITE);
	stdShader->SetRenderTiming(RENDER_TIMING::POST_EFFECT);
	AddResource(L"post_effect_shader", stdShader);

	//UI Shader

	stdShader = new CGraphicsShader;
	strPath = CPathManager::GetInst()->GetContentPath();
	strPath += L"shader\\ui_shader.fx";
	stdShader->CreateVertexShader(strPath, "vs_main");
	stdShader->CreatePixelShader(strPath, "ps_main");

	stdShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	stdShader->SetDepthStencilType(DEPTH_STENCIL_TYPE::NO_TEST_NO_WRITE);
	stdShader->SetRenderTiming(RENDER_TIMING::FOWARD);


	AddResource(L"ui_shader",stdShader);


	stdShader = new CGraphicsShader;
	strPath = CPathManager::GetInst()->GetContentPath();
	strPath += L"shader\\box2d_collider_shader.fx";
	stdShader->CreateVertexShader(strPath, "vs_main");
	stdShader->CreatePixelShader(strPath, "ps_main");

	stdShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	stdShader->SetDepthStencilType(DEPTH_STENCIL_TYPE::NO_TEST_NO_WRITE);
	stdShader->SetRenderTiming(RENDER_TIMING::FOWARD);


	AddResource(L"box2d_collider_debug", stdShader);
}

void CResourceManager::CreateDefaultTexture()
{
	LoadRes<CTexture>(L"background", L"texture\\background.png");
	LoadRes<CTexture>(L"player", L"texture\\sakuya_player.png");
	LoadRes<CTexture>(L"monster", L"texture\\monster.png");
	LoadRes<CTexture>(L"monster_bullet_red", L"texture\\monster_bullet_red.png");
	LoadRes<CTexture>(L"monster_bullet_blue", L"texture\\monster_bullet_blue.png");
	LoadRes<CTexture>(L"particle_01", L"texture\\particle\\AlphaCircle.png");
	LoadRes<CTexture>(L"heat_distortion_effet_noise", L"texture\\posteffect\\noise.png");
	LoadRes<CTexture>(L"radial", L"texture\\posteffect\\radial.png");
	LoadRes<CTexture>(L"village_tile_map", L"texture\\tilemap\\village.png");
	LoadRes<CTexture>(L"tile_block", L"texture\\tilemap\\TILE.bmp");

	wstring contentPath = CPathManager::GetInst()->GetContentPath();
	CTexture* noise = new CTexture;
	noise->Load(contentPath + L"texture\\global\\noise_01.png");
	g_global.noise_texture_resolution = Vec2((float)noise->GetWidth(), (float)noise->GetHeight());
	default_tex_map_.insert(make_pair(L"noise_01", noise));

	noise->SetPipelineStage(PIPELINE_STAGE::PS_ALL, 13);
	noise->UpdateData();
}

void CResourceManager::CreateDefaultMaterial()
{
	CMaterial* material = nullptr;
	int a = 1;
	material = new CMaterial(true);
	material->SetShader(CResourceManager::GetInst()->FindRes<CGraphicsShader>(L"std2DShader"));
	material->SetData(SHADER_PARAM::INT_0, &a);
	AddResource(L"std2DMaterial", material);


	material = new CMaterial(true);
	material->SetShader(CResourceManager::GetInst()->FindRes<CGraphicsShader>(L"light2D_shader"));
	AddResource(L"std2DMaterial_lights", material);

	//colliderMaterial

	material = new CMaterial(true);
	material->SetShader(CResourceManager::GetInst()->FindRes<CGraphicsShader>(L"collder2DShader"));
	AddResource(L"collider2DMaterial_none", material);

	material = new CMaterial(true);
	material->SetShader(CResourceManager::GetInst()->FindRes<CGraphicsShader>(L"collder2DShader"));
	
	material->SetData(SHADER_PARAM::INT_0, &a);
	AddResource(L"collider2DMaterial_collision",material);

	//hp material
	material = new CMaterial(true);
	material->SetShader(CResourceManager::GetInst()->FindRes<CGraphicsShader>(L"monster_hp_shader"));
	AddResource(L"monster_hp_material", material);

	//ui material
	material = new CMaterial(true);
	material->SetShader(CResourceManager::GetInst()->FindRes<CGraphicsShader>(L"ui_shader"));
	AddResource(L"ui_material", material);

	//bullet material
	material = new CMaterial(true);
	material->SetShader(CResourceManager::GetInst()->FindRes<CGraphicsShader>(L"std2DShader"));
	material->SetData(SHADER_PARAM::TEX_0, CResourceManager::GetInst()->FindRes<CTexture>(L"monster_bullet_red").Get());
	AddResource(L"monster_bulluet_red_material", material);

	material = new CMaterial(true);
	material->SetShader(CResourceManager::GetInst()->FindRes<CGraphicsShader>(L"std2DShader"));
	material->SetData(SHADER_PARAM::TEX_0, CResourceManager::GetInst()->FindRes<CTexture>(L"monster_bullet_blue").Get());
	AddResource(L"monster_bulluet_blue_material", material);

	material = new CMaterial(true);
	material->SetShader(CResourceManager::GetInst()->FindRes<CGraphicsShader>(L"particle_shader"));
	AddResource(L"particle_material", material);
	
	//tilemap
	material = new CMaterial(true);
	material->SetShader(CResourceManager::GetInst()->FindRes<CGraphicsShader>(L"tile_map_shader"));
	AddResource(L"tile_map_material", material);


	material = new CMaterial(true);
	material->SetShader(CResourceManager::GetInst()->FindRes<CGraphicsShader>(L"post_effect_shader"));
	AddResource(L"post_effect_material", material);

	material = new CMaterial(true);
	material->SetShader(CResourceManager::GetInst()->FindRes<CGraphicsShader>(L"box2d_collider_debug"));
	AddResource(L"box2d_collider_debug_material", material);

}

void CResourceManager::CreateDefaultComputeShader()
{
	wstring contentPath = CPathManager::GetInst()->GetContentPath();

	Ptr<CComputeShader> cs = new CTestComputeShader;
	cs->CreateComputeShader(contentPath + L"shader\\test_compute_shader.fx", "cs_main");
	AddResource<CComputeShader>(L"test_compute_shader", cs.Get());

	cs = new CParticleUpdateShader;
	cs->CreateComputeShader(contentPath + L"shader\\particle_update_shader.fx", "cs_main");
	AddResource<CComputeShader>(L"particle_update_shader", cs.Get());

}

void CResourceManager::AddPrefab(const wstring& stringKey, CGameObject* prototype)
{
	Ptr<CPrefab> prefab = new CPrefab(prototype);
	const wstring relativePath = CreateRelativePath(stringKey, prefab);
	prefab->Save(relativePath);
	AddResource<CPrefab>(stringKey, prefab.Get());
}


Ptr<CGraphicsShader> CResourceManager::LoadGraphicShader(const wstring& key, const wstring& strPath, BLEND_TYPE blendType, DEPTH_STENCIL_TYPE depthType , D3D11_PRIMITIVE_TOPOLOGY topology)
{
	Ptr<CGraphicsShader> shader = FindRes<CGraphicsShader>(key);
	wstring contentPath = CPathManager::GetInst()->GetContentPath();
	contentPath += strPath;
	if (nullptr != shader)
	{
		return shader;
	}
	shader->CreateVertexShader(contentPath, "vs_main");
	shader->CreatePixelShader(contentPath, "ps_main");
	shader->SetBlendType(blendType);
	shader->SetDepthStencilType(depthType);
	shader->SetRasterizerType(RASTERIZER_TYPE::CULL_NONE);
	shader->SetTopology(topology);

	AddResource(key, shader.Get());
	return shader;
}


