#pragma once
#include "CWidget.h"
class CSingleChildWidget :
    public CWidget
{
public:
    CSingleChildWidget();
    ~CSingleChildWidget();
protected:
    virtual void Update() = 0;
    virtual void UpdateChild();
private:
    CWidget* child;
};

