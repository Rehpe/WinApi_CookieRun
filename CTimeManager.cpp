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
	//1�� ���� ��ŭ �� �� �ִ��� �޾ƿ��� �Լ�(1�ʴ� ī���� ������), �ִ� õ������ �� �� �ִ�
	QueryPerformanceFrequency(&m_llFrequency);

	//ù �ʱ�ȭ ������ CurCount�� PrevCount�� �����ϰ� �޾ƿ´�.
	QueryPerformanceCounter(&m_llCurCount);
	QueryPerformanceCounter(&m_llPrevCount);
}

void CTimeManager::tick()
{
	//�Ͻ����� ���¶��
	if (CLevelManager::GetInst()->GetCurLevel()->GetPause())
	{
		QueryPerformanceCounter(&m_llCurCount);

		m_fDeltaTime = (float)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart)
			/ (float)m_llFrequency.QuadPart;

		//���� �ð�
		m_fTime += m_fDeltaTime;

		//���� �Լ� ȣ�� Ƚ��
		m_iCallCount++;

		//���� ī��Ʈ ���� �ٽ� ����ī��Ʈ�� �����ϰ� �ʱ�ȭ�Ѵ�.
		m_llPrevCount = m_llCurCount;

		//DT 0���� ����//
		m_fDeltaTime = 0;
		return;
	}

	//������ ���� ī��Ʈ�� �޾ƿ��� �Լ�
	//init���� �ʱ�ȭ�� �޾Ƴ���, tick���� �ٽ� �޾����� PrevCount�� ���̰� �߻��� ��
	QueryPerformanceCounter(&m_llCurCount);

	//���� ī��Ʈ���� ���� ī��Ʈ�� ����(QuadPart: ī��Ʈ�� ����ִ� ����)
	//�� ī��Ʈ�� ���̸� init���� ���� Frequency(�� õ��)�� ���� ���� �ٷ� 1������ ���� �ð� (D.T)
	//0.000001.. �� ���� ���� ���� ���ؾ��ϹǷ� �Ǽ� ����� �ʿ��ϱ� ������ �Ǽ� ĳ������ ���ش�.
	
	m_fDeltaTime = (float)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart)
		/ (float)m_llFrequency.QuadPart;

	//���� �ð�
	m_fTime += m_fDeltaTime;

	//���� �Լ� ȣ�� Ƚ��
	m_iCallCount++;

	//���� ī��Ʈ ���� �ٽ� ����ī��Ʈ�� �����ϰ� �ʱ�ȭ�Ѵ�.
	m_llPrevCount = m_llCurCount;
}

void CTimeManager::render()
{
	//1�ʿ� �� �� ������ Ÿ��Ʋ�� ����Ͽ� FPS ������ ������ �� �� �ְ� �Ѵ�.
	if (m_fTime >= 1.0f)
	{
		wchar_t szBuff[256] = {};
		swprintf_s(szBuff, L"FPS : %d, DT : %f", m_iCallCount, m_fDeltaTime);
		SetWindowText(CEngine::GetInst()->GetMainWnd(), szBuff);

		//1�� ���� �ʱ�ȭ
		m_fTime = 0.f;
		m_iCallCount = 0;
	}
}
