#pragma once

// Ű �Է� ��� �Ŵ���

enum class KEY
{
	UP,
	DOWN,
	LEFT,
	RIGHT,

	ESC,
	SPACE,
	ENTER,
	LCTRL,
	LALT,
	LSHIFT,
	TAB,

	LBTN,
	RBTN,

	W,
	A,
	S,
	D,

	Z,

	_1,
	_2,
	_3,

	END,
};

enum class KEY_STATE
{
	TAP,		//�� �� ���� ����
	PRESSED,	//��� �����ִ� ����
	RELEASE,	//Ű���� ���� ���� ��, ������ ������ ����

	NONE,
};

struct tKeyInfo
{
	KEY		key;
	KEY_STATE	state;
	bool	bPrevPressed;	//�ش� Ű�� ���� ƽ���� ���� ���� �ִ°�?
};

class CKeyManager
{
	SINGLETON(CKeyManager);

private:
	//Key�� ������ŭ Ű:Ű���� ����ü�� ���� ����
	vector<tKeyInfo> m_vecKey;
	Vec2		m_vMousePos;	//���콺 Ŀ���� ����Ű�� ��ǥ��

public:
	void init();
	void tick();

public:
	KEY_STATE GetKeyState(KEY _key) { return m_vecKey[(UINT)_key].state; }
	Vec2 GetMousePos() { return m_vMousePos;  }
};

