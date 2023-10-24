#include "pch.h"
#include "CEngine.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CLevelManager.h"
#include "CCamera.h"
#include "CPathManager.h"
#include "CResourceManager.h"
#include "CTexture.h"
#include "CEventManager.h"
#include "CCollisionManager.h"
#include "CUIManager.h"
#include "CSoundManager.h"

CEngine::CEngine()
	: m_hMainWnd(nullptr)
	, m_hDC(nullptr)
	, m_ptResolution{}
	, m_pMemTex(nullptr)
	, m_arrPen{}
{
}

CEngine::~CEngine()
{
	//윈도우 등은 알아서 프로그램이 종료될 때 종료되나
	//DC, 타이머 등의 커널 오브젝트는 직접 엔진 소멸자에서 종료시켜주어야한다.
	ReleaseDC(m_hMainWnd, m_hDC);

	//만들어둔 펜을 삭제한다.
	for (UINT i = 0; i < (UINT)PEN_TYPE::END; i++)
	{
		DeleteObject(m_arrPen[i]);
	}
}

void CEngine::init(HWND _hwnd, UINT _iWidth, UINT _iHeight)
{
	//인자로 들어온 메인 윈도우를 멤버변수 m_hMainWnd에 저장
	m_hMainWnd = _hwnd;

	//인자로 들어온 해상도 정보를 멤버변수 m_ptResolution에 저장
	m_ptResolution.x = _iWidth;
	m_ptResolution.y = _iHeight;

	//HDC 초기화(Handle Device Context)
	m_hDC = GetDC(m_hMainWnd);

	//윈도우 크기 설정
	ChangeWindowSize(_iWidth, _iHeight);

	//자주 사용하는 색의 펜과 브러쉬를 미리 만들어둔다.
	CreatePenBrush();

	//Manager 클래스 초기화
	CPathManager::GetInst()->init();
	CTimeManager::GetInst()->init();
	CKeyManager::GetInst()->init();
	CSoundManager::GetInst()->init();
	CLevelManager::GetInst()->init();
}

void CEngine::progress()
{	
	//엔진은 프레임마다 tick과 render을 산하 모든 매니저에게 명령한다.

	//논리적 연산
	tick();

	//화면 렌더링 갱신
	render();

	//Render까지 실행 후, 지연처리가 필요한 이벤트 처리
	CEventManager::GetInst()->tick();
}

void CEngine::tick()
{
	//=====각 매니저들에게 순서에 맞게 tick 명령=====

	//모든 연산은 DT를 기반으로 이루어져야하기 때문에, TimeManager의 tick을 가장 먼저 호출해야한다.
	CTimeManager::GetInst()->tick();

	//DT를 받은 다음 키 상태를 체크하기 위해 KeyManager의 tick을 호출한다.
	CKeyManager::GetInst()->tick();

	//Camera
	CCamera::GetInst()->tick();

	//그 후, 엔진은 매 프레임마다 레벨매니저의 tick을 호출한다.
	CLevelManager::GetInst()->tick();

	//레벨이 가지고 있는 오브젝트들의 연산/움직임이 끝나면, 충돌 연산
	CCollisionManager::GetInst()->tick();

	//UI 이벤트 체크
	CUIManager::GetInst()->tick();
}

void CEngine::render()
{
	//화면 내의 잔상을 없애기 위해 버퍼를 만들어 매 프레임 버퍼에 그려진 화면으로 갱신한다
	
	//1.버퍼 텍스쳐 화면을 깨끗히 덮어버린다
	Rectangle(m_pMemTex->GetDC(), -1, -1, m_ptResolution.x + 1, m_ptResolution.y + 1);

	//그 후 레벨을 버퍼 텍스쳐에 렌더링한다. 
	CLevelManager::GetInst()->render(m_pMemTex->GetDC());

	//카메라 이펙트가 있다면 이때 렌더링
	CCamera::GetInst()->render(m_pMemTex->GetDC());

	//렌더링이 끝나면, 버퍼 텍스쳐에서 메인 윈도우 비트맵으로 렌더링된 화면을 복사한다
	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y,
		m_pMemTex->GetDC(), 0, 0, SRCCOPY);

	//TimeManager -> 타이틀창에 FPS 띄워줌
	CTimeManager::GetInst()->render();

}

void CEngine::ChangeWindowSize(UINT _iWidth, UINT _iHeight)
{
	//인자로 들어온 값을 멤버변수에 저장
	m_ptResolution.x = _iWidth;
	m_ptResolution.y = _iHeight;

	//여기에 적힌 width와 height 값은 윈도우 타이틀창과 메뉴바의 크기를 포함한 값으로,
	//정확하게 타이틀바를 제외한 해상도를 맞추고 싶다면 AdjustWindowRect 함수를 써야한다.

	//좌,상,우,하 의 정수 좌표값을 가진 구조체 RECT
	RECT rt = { 0, 0, m_ptResolution.x, m_ptResolution.y };

	//현재 윈도우의 메뉴 정보를 가져온다
	HMENU hMenu = GetMenu(m_hMainWnd);

	//메뉴가 있는 윈도우라면
	if (hMenu != nullptr)
	{
		//인자로 RECT의 주소값, 윈도우 스타일, 메뉴의 여부 
		//여기서 &rt는 INOUT 인자로, 이 함수가 &rt의 값을 참조하여
		//윈도우 스타일을 고려한 정확한 해상도 사이즈를 다시 &rt에 담아줄 것이다.
		AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, true);
	}
	else
	{
		AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	}

	//이제 &rt에 원하는 윈도우 값을 담아왔으니, SetWindowPos 함수를 실행한다.
	SetWindowPos(m_hMainWnd, nullptr, 0, 0,
		rt.right - rt.left, rt.bottom - rt.top, 0);

	//백버퍼용 텍스쳐가 없다면 제작
	if (m_pMemTex == nullptr)
	{
		//잔상 없는 화면 용 비트맵 제작
		m_pMemTex = CResourceManager::GetInst()->CreateTexture(L"BackBuffer", m_ptResolution.x, m_ptResolution.y);
	}
	//있다면 기존의 백버퍼를 리사이즈한다.
	else
	{
		m_pMemTex->Resize(m_ptResolution.x, m_ptResolution.y);
	}
}

void CEngine::CreatePenBrush()
{
	m_arrPen[(UINT)PEN_TYPE::RED] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_arrPen[(UINT)PEN_TYPE::GREEN] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	m_arrPen[(UINT)PEN_TYPE::BLUE] = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
}
