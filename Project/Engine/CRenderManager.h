#pragma once
#include "singleton.h"
#include "ptr.h"
#include "CTexture.h"

class CCamera;
class CLight2D;

class CRenderManager :
    public CSingleton<CRenderManager>
{
    SINGLE(CRenderManager);

public:
    void Init();
    void Render();
public:
    int RegisterCamera(CCamera* camera, int cameraIndex);
    void RegisterToolCamera(CCamera* camera) { tool_camera_ = camera; }
    int RegisterLight2D(CLight2D* light)
    {
        light2D_vector_.push_back(light);

        return static_cast<int>(light2D_vector_.size() - 1);
    }
    void CopyRenderTexture();

    CTexture* GetViewPortTexture() { return viewport_traget_.Get(); }
private:
    void UpdateLight2D();
    void UpdataGlobalData();
    void RenderPlay();
    void RenderTool();
private:
    vector<CCamera*> camera_vector_;
    vector<CLight2D*> light2D_vector_;
    Ptr<CTexture> post_effect_target_;
    Ptr<CTexture> viewport_traget_;
    CCamera* tool_camera_;
};

