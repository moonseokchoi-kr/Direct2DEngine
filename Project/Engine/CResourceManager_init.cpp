#include "pch.h"

#include "CMaterial.h"
#include "CResourceManager.h"
#include "CMesh.h"
#include "CGraphicsShader.h"
#include "CTexture.h"

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
	indexBuffer.push_back(0);
	indexBuffer.push_back(2);
	indexBuffer.push_back(3);

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

	wstring strPath = CPathManager::GetInst()->GetContentPath();
	strPath += L"shader\\std2d.fx";
	stdShader = new CGraphicsShader;
	stdShader->CreateVertexShader(strPath, "VS_std");
	stdShader->CreatePixelShader(strPath, "PS_std");
 	stdShader->SetBlendType(BLEND_TYPE::ALPHA_BLEND);
 	stdShader->SetRasterizerType(RASTERIZER_TYPE::CULL_NONE);
	AddResource(L"std2DShader", stdShader);


	//colliderShader
	
}

void CResourceManager::CreateDefaultTexture()
{
	CTexture* tex = nullptr;

	wstring strPath = CPathManager::GetInst()->GetContentPath();
	strPath += L"texture\\valkiry.PNG";
	tex = new CTexture;
	tex->Load(strPath);
	
	AddResource(L"Background", tex);
}

void CResourceManager::CreateDefaultMaterial()
{
	CMaterial* material = nullptr;

	material = new CMaterial;

	material->SetShader(CResourceManager::GetInst()->FindRes<CGraphicsShader>(L"std2DShader"));

	AddResource(L"std2DMaterial", material);
}
