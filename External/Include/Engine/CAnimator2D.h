#pragma once
#include "CComponent.h"
#include "CTexture.h"

class CAnimation2D;
class CAnimator2D :
    public CComponent
{
public:
    CAnimator2D();
    CAnimator2D(const CAnimator2D& origin);
    ~CAnimator2D();

public:
    void FinalUpdate() override;
    void UpdateData() override;

    CLONE(CAnimator2D);
    
public:
    void CreateAnimation(const wstring& animName, Ptr<CTexture> atlasTexture, UINT leftTopX, UINT leftTopY, UINT sizeX, UINT sizeY, UINT frameCount, float duration);
    CAnimation2D* FindAnimation(const wstring& animationName);
    void Play(const wstring& animationName, UINT startFrame = 0, bool repeat = false);
    //void Stop()
    //void Pause()

public:
    CAnimation2D* GetCurrentAnimation() { return current_animation_; }

private:
    unordered_map<wstring, CAnimation2D*> animation_map_;
    CAnimation2D* current_animation_;
    bool animation_repeat_;
};

