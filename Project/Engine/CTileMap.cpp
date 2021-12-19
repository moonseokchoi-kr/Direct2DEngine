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
		Tile tile = {};
		tile.index = i;
		tile_altas_image_coord_vector_.push_back(tile);
	}

	buffer_ = new CStructuredBuffer;
	buffer_->Create(sizeof(Tile), tile_map_row_count_ * tile_map_column_count_, STRUCTURE_BUFFER_TYPE::READ_ONLY, nullptr,true);
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

	material_->SetData(SHADER_PARAM::VEC2_0, &atlasResolution);
	material_->SetData(SHADER_PARAM::VEC2_1, &tileUV);

	material_->SetData(SHADER_PARAM::TEX_0, atlas_texture_.Get());

	material_->UpdateData();
	buffer_->Create(buffer_->GetElementSize(), tile_map_row_count_ * tile_map_column_count_, buffer_->GetBufferType(), nullptr, buffer_->IsCpuAccess());
	buffer_->SetData(tile_altas_image_coord_vector_.data(), (UINT)(sizeof(Tile) * tile_altas_image_coord_vector_.size()));
	buffer_->UpdateData(PIPELINE_STAGE::PS_PIXEL,20);
}

void CTileMap::Render()
{
	UpdateData();
	mesh_->Render();
}

void CTileMap::SaveToScene(FILE* file)
{
	CComponent::SaveToScene(file);

	SaveResReference(atlas_texture_, file);
	fwrite(&tile_map_column_count_, sizeof(int), 1, file);
	fwrite(&tile_map_row_count_, sizeof(int), 1, file);

	fwrite(&tile_size_, sizeof(Vec2), 1, file);

	UINT iDataSize = buffer_->GetElementSize() * buffer_->GetElementCount();
	fwrite(&iDataSize, sizeof(UINT), 1, file);

	void* data = malloc(iDataSize);
	buffer_->GetData(data, iDataSize);
	fwrite(data, iDataSize, 1, file);

	free(data);

}

void CTileMap::LoadFromScene(FILE* file)
{
	CComponent::LoadFromScene(file);

	LoadResReference(atlas_texture_, file);
	fread(&tile_map_column_count_, sizeof(int), 1, file);
	fread(&tile_map_row_count_, sizeof(int), 1, file);

	fread(&tile_size_, sizeof(Vec2), 1, file);

	UINT dataSize = buffer_->GetElementSize() * buffer_->GetElementCount();
	fread(&dataSize, sizeof(UINT), 1, file);

	void* data = malloc(dataSize);
	fread(data, dataSize, 1, file);

	buffer_->Create(sizeof(int), tile_map_column_count_ * tile_map_row_count_, STRUCTURE_BUFFER_TYPE::READ_ONLY, data, true);
}
