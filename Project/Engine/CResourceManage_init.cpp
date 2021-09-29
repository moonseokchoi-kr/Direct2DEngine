#include "pch.h"
#include "CResourceManager.h"
#include "CMesh.h"
#include "CShader.h"

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
	v.pos = Vec3(-0.5f, 0.5f, 0.5f);
	v.color = Vec4(1.f, 0.f, 0.f, 1.f);
	vtx.push_back(v);

	v.pos = Vec3(0.5f, 0.5f, 0.5f);
	v.color = Vec4(0.f, 1.f, 0.f, 1.f);
	vtx.push_back(v);

	v.pos = Vec3(0.5f, -0.5f, 0.5f);
	v.color = Vec4(0.f, 0.f, 1.f, 1.f);
	vtx.push_back(v);

	v.pos = Vec3(-0.5f, -0.5f, 0.5f);
	v.color = Vec4(1.f, 0.f, 1.f, 1.f);
	vtx.push_back(v);

	idx.push_back(0);
	idx.push_back(1);
	idx.push_back(2);
	idx.push_back(0);
	idx.push_back(2);
	idx.push_back(3);

	mesh = new CMesh;
	mesh->Create(vtx.data(), vtx.size(), idx.data(), idx.size());
	AddRes(L"RectMesh", mesh);

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

	AddRes(L"std2DShader", stdShader);

}

void CResourceManager::CreateDefaultTexture()
{
}

void CResourceManager::CreateDefaultMaterial()
{
}
