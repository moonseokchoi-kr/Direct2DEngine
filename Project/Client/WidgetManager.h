#pragma once

class Widget;
class CTexture;

class WidgetManager :
    public CSingleton<WidgetManager>
{
    SINGLE(WidgetManager);

public:
    void Init(HWND hWnd);
    void Update();
    void Render();

public:
    Widget* FindWidget(const string& key);
    void SetFocusMainWindow();

private:
    void CreateWidgets();
    void AddWidget(const string& key, Widget* widget);
    void ImGuiInitFrame();
    void ImGuiDemo();
    void DockingSpace();
private:
    bool is_active_demo_;
    unordered_map<string, Widget*> widget_map_;
};

