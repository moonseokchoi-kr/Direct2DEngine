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
	void Start();
	void LateUpdate();
	void UpdateData() override;

	CLONE(CAnimation2D);

public:
	void Create(const wstring& animName, Ptr<CTexture> atlasTexture, UINT leftTopX, UINT leftTopY, UINT sizeX, UINT sizeY, UINT frameCount, float duration, bool filpHorizon=false, bool filpVertical=false);
	void CreateFrame(Ptr<CTexture> atlasTexture, UINT leftTopX, UINT leftTopY, UINT sizeX, UINT sizeY, UINT frameCount, float duration, bool filpHorizon=false, bool filpVertical=false);
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
	UINT GetMaxFrameCount(){ return (UINT)frame_vector_.size(); }
	
	void SetCurrentFrameData(const AnimationFrame& data);
	Vec2 GetBackBoard() { return animation_back_board_; }
	void SetBackBoard(Vec2 back_borad);

	const AnimationFrame& GetCurrentFrameData() { return frame_vector_[current_frame_]; }
	CTexture* GetAtlas() { return atlas_texture_.Get(); }

	bool IsRepeat() { return animation_repeat_; }
	void SetRepeat(bool repeat) { animation_repeat_ = repeat; }

	bool IsPlayOnStart() { return play_on_start_; }
	void SetPlayOnStart(bool play) { play_on_start_ = play; }

	void Clear();
	static void ClearAnim();
	void ClearFrame(int index);

	void SetCollisionOffset();

public:
	void SaveToScene(FILE* file) override;
	void LoadFromScene(FILE* file) override;
private:
	vector<AnimationFrame> frame_vector_;
	CAnimator2D* owner_;
	Ptr<CTexture> atlas_texture_;
	Vec2 animation_back_board_;

	int current_frame_;
	bool animation_finish_;
	float accumulated_time_;

	bool animation_repeat_;
	bool play_on_start_;
	friend class CAnimator2D;
};

