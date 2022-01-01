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
    void Start() override;
    void LateUpdate() override;
    void UpdateData() override;
    void FinalUpdate() override{}
    CLONE(CAnimator2D);
    
public:
    void CreateAnimation(const wstring& animName, Ptr<CTexture> atlasTexture, UINT leftTopX, UINT leftTopY, UINT sizeX, UINT sizeY, UINT frameCount, float duration, bool flipHorizon=false, bool flipVertical = false);
    CAnimation2D* FindAnimation(const wstring& animationName);
    void Play(const wstring& animationName, UINT startFrame = 0, bool repeat = false);
    void AddAnimation(CAnimation2D* animtion);
    //void Stop()
    //void Pause()

public:
    void SaveToScene(FILE* file) override;
    void LoadFromScene(FILE* file) override;

public:
    CAnimation2D* GetCurrentAnimation() { return current_animation_; }

    const unordered_map<wstring, CAnimation2D*>& GetAnimations() { return animation_map_; }
private:
    unordered_map<wstring, CAnimation2D*> animation_map_;
    CAnimation2D* current_animation_;

};

