#pragma once

// 키 입력 담당 매니저

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
	TAP,		//한 번 눌린 상태
	PRESSED,	//계속 눌려있는 상태
	RELEASE,	//키에서 손을 뗐을 때, 눌림이 해제된 상태

	NONE,
};

struct tKeyInfo
{
	KEY		key;
	KEY_STATE	state;
	bool	bPrevPressed;	//해당 키가 이전 틱에서 눌린 적이 있는가?
};

class CKeyManager
{
	SINGLETON(CKeyManager);

private:
	//Key의 갯수만큼 키:키상태 구조체를 담을 벡터
	vector<tKeyInfo> m_vecKey;
	Vec2		m_vMousePos;	//마우스 커서가 가리키는 좌표값

public:
	void init();
	void tick();

public:
	KEY_STATE GetKeyState(KEY _key) { return m_vecKey[(UINT)_key].state; }
	Vec2 GetMousePos() { return m_vMousePos;  }
};

