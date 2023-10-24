#include "pch.h"
#include "CKeyManager.h"
#include "CEngine.h"

//실제 GetAsyncKeyState에 들어가는 VK 단축키
//헤더파일의 KEY enum class와 동일한 사이즈, 동일한 순서여야한다.

int g_arrVK[(UINT)KEY::END]
=
{
	VK_UP,
	VK_DOWN,
	VK_LEFT,
	VK_RIGHT,

	VK_ESCAPE,
	VK_SPACE,
	VK_RETURN,
	VK_LCONTROL,
	VK_LMENU,
	VK_LSHIFT,
	VK_TAB,

	VK_LBUTTON,
	VK_RBUTTON,

	'W',
	'A',
	'S',
	'D',

	'Z',

	'1',
	'2',
	'3',
};

CKeyManager::CKeyManager()
{
}

CKeyManager::~CKeyManager()
{
}

void CKeyManager::init()
{
	//내가 추가한 KEY 갯수만큼 반복문을 돌며 키상태를 m_vecKey에 삽입한다.
	for (int i = 0; i < (int)KEY::END; i++)
	{
		m_vecKey.push_back(tKeyInfo{ (KEY)i, KEY_STATE::NONE });
	}
}

void CKeyManager::tick()
{
	//메인 윈도우가 focus 상태(켜져있는) 상태일 때에만 매 프레임 키 입력을 확인한다.
	if (GetFocus())
	{
		for (size_t i = 0; i < m_vecKey.size(); i++)
		{
			//키가 현재 눌려있는가? 의미의 비트연산 -> 현재 눌렸다면
			if (GetAsyncKeyState(g_arrVK[(UINT)m_vecKey[i].key]) & 0x8000)
			{
				//확인할 키의 전 프레임 상태(bPrevPressed) 체크
				
				//이전 프레임에 해당 키가 눌려있지 않았다면
				if (m_vecKey[i].bPrevPressed == false)
				{
					//이 프레임에 처음 눌린 것으로 키 상태: TAP
					m_vecKey[i].state = KEY_STATE::TAP;

					//키의 bPrevPressed 값 변경
					m_vecKey[i].bPrevPressed = true;
				}
				//이전 프레임에 해당 키가 눌려있었다면
				else
				{
					//이전에도, 지금도 눌려있다는 뜻이므로 키 상태: PRESSED
					m_vecKey[i].state = KEY_STATE::PRESSED;
				}
			}
			//현재 눌려있지 않은 상태라면
			else
			{
				//이전 프레임에 해당 키가 눌려있지 않았다면
				if (m_vecKey[i].bPrevPressed == false)
				{
					//이전에도, 지금도 눌려있지 않은 상태로 키 상태: NONE
					m_vecKey[i].state = KEY_STATE::NONE;
				}
				//이전 프레임에 해당 키가 눌려있었다면
				else
				{
					//이전 프레임에 눌렸다가 이번 프레임에 떨어진 것으로 키 상태: RELEASE
					m_vecKey[i].state = KEY_STATE::RELEASE;

					//이번 프레임에는 눌려있지 않으므로 bPrevPressed 값 false로 초기화
					m_vecKey[i].bPrevPressed = false;
				}
			}
		}
	}
	//윈도우가 focus 상태가 아니라면(비활성화 상태)
	else
	{
		//모든 키의 상태를 NONE으로 만들어줄 것이다.
		for (size_t i = 0; i < m_vecKey.size(); i++)
		{
			//키 씹힘을 방지하기 위해 tap, pressed 상태였던 키들은 release 단계를 거쳐 NONE으로 바꾼다.
			if (m_vecKey[i].state == KEY_STATE::TAP || m_vecKey[i].state == KEY_STATE::PRESSED)
			{
				m_vecKey[i].state = KEY_STATE::RELEASE;
			}
			//위 if문에서 release 상태로 바뀐 키들은 아래 else if 구문에서 모두 NONE상태가 될 것이다
			else if (m_vecKey[i].state == KEY_STATE::RELEASE)
			{
				m_vecKey[i].state = KEY_STATE::NONE;
			}
		}
	}


	//마우스 커서의 좌표를 매 프레임 기록한다
	POINT ptMousePos;
	GetCursorPos(&ptMousePos);

	//GetCursorPos에서 받아온 값은 윈도우가 전체화면을 기준으로 했을 때의 좌표값으로, 현재 윈도우의 해상도에 맞게 수정보안이 필요하다.
	//ScreenToClient에 실행시킬 메인 윈도우 핸들값을 넣어주면 이 윈도우에 맞게 마우스 좌표값을 수정해준다.
	
	ScreenToClient(CEngine::GetInst()->GetMainWnd(), &ptMousePos);
	m_vMousePos = ptMousePos;
}
