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

	//DockingSpace();

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
void WidgetManager::ImGuiInitFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
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
#include <Engine/CResourceManager.h>
#include <Engine/CTexture.h>
#include "MenuWidget.h"
#include "InspectorWidget.h"
#include "ModalListWidget.h"
#include "AtlasTextureTool.h"
#include "HirarchyViewWidget.h"
#include "AnimationTool.h"
#include "AnimationOffsetTool.h"
#include "TileMapTool.h"
#include "ViewPortWidget.h"
#include "ComboWidget.h"

void WidgetManager::CreateWidgets()
{
	Widget* widget = nullptr;
	
	widget = new MenuWidget;
	AddWidget(widget->GetName(), widget);

	widget = new InspectorWidget;
	widget->Activate();
	Ptr<CMaterial> target = CResourceManager::GetInst()->FindRes<CMaterial>(L"backgound_material");
	((InspectorWidget*)widget)->SetTargetResource(target.Get());
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
	widget->Deactivate();
	AddWidget(widget->GetName(), widget);

	widget = new TileMapTool;
	widget->Deactivate();
	AddWidget(widget->GetName(), widget);

	widget = new ViewPortWidget;
	widget->Activate();
	AddWidget(widget->GetName(), widget);

	widget = new ComboWidget;
	widget->Deactivate();
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

void WidgetManager::DockingSpace()
{
	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static bool is_open = true;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &is_open,window_flags);
	if (!opt_padding)		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// Submit the DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

// 	if (ImGui::BeginMenuBar())
// 	{
// 		if (ImGui::BeginMenu("Options"))
// 		{
// 			// Disabling fullscreen would allow the window to be moved to the front of other windows,
// 			// which we can't undo at the moment without finer window depth/z control.
// 			ImGui::MenuItem("Full screen", NULL, &opt_fullscreen);
// 			ImGui::MenuItem("Padding", NULL, &opt_padding);
// 			ImGui::Separator();
// 
// 			if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
// 			if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
// 			if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
// 			if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
// 			if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
// 			ImGui::Separator();
// 
// 			if (ImGui::MenuItem("Close", NULL, false, &is_open != NULL))
// 				is_open = false;
// 			ImGui::EndMenu();
// 		}
// 
// 		ImGui::EndMenuBar();
// 	}

	ImGui::End();
}
