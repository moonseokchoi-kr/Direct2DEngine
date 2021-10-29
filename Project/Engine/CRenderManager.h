#pragma once
#include "singleton.h"

class CCamera;
class CLight2D;

class CRenderManager :
    public CSingleton<CRenderManager>
{
    SINGLE(CRenderManager);

public:
    void Render();
public:
    int RegisterCamera(CCamera* camera, int cameraIndex);
    int RegisterLight2D(CLight2D* light)
    {
        light2D_vector_.push_back(light);

        return light2D_vector_.size() - 1;
    }

private:
    void UpdateLight2D();
private:
    vector<CCamera*> camera_vector_;
    vector<CLight2D*> light2D_vector_;
};

