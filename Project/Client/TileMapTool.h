#pragma once
#include "Widget.h"

class CTileMap;
class AtlasTextureTool;

class TileMapTool :
    public Widget
{
public:
    TileMapTool();
    ~TileMapTool();
public:
    void Init() override;
    void Update() override;

public:
    void SetTileMap(CTileMap* tilemap) { tile_map_ = tilemap; }
    
private:
    void ShowTileCanvas();
    void ShowDetail();
    void CalTileIndex();

    void MouseEvent();
private:
    CTileMap* tile_map_;
    AtlasTextureTool* atlas_texture_tool_;

    Vec2 tile_map_size_;
    Vec2 current_mouse_pos_;
    Vec2 canvas_lt_;
    Vec2 canvas_size_;

    vector<int> tile_index_vector_;
    
    int current_tile_index_;
    bool value_change;
    bool size_change;
};

