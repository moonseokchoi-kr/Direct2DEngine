#include "pch.h"
#include "CollisionManagerView.h"
#include <Engine/CCollisionManager.h>

CollisionManagerView::CollisionManagerView()
	:Widget("Collision Manager")
{
}

CollisionManagerView::~CollisionManagerView()
{
}

void CollisionManagerView::Init()
{
}

void CollisionManagerView::Update()
{
	if (ImGui::Begin(GetName().c_str(), &is_active_))
	{
		if (ImGui::CollapsingHeader("Check Collision Layer", ImGuiTreeNodeFlags_DefaultOpen))
		{
			//layer checkbox 구성
			//역계단 형태
		}


		ImGui::End();
	}
	else
	{
		ImGui::End();
	}
}
