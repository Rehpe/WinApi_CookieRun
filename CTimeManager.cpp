#include "pch.h"
#include "CTimeManager.h"
#include "CEngine.h"
#include "CLevelManager.h"
#include "CLevel.h"

CTimeManager::CTimeManager()
	: m_llPrevCount{}
	, m_llCurCount{}
	, m_llFrequency{}
	, m_fDeltaTime(0.f)
	, m_fTime(0.f)
	, m_iCallCount(0)
{
}

CTimeManager::~CTimeManager()
{
}

void CTimeManager::init()
{
	//1초 동안 얼만큼 셀 수 있는지 받아오는 함수(1초당 카운팅 증가량), 최대 천만까지 셀 수 있다
	QueryPerformanceFrequency(&m_llFrequency);

	//첫 초기화 때에는 CurCount와 PrevCount를 동일하게 받아온다.
	QueryPerformanceCounter(&m_llCurCount);
	QueryPerformanceCounter(&m_llPrevCount);
}

void CTimeManager::tick()
{
	//일시정지 상태라면
	if (CLevelManager::GetInst()->GetCurLevel()->GetPause())
	{
		QueryPerformanceCounter(&m_llCurCount);

		m_fDeltaTime = (float)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart)
			/ (float)m_llFrequency.QuadPart;

		//누적 시간
		m_fTime += m_fDeltaTime;

		//실제 함수 호출 횟수
		m_iCallCount++;

		//이전 카운트 값을 다시 현재카운트와 동일하게 초기화한다.
		m_llPrevCount = m_llCurCount;

		//DT 0으로 고정//
		m_fDeltaTime = 0;
		return;
	}

	//현재의 누적 카운트를 받아오는 함수
	//init에서 초기화로 받아놓고, tick에서 다시 받았으니 PrevCount와 차이가 발생할 것
	QueryPerformanceCounter(&m_llCurCount);

	//현재 카운트에서 이전 카운트를 뺀다(QuadPart: 카운트가 들어있는 변수)
	//그 카운트의 차이를 init에서 구한 Frequency(약 천만)로 나눈 값이 바로 1프레임 간격 시간 (D.T)
	//0.000001.. 의 아주 작은 수를 구해야하므로 실수 계산이 필요하기 때문에 실수 캐스팅을 해준다.
	
	m_fDeltaTime = (float)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart)
		/ (float)m_llFrequency.QuadPart;

	//누적 시간
	m_fTime += m_fDeltaTime;

	//실제 함수 호출 횟수
	m_iCallCount++;

	//이전 카운트 값을 다시 현재카운트와 동일하게 초기화한다.
	m_llPrevCount = m_llCurCount;
}

void CTimeManager::render()
{
	//1초에 한 번 윈도우 타이틀에 출력하여 FPS 정보를 눈으로 볼 수 있게 한다.
	if (m_fTime >= 1.0f)
	{
		wchar_t szBuff[256] = {};
		swprintf_s(szBuff, L"FPS : %d, DT : %f", m_iCallCount, m_fDeltaTime);
		SetWindowText(CEngine::GetInst()->GetMainWnd(), szBuff);

		//1초 누적 초기화
		m_fTime = 0.f;
		m_iCallCount = 0;
	}
}
