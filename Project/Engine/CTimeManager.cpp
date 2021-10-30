#include "pch.h"
#include "CTimeManager.h"

#include "CCore.h"

CTimeManager::CTimeManager()
	: current_count_{}
	, prev_count_{}
	, frequency_{}
	, delta_time_(0.)
	, acc_(0.)
	, call_count_(0)
	, stopped_(false)
	, stop_time_{}
	, base_time_{}
	, paused_time_{}
{

}

CTimeManager::~CTimeManager()
{

}

void CTimeManager::Init()
{
	// 현재 카운트
	QueryPerformanceCounter(&prev_count_);
	base_time_ = prev_count_;
	// 초당 카운트 횟수
	QueryPerformanceFrequency(&frequency_);
	counts_per_sec_ = 1.0 / (double)frequency_.QuadPart;
}

void CTimeManager::Update()
{
	if (stopped_)
	{
		delta_time_ = 0.0f;
		return;
	}
	QueryPerformanceCounter(&current_count_);

	// 이전 프레임의 카운팅과, 현재 프레임 카운팅 값의 차이를 구한다.
	delta_time_ = (double)(current_count_.QuadPart - prev_count_.QuadPart) * (double)counts_per_sec_;

	// 이전카운트 값을 현재값으로 갱신(다음번에 계산을 위해서)
	prev_count_ = current_count_;
	
	if (delta_time_ < 0.0)
	{
		delta_time_ = 0.0f;
	}

#ifdef _DEBUG
	if (delta_time_ > (1. / 60.))
		delta_time_ = (1. / 60.);
#endif
}

void CTimeManager::Render()
{
	++call_count_;
	acc_ += delta_time_; // DT 누적

	if (acc_ >= 1.)
	{
		fps_ = call_count_;
		acc_ = 0.;
		call_count_ = 0;

		wchar_t szBuffer[255] = {}; 
		swprintf_s(szBuffer, L"FPS : %d,  DT : %lf", fps_, delta_time_);
		SetWindowText(CCore::GetInst()->GetMainHwnd(), szBuffer);
	}
}

void CTimeManager::Reset()
{
}

void CTimeManager::Start()
{
	LARGE_INTEGER startTime;
	QueryPerformanceCounter(&startTime);

	if (stopped_)
	{
		paused_time_.QuadPart += (startTime.QuadPart - stop_time_.QuadPart);

		prev_count_ = startTime;
		stop_time_ = LARGE_INTEGER();
		stopped_ = false;
	}
}

void CTimeManager::Stop()
{
	LARGE_INTEGER currTime;
	QueryPerformanceCounter(&currTime);
	if (!stopped_)
	{
		stop_time_ = currTime;
		stopped_ = true;
	}
}

float CTimeManager::GetTotalTime()
{
	if (stopped_)
	{
		return(float)(((stop_time_.QuadPart - paused_time_.QuadPart) - base_time_.QuadPart) * counts_per_sec_);

	}
	else
	{
		return(float)(((current_count_.QuadPart - paused_time_.QuadPart) - base_time_.QuadPart) * counts_per_sec_);
	}
}
