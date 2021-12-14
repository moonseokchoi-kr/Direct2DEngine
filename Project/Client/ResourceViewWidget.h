#pragma once
#include "Widget.h"

#include "TreeWidget.h"
class ResourceViewWidget :
    public Widget
{
public:
    ResourceViewWidget();
    ~ResourceViewWidget();

public:
    void Init() override;
    void Update() override;
public:
    void Renew();
    void ReleaseSelectNode() { tree_.ReleaseSelectNode(); }
public:
    void ResourceItemClicked(DWORD_PTR item, DWORD_PTR data);

private:
    TreeWidget tree_;
};

