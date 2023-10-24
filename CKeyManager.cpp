#include "pch.h"
#include "CKeyManager.h"
#include "CEngine.h"

//���� GetAsyncKeyState�� ���� VK ����Ű
//��������� KEY enum class�� ������ ������, ������ ���������Ѵ�.

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
	//���� �߰��� KEY ������ŭ �ݺ����� ���� Ű���¸� m_vecKey�� �����Ѵ�.
	for (int i = 0; i < (int)KEY::END; i++)
	{
		m_vecKey.push_back(tKeyInfo{ (KEY)i, KEY_STATE::NONE });
	}
}

void CKeyManager::tick()
{
	//���� �����찡 focus ����(�����ִ�) ������ ������ �� ������ Ű �Է��� Ȯ���Ѵ�.
	if (GetFocus())
	{
		for (size_t i = 0; i < m_vecKey.size(); i++)
		{
			//Ű�� ���� �����ִ°�? �ǹ��� ��Ʈ���� -> ���� ���ȴٸ�
			if (GetAsyncKeyState(g_arrVK[(UINT)m_vecKey[i].key]) & 0x8000)
			{
				//Ȯ���� Ű�� �� ������ ����(bPrevPressed) üũ
				
				//���� �����ӿ� �ش� Ű�� �������� �ʾҴٸ�
				if (m_vecKey[i].bPrevPressed == false)
				{
					//�� �����ӿ� ó�� ���� ������ Ű ����: TAP
					m_vecKey[i].state = KEY_STATE::TAP;

					//Ű�� bPrevPressed �� ����
					m_vecKey[i].bPrevPressed = true;
				}
				//���� �����ӿ� �ش� Ű�� �����־��ٸ�
				else
				{
					//��������, ���ݵ� �����ִٴ� ���̹Ƿ� Ű ����: PRESSED
					m_vecKey[i].state = KEY_STATE::PRESSED;
				}
			}
			//���� �������� ���� ���¶��
			else
			{
				//���� �����ӿ� �ش� Ű�� �������� �ʾҴٸ�
				if (m_vecKey[i].bPrevPressed == false)
				{
					//��������, ���ݵ� �������� ���� ���·� Ű ����: NONE
					m_vecKey[i].state = KEY_STATE::NONE;
				}
				//���� �����ӿ� �ش� Ű�� �����־��ٸ�
				else
				{
					//���� �����ӿ� ���ȴٰ� �̹� �����ӿ� ������ ������ Ű ����: RELEASE
					m_vecKey[i].state = KEY_STATE::RELEASE;

					//�̹� �����ӿ��� �������� �����Ƿ� bPrevPressed �� false�� �ʱ�ȭ
					m_vecKey[i].bPrevPressed = false;
				}
			}
		}
	}
	//�����찡 focus ���°� �ƴ϶��(��Ȱ��ȭ ����)
	else
	{
		//��� Ű�� ���¸� NONE���� ������� ���̴�.
		for (size_t i = 0; i < m_vecKey.size(); i++)
		{
			//Ű ������ �����ϱ� ���� tap, pressed ���¿��� Ű���� release �ܰ踦 ���� NONE���� �ٲ۴�.
			if (m_vecKey[i].state == KEY_STATE::TAP || m_vecKey[i].state == KEY_STATE::PRESSED)
			{
				m_vecKey[i].state = KEY_STATE::RELEASE;
			}
			//�� if������ release ���·� �ٲ� Ű���� �Ʒ� else if �������� ��� NONE���°� �� ���̴�
			else if (m_vecKey[i].state == KEY_STATE::RELEASE)
			{
				m_vecKey[i].state = KEY_STATE::NONE;
			}
		}
	}


	//���콺 Ŀ���� ��ǥ�� �� ������ ����Ѵ�
	POINT ptMousePos;
	GetCursorPos(&ptMousePos);

	//GetCursorPos���� �޾ƿ� ���� �����찡 ��üȭ���� �������� ���� ���� ��ǥ������, ���� �������� �ػ󵵿� �°� ���������� �ʿ��ϴ�.
	//ScreenToClient�� �����ų ���� ������ �ڵ鰪�� �־��ָ� �� �����쿡 �°� ���콺 ��ǥ���� �������ش�.
	
	ScreenToClient(CEngine::GetInst()->GetMainWnd(), &ptMousePos);
	m_vMousePos = ptMousePos;
}
