#pragma once



class CTimeManager
	:public CSingleton<CTimeManager>
{
	SINGLE(CTimeManager);

public:
	void Init();
	void Update();
	void Render();

	void Reset();
	void Start();
	void Stop();
public:
	double GetDT() { return delta_time_; }
	float GetfDT() { return static_cast<float>(delta_time_); }

	float GetTotalTime();
public:
	static const float PHYSICS_TIME_STEP;
private:
	LARGE_INTEGER	current_count_; //현재시간
	LARGE_INTEGER	prev_count_; //이전시간
	LARGE_INTEGER   frequency_;
	
	
	double	counts_per_sec_; //초당 카운트수

	double			delta_time_;	// 프레임 사이의 시간 값 
	double			acc_; // 1초 체크를 위한 누적 시간

	LARGE_INTEGER			base_time_;
	LARGE_INTEGER			paused_time_;
	LARGE_INTEGER			stop_time_;

	bool			stopped_;

	UINT			call_count_; // 함수 호출 회수 체크
	UINT			fps_;		// 초당 호출 횟수 

	// FPS
	// 1 프레임당 시간 (Delta Time)
};

