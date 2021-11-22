#include "pch.h"
#include "CKeyManager.h"

#include "CCore.h"

int g_arrVK[(int)KEY::LAST] =
{
	VK_LEFT, //LEFT,
	VK_RIGHT,//RIGHT,
	VK_UP,//UP,
	VK_DOWN,//DOWN,	

	'Q',
	'W',
	'E',
	'R',
	'T',
	'Y',
	'U',
	'I',
	'O',
	'P',
	'A',
	'S',
	'D',
	'F',
	'G',
	'Z',
	'X',
	'C',
	'V',
	'B',
	'M',
	
	VK_MENU,
	VK_LCONTROL,
	VK_LSHIFT,
	VK_SPACE,
	VK_RETURN,
	VK_ESCAPE,
	
	VK_LBUTTON,
	VK_RBUTTON,

	//LAST,
};





CKeyManager::CKeyManager()
{
}

CKeyManager::~CKeyManager()
{
}

void CKeyManager::Init()
{
	for (int i = 0; i < (int)KEY::LAST; ++i)
	{
		vec_key_.push_back(KeyInfo{ KEY_STATE::NONE, false });
	}	
}

void CKeyManager::Update()
{
	// 윈도우 포커싱 알아내기
	//HWND hMainWnd = CCore::GetInst()->GetMainHwnd();
	HWND hWnd = GetFocus();

	// 윈도우 포커싱 중일 때 키 이벤트 동작
	if (nullptr != hWnd)
	{
		for (int i = 0; i < (int)KEY::LAST; ++i)
		{
			// 키가 눌려있다.
			if (GetAsyncKeyState(g_arrVK[i]) & 0x8000)
			{
				if (vec_key_[i].prev_push)
				{
					// 이전에도 눌려있었다.
					vec_key_[i].state = KEY_STATE::HOLD;
				}
				else
				{
					// 이전에 눌려있지 않았다.
					vec_key_[i].state = KEY_STATE::TAP;
				}

				vec_key_[i].prev_push = true;
			}

			// 키가 안눌려있다.
			else
			{
				if (vec_key_[i].prev_push)
				{
					// 이전에 눌려있었다.
					vec_key_[i].state = KEY_STATE::AWAY;
				}
				else
				{
					// 이전에도 안눌려있었다.
					vec_key_[i].state = KEY_STATE::NONE;
				}

				vec_key_[i].prev_push = false;
			}
		}

		// Mouse 위치 계산
		POINT ptPos = {};
		GetCursorPos(&ptPos);
		ScreenToClient(CCore::GetInst()->GetMainHwnd(), &ptPos);

		cur_mouse_pos_ = Vec2((float)ptPos.x, (float)ptPos.y);
	}

	// 윈도우 포커싱 해제상태
	else
	{
		for (int i = 0; i < (int)KEY::LAST; ++i)
		{
			vec_key_[i].prev_push = false;

			if (KEY_STATE::TAP == vec_key_[i].state || KEY_STATE::HOLD == vec_key_[i].state)
			{
				vec_key_[i].state = KEY_STATE::AWAY;				
			}		
			else if (KEY_STATE::AWAY == vec_key_[i].state)
			{
				vec_key_[i].state = KEY_STATE::NONE;
			}			
		}
	}	
}
