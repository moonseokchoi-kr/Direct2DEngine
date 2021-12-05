#include "pch.h"
#include "TileMapWidget.h"
#include "TileMapTool.h"
#include "WidgetManager.h"

#include <Engine/CGameObject.h>
TileMapWidget::TileMapWidget()
	:ComponentWidget("TileMap",Vec2(0,100))
{
}

TileMapWidget::~TileMapWidget()
{
}

void TileMapWidget::Update()
{
	Start();
	{
		if (ImGui::BeginTable("##detail", 2))
		{
			ImGui::TableNextColumn();
			ImGui::Text("TileMap");
			ImGui::TableNextColumn();

			if (ImGui::Button("Open Editor"))
			{
				TileMapTool* tileMap = dynamic_cast<TileMapTool*>(WidgetManager::GetInst()->FindWidget("tile_map_tool"));
				tileMap->SetTileMap(GetTarget()->TileMap());
				tileMap->Activate();
			}
			ImGui::EndTable();
		}

	}
	End();
}
