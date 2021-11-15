#include "pch.h"
#include "CRenderManager.h"

#include "CDevice.h"
#include "CLight2D.h"
#include "CCamera.h"
#include "CConstBuffer.h"
CRenderManager::CRenderManager()
{
	//메인카메라 자리 선점
	camera_vector_.resize(1);
}

CRenderManager::~CRenderManager()
{

}

void CRenderManager::Render()
{
	UpdateLight2D();

	CDevice::GetInst()->ClearTarget();

	if (nullptr != camera_vector_[0])
	{
		camera_vector_[0]->Render();
	}

	for (size_t i = 1; i < camera_vector_.size(); ++i)
	{
		if(nullptr != camera_vector_[i])
			continue;
		camera_vector_[i]->Render();
	}

	CDevice::GetInst()->Present();

	camera_vector_.clear();
	camera_vector_.resize(1);

	light2D_vector_.clear();
}

int CRenderManager::RegisterCamera(CCamera* camera, int cameraIndex)
{
	if (-1 == cameraIndex)
	{
		camera_vector_.push_back(camera);
		return static_cast<int>(camera_vector_.size() - 1);
	}
	else
	{
		if (camera_vector_.size() < cameraIndex)
		{
			camera_vector_.resize(static_cast<size_t>(cameraIndex + 1));
			camera_vector_[cameraIndex] = camera;

			return cameraIndex;
		}
		else
		{
			if (nullptr != camera_vector_[cameraIndex])
			{
				assert(!(0 == cameraIndex));

				camera_vector_.push_back(camera);
				return static_cast<int>(camera_vector_.size() - 1);
			}
			else
			{
				camera_vector_[cameraIndex] = camera;
				return cameraIndex;
			}
		}
	}
}

void CRenderManager::UpdateLight2D()
{
	struct 
	{
		LightInfo light_info_array[50];
		int	count;
		Vec3 padding;
	}lightStruct;

	for (size_t i = 0; i < light2D_vector_.size(); ++i)
	{
		lightStruct.light_info_array[i] = light2D_vector_[i]->GetLightInfo();
	}
	lightStruct.count = static_cast<int>(light2D_vector_.size());

	CConstBuffer* constBuffer = CDevice::GetInst()->GetConstBuffer(CONSTANT_BUFFER_TYPE::LIGHT2D);
	constBuffer->SetData(&lightStruct, sizeof(lightStruct));
	constBuffer->SetPipelineStage(PIPELINE_STAGE::PS_ALL);
	constBuffer->UpdateData();

}

void CRenderManager::UpdataGlobalData()
{
	static CConstBuffer* cb = CDevice::GetInst()->GetConstBuffer(CONSTANT_BUFFER_TYPE::GLOBAL);
	cb->SetData(&g_global, sizeof(GlobalData));
	cb->SetPipelineStage(PS_ALL);
	cb->UpdateData();
	cb->UpdateDataComputeShdaer();
}
