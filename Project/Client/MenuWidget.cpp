#include "pch.h"
#include "MenuWidget.h"
#include "WidgetManager.h"
#include "CSceneSaveLoad.h"
#include "InspectorWidget.h"



#include <Engine/CSceneManager.h>
#include <Engine/CPathManager.h>
#include <Engine/CEventManager.h>
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
	is_play_ = SCENE_MODE::STOP == CSceneManager::GetInst()->GetSceneMode();
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
		if (ImGui::MenuItem("Save", "CTRL+S",false,is_play_))
		{
			wstring contentPath = CPathManager::GetInst()->GetContentPath();
			contentPath += L"scene\\test.scene";
			CSceneSaveLoad::SaveScene(contentPath);
		}

		if (ImGui::MenuItem("Load", "CTRL+A",false,is_play_))
		{
			wstring contentPath = CPathManager::GetInst()->GetContentPath();
			contentPath += L"scene\\test.scene";
			CScene* newScene = CSceneSaveLoad::LoadScene(contentPath);
			InspectorWidget* widget =(InspectorWidget*)WidgetManager::GetInst()->FindWidget("inspector_view");
			CEventManager::GetInst()->AddEvent(Event{ EVENT_TYPE::SCENE_CHANGE, (DWORD_PTR)newScene, 0 });
			widget->SetGameObject(nullptr);
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

			// 플레이 시작 할 때 저장해둔 임시 Scene 으로 복구한다.
			wstring contentPath = CPathManager::GetInst()->GetContentPath();
			contentPath += L"scene\\test.scene";
			CScene* newScene = CSceneSaveLoad::LoadScene(contentPath);
			InspectorWidget* widget = (InspectorWidget*)WidgetManager::GetInst()->FindWidget("inspector_view");
			CEventManager::GetInst()->AddEvent(Event{ EVENT_TYPE::SCENE_CHANGE, (DWORD_PTR)newScene, 0 });
			widget->SetGameObject(nullptr);
			bPush = true;
		}

		ImGui::EndMenu();
	}

	if (bPush)
	{
		WidgetManager::GetInst()->SetFocusMainWindow();
	}
}
