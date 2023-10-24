#pragma once

//��⸶�� �ٸ� �����Ӽ��� �������ֱ� ���� DT(DeltaTime)�� ������ִ� �ð� ����ȭ / �ð� ���� Ŭ����

class CTimeManager
{
	SINGLETON(CTimeManager);

private:

	//DT�� ���ϴµ� �ʿ��� ��
	LARGE_INTEGER m_llPrevCount;
	LARGE_INTEGER m_llCurCount;
	LARGE_INTEGER m_llFrequency;

	//���ϰ��� �ϴ� ��ŸŸ��
	float		  m_fDeltaTime;

	//��ŸŸ���� ������ ���� �ð�
	float		  m_fTime;

	//������ tick�Լ��� ȣ��� Ƚ��
	UINT		  m_iCallCount;

public:
	void init();
	void tick();
	void render();

public:
	float GetDeltaTime() { return m_fDeltaTime; }

};

