#pragma once
#include "CComponent.h"
class CCamera :
    public CComponent
{
public:
    CCamera();
    ~CCamera();

public:
    void Update() override;
    void LateUpdate() override;
    void FinalUpdate() override;
    virtual void UpdateData() {};

private:
    Matrix view_matrix_;
};

