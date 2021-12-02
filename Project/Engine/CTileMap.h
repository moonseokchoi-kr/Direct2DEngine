#pragma once
#include "CComponent.h"
#include "CMesh.h"
#include "CTexture.h"
#include "CMaterial.h"

class CStructuredBuffer;
class CTileMap :
    public CComponent
{
public:
    CTileMap();
    CTileMap(const CTileMap& origin);
    ~CTileMap();


public:
    CLONE(CTileMap);
public:
    void FinalUpdate() override;
    void UpdateData() override;
    void Render();
public:
    void SetAtlasTexture(Ptr<CTexture> tex) { atlas_texture_ = tex; }
    void SetTileMapSize(int col, int row)
    {
        tile_map_column_size_ = col;
        tile_map_row_size_ = row;
    }
    void SetTileSize(Vec2 tileSize)
    {
        tile_size_ = tileSize;
    }

    Ptr<CMesh> GetMesh() { return mesh_;}
    Ptr<CMaterial> GetMaterial() { return material_; }
private:
    Ptr<CMesh> mesh_;
    Ptr<CMaterial> material_;
    Ptr<CTexture> atlas_texture_;

    CStructuredBuffer* buffer_;

    vector<int> tile_altas_image_coord_vector_;

    int tile_map_column_size_;
    int tile_map_row_size_;

    int current_tile_index_;

    Vec2 tile_size_;
};

