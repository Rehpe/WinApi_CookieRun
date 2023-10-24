#pragma once
#include "CEntity.h"
#include "CFSM.h"

class CFSM;
class CObj;

class CState :
    public CEntity
{

    friend class CFSM;

public:
    CState();
    virtual ~CState();

public:
    CLONE_DEACTIVATE(CState);

private:
    CFSM*       m_pOwnerFSM;        //State�� �����ϰ� �ִ� FSM
    wstring    m_strKey;

public:
    //State�� ���� ���·� ��ȯ���� �� �� ��(final tick), ���԰� ���� �Լ��� �ʼ��� �������Ѵ�
    virtual void final_tick();
    virtual void Enter();
    virtual void Exit();

public:
    CFSM* GetOwnerFSM() { return m_pOwnerFSM; }
    CObj* GetOwner() { return m_pOwnerFSM->GetOwner(); }

    void ChangeState(const wchar_t* _strStateName);

    const wstring& GetKey() { return m_strKey; }
    void SetKey(const wstring& _strKey) { m_strKey = _strKey; }


};

