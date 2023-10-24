#pragma once

//기기마다 다른 프레임수를 보정해주기 위한 DT(DeltaTime)을 계산해주는 시간 동기화 / 시간 관리 클래스

class CTimeManager
{
	SINGLETON(CTimeManager);

private:

	//DT를 구하는데 필요한 값
	LARGE_INTEGER m_llPrevCount;
	LARGE_INTEGER m_llCurCount;
	LARGE_INTEGER m_llFrequency;

	//구하고자 하는 델타타임
	float		  m_fDeltaTime;

	//델타타임이 누적된 실제 시간
	float		  m_fTime;

	//실제로 tick함수가 호출된 횟수
	UINT		  m_iCallCount;

public:
	void init();
	void tick();
	void render();

public:
	float GetDeltaTime() { return m_fDeltaTime; }

};

