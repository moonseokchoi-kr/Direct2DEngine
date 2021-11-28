#pragma once
#include "CEntity.h"
#include "CTexture.h"

class CAnimator2D;

class CAnimation2D :
	public CEntity
{
public:
	CAnimation2D();
	CAnimation2D(const CAnimation2D& origin);
	~CAnimation2D();

public:
	void FinalUpdate();
	void UpdateData() override;

	CLONE(CAnimation2D);

public:
	void Create(const wstring& animName, Ptr<CTexture> atlasTexture, UINT leftTopX, UINT leftTopY, UINT sizeX, UINT sizeY, UINT frameCount, float duration);

public:
	void SetCurrentFrame(UINT currentFrame)
	{
		current_frame_ = currentFrame;
		accumulated_time_ = 0.f;
	}

	void Play()
	{
		animation_finish_ = false;
	}

	bool IsFinish() { return animation_finish_; }
	UINT GetCurrentFrameIndex() { return current_frame_; }
	UINT GetMaxFrameCount(){ return frame_vector_.size(); }
	void SetCurrentFrameData(const AnimationFrame& data);
	const AnimationFrame& GetCurrentFrameData() { return frame_vector_[current_frame_]; }
	void Clear();
	void ClearFrame(int index);
private:
	vector<AnimationFrame> frame_vector_;
	CAnimator2D* owner_;
	Ptr<CTexture> atlas_texture_;
	int current_frame_;
	bool animation_finish_;
	float accumulated_time_;

	friend class CAnimator2D;
};

