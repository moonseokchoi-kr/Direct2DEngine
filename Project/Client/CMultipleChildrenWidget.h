#pragma once
#include "Widget.h"
class CMultipleChildrenWidget :
    public CWidget
{
public:
    CMultipleChildrenWidget();
    ~CMultipleChildrenWidget();
    
public:
    virtual void Update() = 0;
protected:
    virtual void UpdateChildren();
private:
    vector<CWidget*> children;
};

