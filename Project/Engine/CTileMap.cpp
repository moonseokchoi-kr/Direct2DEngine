#include "pch.h"
#include "CTileMap.h"

#include "CTransform.h"
#include "CMaterial.h"
#include "CMesh.h"

#include "CResourceManager.h"



CTileMap::CTileMap()
	:CComponent(COMPONENT_TYPE::TILEMAP)
	,tile_map_column_count_(8)
	,tile_map_row_count_(6)
	,tile_size_(Vec2(64.f,64.f))
{
	mesh_ = CResourceManager::GetInst()->FindRes<CMesh>(L"RectMesh");
	material_ = CResourceManager::GetInst()->FindRes<CMaterial>(L"tile_map_material");

	for (UINT i = 0; i < (UINT)(tile_map_column_count_ * tile_map_row_count_); ++i)
	{
		tile_altas_image_coord_vector_.push_back(i);
	}

	buffer_ = new CStructuredBuffer;
	buffer_->Create(sizeof(int), tile_map_row_count_ * tile_map_column_count_, STRUCTURE_BUFFER_TYPE::READ_ONLY, nullptr,true);
}

CTileMap::CTileMap(const CTileMap& origin)
	:CComponent(COMPONENT_TYPE::TILEMAP)
{
}

CTileMap::~CTileMap()
{
	SafeDelete(buffer_);
}

void CTileMap::FinalUpdate()
{
}

void CTileMap::UpdateData()
{
	GetTransform()->UpdateData();
	material_->SetData(SHADER_PARAM::INT_0, &tile_map_column_count_);
	material_->SetData(SHADER_PARAM::INT_1, &tile_map_row_count_);

	Vec2 atlasResolution = Vec2((float)atlas_texture_->GetWidth(), (float)atlas_texture_->GetHeight());
	Vec2 tileUV = tile_size_ / atlasResolution;
	int iTileidx = 7;
	material_->SetData(SHADER_PARAM::INT_2, &iTileidx);
	



	material_->SetData(SHADER_PARAM::VEC2_0, &atlasResolution);
	material_->SetData(SHADER_PARAM::VEC2_1, &tileUV);

	material_->SetData(SHADER_PARAM::TEX_0, atlas_texture_.Get());

	material_->UpdateData();
	buffer_->Create(sizeof(int), tile_map_row_count_ * tile_map_column_count_, STRUCTURE_BUFFER_TYPE::READ_ONLY, nullptr, true);
	buffer_->SetData(tile_altas_image_coord_vector_.data(), (UINT)(sizeof(int) * tile_altas_image_coord_vector_.size()));
	buffer_->UpdateData(PIPELINE_STAGE::PS_PIXEL,20);
}

void CTileMap::Render()
{
	UpdateData();
	mesh_->Render();
}
