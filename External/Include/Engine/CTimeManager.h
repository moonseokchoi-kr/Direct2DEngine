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
	double GetDT() { return m_deltaTime; }
	float GetfDT() { return (float)m_deltaTime; }

	float GetTotalTime();
private:
	LARGE_INTEGER	m_currentCount; //현재시간
	LARGE_INTEGER	m_prevCount; //이전시간
	LARGE_INTEGER   m_frequency;
	
	
	double	m_countsPerSec; //초당 카운트수

	double			m_deltaTime;	// 프레임 사이의 시간 값 
	double			m_dAcc; // 1초 체크를 위한 누적 시간

	LARGE_INTEGER			m_baseTime;
	LARGE_INTEGER			m_pausedTime;
	LARGE_INTEGER			m_stopTime;

	bool			m_stopped;

	UINT			m_callCount; // 함수 호출 회수 체크
	UINT			m_fps;		// 초당 호출 횟수 

	// FPS
	// 1 프레임당 시간 (Delta Time)
};

