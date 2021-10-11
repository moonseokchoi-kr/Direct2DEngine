#include "pch.h"

#include "CMaterial.h"
#include "CResourceManager.h"
#include "CMesh.h"
#include "CShader.h"
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
	vector<VTX> vtx;
	vector<UINT> idx;

	VTX v;
	//
	// =========
	// Rect Mesh
	// =========
	// 
	v.pos = Vec3(-0.5f, 0.5f, 0);
	v.color = Vec4(1.f, 0.f, 0.f, 1.f);
	v.uv = Vec2(0, 0);
	vtx.push_back(v);

	v.pos = Vec3(0.5f, 0.5f, 0);
	v.color = Vec4(0.f, 1.f, 0.f, 1.f);
	v.uv = Vec2(1, 0);
	vtx.push_back(v);

	v.pos = Vec3(0.5f, -0.5f, 0);
	v.color = Vec4(0.f, 0.f, 1.f, 1.f);
	v.uv = Vec2(1, 1);
	vtx.push_back(v);

	v.pos = Vec3(-0.5f, -0.5f, 0);
	v.color = Vec4(1.f, 0.f, 1.f, 1.f);
	v.uv = Vec2(0, 1);
	vtx.push_back(v);

	idx.push_back(0);
	idx.push_back(1);
	idx.push_back(2);
	idx.push_back(0);
	idx.push_back(2);
	idx.push_back(3);

	mesh = new CMesh;
	mesh->Create(vtx.data(), (UINT)vtx.size(), idx.data(), (UINT)idx.size());
	AddResource(L"RectMesh", mesh);

	vtx.clear();
	idx.clear();
}

void CResourceManager::CreateDefaultShader()
{
	CShader* stdShader = nullptr;

	wstring strPath = CPathManager::GetInst()->GetContentPath();
	strPath += L"shader\\std2d.fx";
	stdShader = new CShader;
	stdShader->CreateVtxShader(strPath, "VS_std");
	stdShader->CreatePxlShader(strPath, "PS_std");

	AddResource(L"std2DShader", stdShader);

}

void CResourceManager::CreateDefaultTexture()
{
	CTexture* tex = nullptr;

	wstring strPath = CPathManager::GetInst()->GetContentPath();
	strPath += L"texture\\valkiry.DDS";
	tex = new CTexture;
	tex->Load(strPath);
	
	AddResource(L"Background", tex);
}

void CResourceManager::CreateDefaultMaterial()
{
	CMaterial* material = nullptr;

	material = new CMaterial;

	material->SetShader(CResourceManager::GetInst()->FindRes<CShader>(L"std2DShader"));

	AddResource(L"std2DMaterial", material);
}
