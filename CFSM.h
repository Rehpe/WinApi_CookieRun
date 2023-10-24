#pragma once
#include "CComponent.h"

//플레이어가 가지는 다양한 상태를 컨트롤하는 클래스

class CState;

class CFSM :
    public CComponent
{
public:
    CFSM(CObj* _pOwner);
    CFSM(const CFSM& _other);
    ~CFSM();

public:
    CLONE(CFSM);

private:
    map<wstring, CState*>   m_mapState;
    CState*                 m_pCurState;

public:
    virtual void tick() override;
    virtual void final_tick() override;
    virtual void render(HDC _dc) override;

public:
    void AddState(const wstring& _strKey, CState* _pState);
    CState* FindState(const wstring& _strKey);
    
    CState* GetCurState() { return m_pCurState; }

    void ChangeState(const wstring& _strStateName);

};

