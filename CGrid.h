#pragma once
#include "CButtonUI.h"

typedef void(CEntity::* DELEGATE)(void);

class CGrid :
    public CButtonUI
{
public:
    CGrid();
    ~CGrid();

public:
    CLONE(CGrid);

private:
    // Delegate
   // CEntity*        m_pInst;
   // DELEGATE        m_pDelegateFunc;

public:
  //  virtual void tick();
    virtual void render(HDC _dc);


public:
    //void SetDelegate(CEntity* _pInst, DELEGATE _Func)
    //{
    //    m_pInst = _pInst;
    //    m_pDelegateFunc = _Func;
    //}

    virtual void MouseLbtnClicked() override;


};

