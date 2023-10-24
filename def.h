#pragma once

//정의, 매크로(define) 등을 정리해둔 헤더파일

//싱글톤 선언 매크로
#define SINGLETON(type) public:\
						static type* GetInst()\
						{\
						static type inst;\
						return &inst;\
						}\
						private:\
							type();\
							~type();

//D.T 매크로
#define DT CTimeManager::GetInst()->GetDeltaTime()

//키보드 입력 확인 매크로
#define IsRelease(key) CKeyManager::GetInst()->GetKeyState(key) == KEY_STATE::RELEASE
#define IsTap(key) CKeyManager::GetInst()->GetKeyState(key) == KEY_STATE::TAP
#define IsPressed(key) CKeyManager::GetInst()->GetKeyState(key) == KEY_STATE::PRESSED

// nullptr인지 확인하고 아닐 경우 delete하고 nullptr로 초기화하는 동적할당 삭제 매크로
#define DEL(p) if(p != nullptr) delete p; p = nullptr;

//현재 마우스 포인터의 좌표 위치를 반환하는 매크로
#define MOUSE_POS CKeyManager::GetInst()->GetMousePos()

//복사함수
#define CLONE(type) virtual type* Clone() { return new type(*this); }

//복사함수를 사용하지 않음(비활성화)
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
	BACKGROUND,			//제일 뒤에 깔릴 배경
	DEFAULT,			//레이어 지정을 하지 않았을 때의 제일 기본 레이어
	OBSTACLE,			//바닥, 장애물 등
	JELLY,
	GROUND,
	BACKGROUND_EFFECT,				
	PLAYER,				//플레이어와 관련 있는 오브젝트들(플레이어의 SFX등)
	PET,				//펫
	PLAYER_SKILL,		//플레이어 스킬
	MONSTER,
	EFFECT,				//효과
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
	STATE_CHANGE,		//	Wparam: FSM Component 주소, lParam: 바꿀 State 이름

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

