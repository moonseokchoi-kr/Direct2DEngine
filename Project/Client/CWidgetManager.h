#pragma once




class CWidget;

class CWidgetManager :
    public CSingleton<CWidgetManager>
{
    SINGLE(CWidgetManager);

public:
    void Init(HWND hWnd);
    void Update();
    void Render();

public:
    CWidget* FindWidget(const string& key);

private:
    void CreateWidgets();
    void AddWidget(const string& key, CWidget* widget);
    void ImGuiInitFrame();
    void ImGuiDemo();

private:
    bool is_active_demo_;
    unordered_map<string, CWidget*> widget_map_;

};

