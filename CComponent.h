#pragma once
#include "CEntity.h"

//����� �Ѱ踦 ������, Ư���� Ż���� �����ϰ� ���ִ� Component Ŭ����

class CComponent :
    public CEntity
{
    friend class CObj;      //Component Ŭ������ Ư���� ������ ���� ��ü(pOWner)�� ���´�

public:
    //Component Ŭ������ Ư���� ������ ���� ��ü�� ������ �ϹǷ�, �����ڿ��� ������ ��ü�� �޾ƾ��Ѵ�.
    CComponent(CObj* _pOwner);
    virtual ~CComponent();

private:
    CObj* m_pOwner; //�ڱ� �ڽ�(Component)�� ������ ��ü

public:
    //Component�� �߻� Ŭ�����̹Ƿ�, tick�� render�� ���������Լ��� ���´�.
    virtual void tick() = 0;
    virtual void final_tick() {};
    virtual void render(HDC _dc) = 0;

public:
    CObj* GetOwner() { return m_pOwner; }

private:
    void SetOwner(CObj* _pOwner) { m_pOwner = _pOwner; }
};

