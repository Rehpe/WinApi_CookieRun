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
    CFSM*       m_pOwnerFSM;        //State를 소유하고 있는 FSM
    wstring    m_strKey;

public:
    //State는 본인 상태로 전환됐을 때 할 일(final tick), 진입과 퇴장 함수를 필수로 가져야한다
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

