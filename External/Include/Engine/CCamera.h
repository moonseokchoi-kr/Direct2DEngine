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
	void UpdateData() override{};

public:
    void SetProjectionType(PROJECTION_TYPE type) { projection_type_ = type; }

private:
    PROJECTION_TYPE projection_type_;
    Matrix view_matrix_;
    Matrix projection_matrix_;
    float far_z_;
    float angle_of_view_y_;
};

