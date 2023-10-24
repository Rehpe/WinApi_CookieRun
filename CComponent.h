#pragma once
#include "CEntity.h"

//상속의 한계를 보안한, 특성을 탈부착 가능하게 해주는 Component 클래스

class CComponent :
    public CEntity
{
    friend class CObj;      //Component 클래스는 특성상 무조건 소유 객체(pOWner)를 갖는다

public:
    //Component 클래스는 특성상 무조건 소유 객체를 가져야 하므로, 생성자에서 무조건 객체를 받아야한다.
    CComponent(CObj* _pOwner);
    virtual ~CComponent();

private:
    CObj* m_pOwner; //자기 자신(Component)을 소유한 객체

public:
    //Component는 추상 클래스이므로, tick과 render을 순수가상함수로 갖는다.
    virtual void tick() = 0;
    virtual void final_tick() {};
    virtual void render(HDC _dc) = 0;

public:
    CObj* GetOwner() { return m_pOwner; }

private:
    void SetOwner(CObj* _pOwner) { m_pOwner = _pOwner; }
};

