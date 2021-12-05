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
        tile_map_column_count_ = col;
        tile_map_row_count_ = row;
        tile_altas_image_coord_vector_.resize((size_t)(col * row));
    }
    void SetTileSize(Vec2 tileSize)
    {
        tile_size_ = tileSize;
    }

    int GetTileMapColumnCount() { return tile_map_column_count_; }
    int GetTileMapRowCount() { return tile_map_row_count_; }
    Ptr<CMesh> GetMesh() { return mesh_;}
    Ptr<CMaterial> GetMaterial() { return material_; }
    Ptr<CTexture>GetAtlas() { return atlas_texture_; }
    const vector<Tile>& GetTileIndexVector() { return tile_altas_image_coord_vector_; }
    void SetTileIndexVector(vector<Tile>& indexVector) { tile_altas_image_coord_vector_ = indexVector; }
private:
    Ptr<CMesh> mesh_;
    Ptr<CMaterial> material_;
    Ptr<CTexture> atlas_texture_;

    CStructuredBuffer* buffer_;

    vector<Tile> tile_altas_image_coord_vector_;

    int tile_map_column_count_;
    int tile_map_row_count_;

    int current_tile_index_;


    Vec2 tile_size_;
};

