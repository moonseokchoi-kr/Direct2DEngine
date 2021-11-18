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
    int RegisterLight2D(CLight2D* light)
    {
        light2D_vector_.push_back(light);

        return static_cast<int>(light2D_vector_.size() - 1);
    }
    void CopyRenderTexture();
private:
    void UpdateLight2D();
    void UpdataGlobalData();
private:
    vector<CCamera*> camera_vector_;
    vector<CLight2D*> light2D_vector_;
    Ptr<CTexture> post_effect_target_;
};

