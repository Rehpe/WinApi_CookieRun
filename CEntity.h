#pragma once

// ����, �Ŵ��� Ŭ���� �� �̱��� Ŭ������ ������ ������ ��� Ŭ�������� Entity Ŭ������ ��ӹ޴´�.
// Entity�� ����: ��ü, ��ü, ������ �ʿ��� �͵�
// �ֻ��� �θ� Ŭ������ CEntity�� ���ϵǾ� ������ ����������.

class CEntity
{
public:
	CEntity();
	CEntity(const CEntity& _Origin);		//���� ������
	virtual ~CEntity();						//�ֻ��� �θ�Ŭ���� �Ҹ��ڿ��� virtual Ű����
	
public:
	virtual CEntity* Clone() = 0;		//Clone�� ���������Լ� ó���Ͽ� �ݵ�� override�ϰ� ����

private:
	static UINT	g_iNextID;		

private:
	//��� ��ü���� �ο��Ǵ� ID��, const�̱� ������ ���� �ѹ��� �ʱ�ȭ�Ǹ�, �ٲ� �� ����
	const UINT		m_id; 
	wstring			m_strName;

public:
	void SetName(const wstring& _str) { m_strName = _str; }
	const wstring& GetName() { return m_strName; }
	UINT GetID() { return m_id; }

};

