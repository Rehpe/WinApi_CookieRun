#pragma once

//오브젝트의 생성/소멸 등의 처리를 다음 tick에 예약해주는 지연처리 업무 담당 클래스 
class CObj;

class CEventManager
{
	SINGLETON(CEventManager);

private:
	vector<tEvent>		m_vecEvent;		//이벤트 구조체들을 담을 벡터
	vector<CObj*>		m_vecGarbage;	//삭제 처리할 오브젝트를 담을 벡터

public:
	void tick();

public:
	void AddEvent(const tEvent& _event) { m_vecEvent.push_back(_event); };
	
};	

