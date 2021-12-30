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
	LARGE_INTEGER	current_count_; //����ð�
	LARGE_INTEGER	prev_count_; //�����ð�
	LARGE_INTEGER   frequency_;
	
	
	double	counts_per_sec_; //�ʴ� ī��Ʈ��

	double			delta_time_;	// ������ ������ �ð� �� 
	double			acc_; // 1�� üũ�� ���� ���� �ð�

	LARGE_INTEGER			base_time_;
	LARGE_INTEGER			paused_time_;
	LARGE_INTEGER			stop_time_;

	bool			stopped_;

	UINT			call_count_; // �Լ� ȣ�� ȸ�� üũ
	UINT			fps_;		// �ʴ� ȣ�� Ƚ�� 

	// FPS
	// 1 �����Ӵ� �ð� (Delta Time)
};

