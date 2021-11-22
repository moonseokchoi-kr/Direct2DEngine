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
	// ������ ��Ŀ�� �˾Ƴ���
	//HWND hMainWnd = CCore::GetInst()->GetMainHwnd();
	HWND hWnd = GetFocus();

	// ������ ��Ŀ�� ���� �� Ű �̺�Ʈ ����
	if (nullptr != hWnd)
	{
		for (int i = 0; i < (int)KEY::LAST; ++i)
		{
			// Ű�� �����ִ�.
			if (GetAsyncKeyState(g_arrVK[i]) & 0x8000)
			{
				if (vec_key_[i].prev_push)
				{
					// �������� �����־���.
					vec_key_[i].state = KEY_STATE::HOLD;
				}
				else
				{
					// ������ �������� �ʾҴ�.
					vec_key_[i].state = KEY_STATE::TAP;
				}

				vec_key_[i].prev_push = true;
			}

			// Ű�� �ȴ����ִ�.
			else
			{
				if (vec_key_[i].prev_push)
				{
					// ������ �����־���.
					vec_key_[i].state = KEY_STATE::AWAY;
				}
				else
				{
					// �������� �ȴ����־���.
					vec_key_[i].state = KEY_STATE::NONE;
				}

				vec_key_[i].prev_push = false;
			}
		}

		// Mouse ��ġ ���
		POINT ptPos = {};
		GetCursorPos(&ptPos);
		ScreenToClient(CCore::GetInst()->GetMainHwnd(), &ptPos);

		cur_mouse_pos_ = Vec2((float)ptPos.x, (float)ptPos.y);
	}

	// ������ ��Ŀ�� ��������
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
