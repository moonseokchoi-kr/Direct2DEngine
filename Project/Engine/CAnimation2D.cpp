#include "pch.h"
#include "CAnimation2D.h"
#include "CConstBuffer.h"

#include "CTimeManager.h"
#include "CDevice.h"

CAnimation2D::CAnimation2D()
	:owner_(nullptr)
	,current_frame_(0)
	,animation_finish_(false)
	,accumulated_time_(0.f)
{
}

CAnimation2D::CAnimation2D(const CAnimation2D& origin)
	:owner_(nullptr)
	,current_frame_(0)
	,animation_finish_(false)
	,accumulated_time_(0.f)
{
}

CAnimation2D::~CAnimation2D()
{
}

void CAnimation2D::FinalUpdate()
{
	if (animation_finish_)
		return;
	accumulated_time_ += fDT;

	if (frame_vector_[current_frame_].duration < accumulated_time_)
	{
		++current_frame_;

		if (frame_vector_.size() <= current_frame_)
		{
			current_frame_ = 0;
			animation_finish_ = true;
			accumulated_time_ = 0.f;
			return;
		}
		accumulated_time_ -= frame_vector_[current_frame_].duration;
	}
}

void CAnimation2D::UpdateData()
{
	AnimationData data;
	data = frame_vector_[current_frame_].animation_data;
	CConstBuffer* cb = CDevice::GetInst()->GetConstBuffer(CB_TYPE::ANIMATE2D);
	cb->SetData(&data, sizeof(AnimationData));
	cb->SetPipelineStage(PIPELINE_STAGE::PS_PIXEL);
	cb->UpdateData();

	atlas_texture_->SetPipelineStage(PIPELINE_STAGE::PS_PIXEL, 12);
	atlas_texture_->UpdateData();
}

void CAnimation2D::Create(const wstring& animationName, Ptr<CTexture> texture, UINT leftTopX, UINT leftTopY, UINT sizeX, UINT sizeY, UINT frameCount, float duration)
{
	SetName(animationName);

	atlas_texture_ = texture;

	float width = (float)atlas_texture_->GetWidth();
	float height = (float)atlas_texture_->GetHeight();

	AnimationFrame frame = {};

	for (UINT i = 0; i < frameCount; ++i)
	{
		UINT ltX = leftTopX + (sizeX * i);
		UINT ltY = leftTopY;

		frame.animation_data.size = Vec2((float)sizeX / width, (float)sizeY / height);
		frame.animation_data.full_size = frame.animation_data.size * 2.f;
		frame.animation_data.left_top = Vec2((float)ltX / width, (float)ltY / height);
		frame.animation_data.left_top -= frame.animation_data.size / 2.f;

		frame.duration = duration;

		frame_vector_.push_back(frame);
	}
}
