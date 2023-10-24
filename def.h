#pragma once

//����, ��ũ��(define) ���� �����ص� �������

//�̱��� ���� ��ũ��
#define SINGLETON(type) public:\
						static type* GetInst()\
						{\
						static type inst;\
						return &inst;\
						}\
						private:\
							type();\
							~type();

//D.T ��ũ��
#define DT CTimeManager::GetInst()->GetDeltaTime()

//Ű���� �Է� Ȯ�� ��ũ��
#define IsRelease(key) CKeyManager::GetInst()->GetKeyState(key) == KEY_STATE::RELEASE
#define IsTap(key) CKeyManager::GetInst()->GetKeyState(key) == KEY_STATE::TAP
#define IsPressed(key) CKeyManager::GetInst()->GetKeyState(key) == KEY_STATE::PRESSED

// nullptr���� Ȯ���ϰ� �ƴ� ��� delete�ϰ� nullptr�� �ʱ�ȭ�ϴ� �����Ҵ� ���� ��ũ��
#define DEL(p) if(p != nullptr) delete p; p = nullptr;

//���� ���콺 �������� ��ǥ ��ġ�� ��ȯ�ϴ� ��ũ��
#define MOUSE_POS CKeyManager::GetInst()->GetMousePos()

//�����Լ�
#define CLONE(type) virtual type* Clone() { return new type(*this); }

//�����Լ��� ������� ����(��Ȱ��ȭ)
#define CLONE_DEACTIVATE(type) virtual type* Clone() { assert(nullptr); return nullptr;}

#define PI 3.1415926535f
#define GRID_SIZE 30
#define BTN_SIZE 94


enum class LEVEL_TYPE
{
	INTRO,
	STAGE_01,
	BONUS,
	SELECT,
	LOAD,
	RESULT,

	EDITOR,

	END,
};


enum class PEN_TYPE
{
	RED,
	GREEN,
	BLUE,

	END,
};

enum class LAYER
{
	BACKGROUND,			//���� �ڿ� �� ���
	DEFAULT,			//���̾� ������ ���� �ʾ��� ���� ���� �⺻ ���̾�
	OBSTACLE,			//�ٴ�, ��ֹ� ��
	JELLY,
	GROUND,
	BACKGROUND_EFFECT,				
	PLAYER,				//�÷��̾�� ���� �ִ� ������Ʈ��(�÷��̾��� SFX��)
	PET,				//��
	PLAYER_SKILL,		//�÷��̾� ��ų
	MONSTER,
	EFFECT,				//ȿ��
	GRID = 13,
	UI = 14,
	PREVIEW = 15,
	END = 16 
};

enum class EVENT_TYPE
{
	CREATE_OBJECT,		//	wparam : Object Adress,	lparam: LAYER
	DELETE_OBJECT,		//	Wparam : Object Adress
	LEVEL_CHANGE,		//	Wparam : LEVEL_TYPE(Next Level)
	STATE_CHANGE,		//	Wparam: FSM Component �ּ�, lParam: �ٲ� State �̸�

};

enum class CAMERA_EFFECT
{
	NONE,
	FADE_IN,
	FADE_OUT,
	DIM,
};

enum class SELECT_COOKIE
{
	NONE,
	GINGERBRAVE,
	MILK,
	FAIRY,
	SNOWSUGAR,
};

enum class SELECT_PET
{
	NONE,
	CHOCODROP,
	MILKBOTTLE,
	FLOWERPOD,
	SNOWGLOBE,
};

enum class CLASS_TYPE
{
	NONE,
	GROUND,
	PLATFORM,
	FIXED_OBSTACLE = 3,
	JELLY,
	ITEM_BEARJELLYPARTY,
	ITEM_BIGGER,
	ITEM_BONUS,
	ITEM_BOOSTER,
	ITEM_COINMAGIC,
	ITEM_MAGNET = 10,
	ITEM_ICEBEARJELLYPARTY,
	ITEM_MINIPOTION,
	ITEM_BIGPOTION,
	JELLY_YELLOWBEAR,
	JELLY_PINKBEAR,
	JELLY_ICEBEAR,
	JELLY_BIGBEAR,
	JELLY_RAINBOWBEAR,
	BONUS_RAINBOW,
	BONUS_GOLD = 20,
	COIN_SILVER,
	COIN_GOLD,
	COIN_BIGSILVER,
	COIN_BIGGOLD,
	ANIM_OBS1 = 30,
	ANIM_OBS2,
	ANIM_OBS3,
	ANIM_OBS4,
	ANIM_OBS5,
	ANIM_OBS6,
	ANIM_OBS7,
	ANIM_OBS8,
	
	TRIGGER,

};

