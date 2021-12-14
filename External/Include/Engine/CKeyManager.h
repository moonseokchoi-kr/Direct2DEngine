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
	M,

	ALT,
	CTRL,
	LSHIFT,
	SPACE,
	ENTER,
	ESC,

	INS,
	DEL,
	PGUP,
	PGDOWN,
	HOME,

	LBTN,
	RBTN,

	N0,
	N1,
	N2,
	N3,
	N4,
	N5,
	N6,
	N7,
	N8,
	N9,

	NUMPAD0,
	NUMPAD1,
	NUMPAD2,
	NUMPAD3,
	NUMPAD4,
	NUMPAD5,
	NUMPAD6,
	NUMPAD7,
	NUMPAD8,
	NUMPAD9,

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
	Vec2			cur_mouse_pos_;
	Vec2			prev_mouse_pos_;
	Vec2			cur_mouse_dir_;
public:
	void Init();
	void Update();

public:
	KEY_STATE GetKeyState(KEY key){return vec_key_[static_cast<int>(key)].state;}
	Vec2 GetMousePos() { return cur_mouse_pos_; }
	Vec2 GetMouseDir() { return cur_mouse_dir_; }
};

