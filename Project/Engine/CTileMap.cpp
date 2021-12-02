#include "pch.h"
#include "CTileMap.h"

#include "CTransform.h"
#include "CMaterial.h"
#include "CMesh.h"

#include "CResourceManager.h"



CTileMap::CTileMap()
	:CComponent(COMPONENT_TYPE::TILEMAP)
	,tile_map_column_size_(1)
	,tile_map_row_size_(1)
	,tile_size_(Vec2(64.f,64.f))
{
	mesh_ = CResourceManager::GetInst()->FindRes<CMesh>(L"RectMesh");
	material_ = CResourceManager::GetInst()->FindRes<CMaterial>(L"tile_map_material");

}

CTileMap::CTileMap(const CTileMap& origin)
	:CComponent(COMPONENT_TYPE::TILEMAP)
{
}

CTileMap::~CTileMap()
{
}

void CTileMap::FinalUpdate()
{
}

void CTileMap::UpdateData()
{
	GetTransform()->UpdateData();
	material_->SetData(SHADER_PARAM::INT_0, &tile_map_column_size_);
	material_->SetData(SHADER_PARAM::INT_1, &tile_map_row_size_);

	int tileIndex = 0;
	material_->SetData(SHADER_PARAM::INT_2, &tileIndex);

	Vec2 atlasResolution = Vec2((float)atlas_texture_->GetWidth(), (float)atlas_texture_->GetHeight());
	Vec2 tileUV = tile_size_ / atlasResolution;

	material_->SetData(SHADER_PARAM::VEC2_0, &atlasResolution);
	material_->SetData(SHADER_PARAM::VEC2_1, &tileUV);

	material_->SetData(SHADER_PARAM::TEX_0, atlas_texture_.Get());

	material_->UpdateData();
}

void CTileMap::Render()
{
	UpdateData();
	mesh_->Render();
}
