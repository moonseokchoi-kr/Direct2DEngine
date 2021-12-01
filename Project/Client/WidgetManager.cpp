#include "pch.h"
#include "WidgetManager.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "Widget.h"


#include <Engine/CKeyManager.h>
#include <Engine/CCore.h>
#include <Engine/CSceneManager.h>
WidgetManager::WidgetManager()
	:is_active_demo_(true)
{

}

WidgetManager::~WidgetManager()
{
	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	Safe_Delete_Map(widget_map_);
}

void WidgetManager::Init(HWND hWnd)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;
	//io.ConfigViewportsNoDefaultParent = true;
	//io.ConfigDockingAlwaysTabBar = true;
	//io.ConfigDockingTransparentPayload = true;
	//io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
	//io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(DEVICE, CONTEXT);

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Read 'docs/FONTS.md' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != NULL);

	CreateWidgets();
}




void WidgetManager::Update()
{
	// Frame ÃÊ±âÈ­
	ImGuiInitFrame();

	// Demo GUI
	ImGuiDemo();


	// GUI Update
	for (const auto& pair : widget_map_)
	{
		if (pair.second->IsActive())
		{
			pair.second->Update();
		}
	}
}

void WidgetManager::Render()
{
	// Rendering
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

Widget* WidgetManager::FindWidget(const string& key)
{
	unordered_map<string, Widget*>::iterator iter = widget_map_.find(key);

	if (iter == widget_map_.end())
	{
		return nullptr;
	}

	return iter->second;
}

void WidgetManager::SetFocusMainWindow()
{
	ImGui::SetWindowFocus(nullptr);
	SetFocus(CCore::GetInst()->GetMainHwnd());
}

#include <Engine/CSceneManager.h>
#include <Engine/CGameObject.h>
#include "MenuWidget.h"
#include "InspectorWidget.h"
#include "ModalListWidget.h"
#include "AtlasTextureTool.h"
#include "HirarchyViewWidget.h"
#include "AnimationTool.h"
#include "AnimationOffsetTool.h"
void WidgetManager::CreateWidgets()
{
	Widget* widget = nullptr;
	
	widget = new MenuWidget;
	AddWidget(widget->GetName(), widget);

	widget = new InspectorWidget;
	CGameObject* target = CSceneManager::GetInst()->FindObjectByName(L"camera");
	dynamic_cast<InspectorWidget*>(widget)->SetGameObject(target);
	AddWidget(widget->GetName(), widget);

	widget = new ModalListWidget;
	widget->Deactivate();
	AddWidget(widget->GetName(), widget);

	widget = new AnimationTool;
	widget->Deactivate();
	AddWidget(widget->GetName(), widget);

	widget = new AnimationOffsetTool;
	widget->Deactivate();
	AddWidget(widget->GetName(), widget);

	widget = new AtlasTextureTool;
	widget->Deactivate();
	AddWidget(widget->GetName(), widget);

	widget = new HirarchyViewWidget;
	widget->Activate();
	AddWidget(widget->GetName(), widget);


	for (const auto& pair : widget_map_)
	{
		pair.second->Init();
	}

}

void WidgetManager::AddWidget(const string& key, Widget* widget)
{
	Widget* wg = FindWidget(key);
	assert(!wg);
	widget_map_.insert(make_pair(key, widget));
}

void WidgetManager::ImGuiInitFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void WidgetManager::ImGuiDemo()
{
	if (KEY_HOLD(KEY::CTRL) && KEY_TAP(KEY::M))
	{
		is_active_demo_ ? is_active_demo_ = false : is_active_demo_ = true;
	}
	if (is_active_demo_)
	{
		ImGui::ShowDemoWindow(&is_active_demo_);
	}
}
