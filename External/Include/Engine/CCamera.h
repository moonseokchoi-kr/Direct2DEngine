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
    void Render() override;
public:
    void SetProjectionType(PROJECTION_TYPE type) { projection_type_ = type; }
    void CheckAllLayout() { layer_check_out_ = 0xffffffff; }
    void SetMainCamera() { camera_index_ = 0; }
public:
    CLONE(CCamera);

private:
    PROJECTION_TYPE projection_type_;
    Matrix view_matrix_;
    Matrix projection_matrix_;
    float far_z_;
    float angle_of_view_y_;

    UINT layer_check_out_;

    int camera_index_;
};

