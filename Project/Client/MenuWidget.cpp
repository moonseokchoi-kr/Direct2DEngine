#include "pch.h"
#include "MenuWidget.h"

MenuWidget::MenuWidget()
	:Widget("menu_bar")
{
}

MenuWidget::~MenuWidget()
{
}

void MenuWidget::Update()
{
	// Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save", "CTRL+S"))
			{
				int a = 0;
			}

			if (ImGui::MenuItem("Load", "CTRL+A"))
			{
				int a = 0;
			}

			ImGui::EndMenu();
		}


		ImGui::EndMainMenuBar();
	}
}
