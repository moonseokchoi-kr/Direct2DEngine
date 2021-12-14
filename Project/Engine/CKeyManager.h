#pragma once

// 1. 프레임 동기화
// 동일 프레임 내에서 같은 키에 대해, 동일한 이벤트를 가져간다.

// 2. 키 입력 이벤트 처리
// tap, hold, away

enum class KEY_STATE
{
	NONE, // 눌리지 않았고, 이전에도 눌리지 않은 상태
	TAP, // 막 누른 시점
	HOLD, // 누르고 있는 
	AWAY, // 막 뗀 시점	
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
	KEY_STATE	state;		// 키의 상태값
	bool		prev_push;	// 이전 프레임에서 눌렸는지 여부
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

