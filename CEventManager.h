#pragma once

//������Ʈ�� ����/�Ҹ� ���� ó���� ���� tick�� �������ִ� ����ó�� ���� ��� Ŭ���� 
class CObj;

class CEventManager
{
	SINGLETON(CEventManager);

private:
	vector<tEvent>		m_vecEvent;		//�̺�Ʈ ����ü���� ���� ����
	vector<CObj*>		m_vecGarbage;	//���� ó���� ������Ʈ�� ���� ����

public:
	void tick();

public:
	void AddEvent(const tEvent& _event) { m_vecEvent.push_back(_event); };
	
};	

