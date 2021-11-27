#pragma once
#include "ComponentWidget.h"

class AnimationWidget :
    public ComponentWidget
{
public:
    AnimationWidget();
    ~AnimationWidget();
public:
    void Init() override;
    void Update() override;

private:
    void ChangeAnimation(DWORD_PTR instance, DWORD_PTR animationName);

};
