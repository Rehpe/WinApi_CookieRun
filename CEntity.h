#pragma once

// 엔진, 매니저 클래스 등 싱글톤 클래스를 제외한 게임의 모든 클래스들은 Entity 클래스를 상속받는다.
// Entity의 정의: 실체, 객체, 관리가 필요한 것들
// 최상위 부모 클래스가 CEntity로 통일되어 관리가 용이해진다.

class CEntity
{
public:
	CEntity();
	CEntity(const CEntity& _Origin);		//복사 생성자
	virtual ~CEntity();						//최상위 부모클래스 소멸자에는 virtual 키워드
	
public:
	virtual CEntity* Clone() = 0;		//Clone을 순수가상함수 처리하여 반드시 override하게 만듦

private:
	static UINT	g_iNextID;		

private:
	//모든 객체에게 부여되는 ID값, const이기 때문에 최초 한번만 초기화되며, 바꿀 수 없다
	const UINT		m_id; 
	wstring			m_strName;

public:
	void SetName(const wstring& _str) { m_strName = _str; }
	const wstring& GetName() { return m_strName; }
	UINT GetID() { return m_id; }

};

