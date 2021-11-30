#include "pch.h"
#include "MenuWidget.h"
#include "WidgetManager.h"
#include <Engine/CSceneManager.h>
#include <Engine/CScene.h>
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
		ShowFileMenu();
		ShowSceneMenu();

		ImGui::EndMainMenuBar();
	}
}

void MenuWidget::ShowFileMenu()
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
}

void MenuWidget::ShowSceneMenu()
{
	SCENE_MODE eCurMode = CSceneManager::GetInst()->GetSceneMode();

	bool enable_play = true;
	bool enable_pause = true;
	bool enable_stop = true;

	if (SCENE_MODE::PLAY == eCurMode)
	{
		enable_play = false;
	}
	else if (SCENE_MODE::PAUSE == eCurMode)
	{
		enable_pause = false;
	}
	else
	{
		enable_pause = false;
		enable_stop = false;
	}

	bool bPush = false;

	if (ImGui::BeginMenu("Scene"))
	{
		if (ImGui::MenuItem("Play", nullptr, nullptr, enable_play))
		{
			SCENE_MODE ePrev = CSceneManager::GetInst()->GetSceneMode();
			CSceneManager::GetInst()->SetSceneMode(SCENE_MODE::PLAY);
			if (SCENE_MODE::STOP == ePrev)
			{
				CScene* pCurScene = CSceneManager::GetInst()->GetCurrentScene();
				//pCurScene->Save();
			}
			bPush = true;
		}

		if (ImGui::MenuItem("Pause", nullptr, nullptr, enable_pause))
		{
			CSceneManager::GetInst()->SetSceneMode(SCENE_MODE::PAUSE);
			bPush = true;
		}

		if (ImGui::MenuItem("Stop", nullptr, nullptr, enable_stop))
		{
			CSceneManager::GetInst()->SetSceneMode(SCENE_MODE::STOP);

			// �÷��� ���� �� �� �����ص� �ӽ� Scene ���� �����Ѵ�.

			bPush = true;
		}

		ImGui::EndMenu();
	}

	if (bPush)
	{
		WidgetManager::GetInst()->SetFocusMainWindow();
	}
}
