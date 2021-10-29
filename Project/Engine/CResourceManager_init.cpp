#include "pch.h"

#include "CMaterial.h"
#include "CResourceManager.h"
#include "CMesh.h"
#include "CGraphicsShader.h"
#include "CTexture.h"
#include "CPrefab.h"
#include "CPathManager.h"

void CResourceManager::Init()
{
	CreateDefaultMesh();
	CreateDefaultShader();
	CreateDefaultTexture();
	CreateDefaultMaterial();
}

void CResourceManager::CreateDefaultMesh()
{
	CMesh* mesh = nullptr;
	vector<VTX> vertexBuffer;
	vector<UINT> indexBuffer;

	VTX vertex;
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
	
	AddResource(L"collder2DShader", stdShader);

	//HP bar
	stdShader = new CGraphicsShader;
	strPath = CPathManager::GetInst()->GetContentPath();
	strPath += L"shader\\monsterhpshader.fx";
	stdShader->CreateVertexShader(strPath, "vs_main");
	stdShader->CreatePixelShader(strPath, "ps_main");

	stdShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	stdShader->SetDepthStencilType(DEPTH_STENCIL_TYPE::NO_TEST);

	AddResource(L"monster_hp_shader", stdShader);


	//monster bullet
	stdShader = new CGraphicsShader;
	strPath = CPathManager::GetInst()->GetContentPath();
	strPath += L"shader\\monsterbulletshader.fx";
	stdShader->CreateVertexShader(strPath, "vs_main");
	stdShader->CreatePixelShader(strPath, "ps_main");

	stdShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	stdShader->SetDepthStencilType(DEPTH_STENCIL_TYPE::NO_TEST);

	AddResource(L"monster_bullet_shader", stdShader);
}

void CResourceManager::CreateDefaultTexture()
{
	wstring strContent = CPathManager::GetInst()->GetContentPath();
	CTexture* texture = nullptr;

	wstring strFilePath = strContent + L"texture\\background.png";
	texture = new CTexture;
	texture->Load(strFilePath);
	AddResource(L"background", texture);

	strContent = CPathManager::GetInst()->GetContentPath();
	
	strFilePath = strContent + L"texture\\raimu_player.png";
	texture = new CTexture;
	texture->Load(strFilePath);
	AddResource(L"player", texture);

	 strContent = CPathManager::GetInst()->GetContentPath();

	strFilePath = strContent + L"texture\\monster_yuki.png";
	texture = new CTexture;
	texture->Load(strFilePath);
	AddResource(L"monster", texture);
	
	texture = new CTexture;
	wstring strPath = CPathManager::GetInst()->GetContentPath();
	strPath += L"texture\\monster_bullet.png";
	texture->Load(strPath);
	AddResource(L"monster_bullet_red", texture);


	texture = new CTexture;
	strPath = CPathManager::GetInst()->GetContentPath();
	strPath += L"texture\\monster_bullet_1.png";
	texture->Load(strPath);
	AddResource(L"monster_bullet_blue", texture);
}

void CResourceManager::CreateDefaultMaterial()
{
	CMaterial* material = nullptr;
	int a = 1;
	material = new CMaterial;
	material->SetShader(CResourceManager::GetInst()->FindRes<CGraphicsShader>(L"std2DShader"));
	AddResource(L"std2DMaterial", material);

	material = new CMaterial;
	material->SetShader(CResourceManager::GetInst()->FindRes<CGraphicsShader>(L"std2DShader"));
	material->SetData(SHADER_PARAM::INT_0, &a);
	material->SetData(SHADER_PARAM::TEX_0, CResourceManager::GetInst()->FindRes<CTexture>(L"player").Get());
	AddResource(L"std2DMaterial_hit", material);

	//colliderMaterial
	material = new CMaterial;
	material->SetShader(CResourceManager::GetInst()->FindRes<CGraphicsShader>(L"collder2DShader"));
	AddResource(L"collider2DMaterial_none", material);

	material = new CMaterial;
	material->SetShader(CResourceManager::GetInst()->FindRes<CGraphicsShader>(L"collder2DShader"));
	
	material->SetData(SHADER_PARAM::INT_0, &a);
	AddResource(L"collider2DMaterial_collision",material);

	//hp material
	material = new CMaterial;
	material->SetShader(CResourceManager::GetInst()->FindRes<CGraphicsShader>(L"monster_hp_shader"));
	AddResource(L"monster_hp_material", material);


	//bullet material
	material = new CMaterial;
	material->SetShader(CResourceManager::GetInst()->FindRes<CGraphicsShader>(L"std2DShader"));
	material->SetData(SHADER_PARAM::TEX_0, CResourceManager::GetInst()->FindRes<CTexture>(L"monster_bullet_red").Get());
	AddResource(L"monster_bulluet_red_material", material);

	material = new CMaterial;
	material->SetShader(CResourceManager::GetInst()->FindRes<CGraphicsShader>(L"std2DShader"));
	material->SetData(SHADER_PARAM::TEX_0, CResourceManager::GetInst()->FindRes<CTexture>(L"monster_bullet_blue").Get());
	AddResource(L"monster_bulluet_blue_material", material);
	
}


Ptr<CTexture> CResourceManager::LoadTexture(const wstring& key, const wstring& strPath)
{
	Ptr<CTexture> texture = FindRes<CTexture>(key);
	wstring contentPath = CPathManager::GetInst()->GetContentPath();
	contentPath += strPath;
	if (nullptr!=texture)
	{
		return texture;
	}
	texture->Load(contentPath);
	AddResource(key, texture.Get());
	return texture;
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

void CResourceManager::AddPrefab(const wstring& stringKey, CGameObject* prototype)
{
	Ptr<CPrefab> prefab = new CPrefab(prototype);

	AddResource<CPrefab>(stringKey, prefab.Get());
}