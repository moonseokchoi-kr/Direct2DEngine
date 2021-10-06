#pragma once

// 1. ������ ����ȭ
// ���� ������ ������ ���� Ű�� ����, ������ �̺�Ʈ�� ��������.

// 2. Ű �Է� �̺�Ʈ ó��
// tap, hold, away

enum class KEY_STATE
{
	NONE, // ������ �ʾҰ�, �������� ������ ���� ����
	TAP, // �� ���� ����
	HOLD, // ������ �ִ� 
	AWAY, // �� �� ����	
};

enum class KEY
{
	LEFT,
	RIGHT,
	UP,
	DOWN,

	Q,
	W,
	E,
	R,
	T,
	Y,
	U,
	I,
	O,
	P,
	A,
	S,
	D,
	F,
	G,
	Z,
	X,
	C,
	V,
	B,

	ALT,
	CTRL,
	LSHIFT,
	SPACE,
	ENTER,
	ESC,

	LBTN,
	RBTN,

	LAST,
};

struct KeyInfo
{	
	KEY_STATE	state;		// Ű�� ���°�
	bool		prev_push;	// ���� �����ӿ��� ���ȴ��� ����
};

class CKeyManager
	:public CSingleton<CKeyManager>
{
	SINGLE(CKeyManager);
private:
	vector<KeyInfo> vec_key_;
	Vec2			 cur_mouse_pos_;


public:
	void Init();
	void Update();

public:
	KEY_STATE GetKeyState(KEY key){return vec_key_[static_cast<int>(key)].state;}
	Vec2 GetMousePos() { return cur_mouse_pos_; }
};

