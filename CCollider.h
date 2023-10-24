#pragma once
#include "CComponent.h"

//오브젝트들의 충돌 관련

class CCollider :
    public CComponent
{
public:
    //충돌체는 특성상 무조건 소유 객체를 가져야 하므로, 생성자에서 무조건 객체를 받아야한다.
    CCollider(CObj* _pOwner);
    CCollider(const CCollider& _other);     //복사생성자
    ~CCollider();

public:
    CLONE(CCollider);

private:
    //기본적으로 충돌체는 자신을 소유한 객체의 위치를 따라다니는데,
    //오프셋 위치 조정을 통해 객체와 얼만큼 떨어져있는지 최종 위치를 셋팅할 수 있다.

    Vec2    m_vOffsetPos;       //주인 객체의 중심점과 얼만큼 떨어져있는지 (고정값)
    Vec2    m_vFinalPos;        //주인 객체의 중심점 위치에 오프셋값을 더한 충돌체의 최종 위치 (가변값) 

    Vec2    m_vScale;           //충돌체의 크기
    Vec2    m_vOffsetScale;     //충돌체 크기 오프셋
    Vec2    m_vFinalScale;      //충돌체 최종 크기

    int     m_iOverlapCount;    //다른 충돌체와 충돌중인지, 몇 개와 충돌중인지 알 수 있는 값

    float   m_fSizeRatio;       //충돌체 크기 비율

public:
    virtual void tick() override;
    virtual void final_tick() override;
    virtual void render(HDC _dc) override;

public:
    void SetOffsetPos(Vec2 _vOffsetPos) { m_vOffsetPos = _vOffsetPos; }
    void SetScale(Vec2 _vScale) { m_vScale = _vScale; }
    void SetOffsetScale(Vec2 _vOffsetScale) { m_vOffsetScale = _vOffsetScale; };

    Vec2 GetOffsetPos() { return m_vOffsetPos; }
    Vec2 GetFinalScale() { return m_vFinalScale; }

    Vec2 GetFinalPos() { return m_vFinalPos; }
    void SetFinalScale(Vec2 _vFinalScale) { m_vFinalScale = _vFinalScale; };

    void SetSizeRatio(float _f) { m_fSizeRatio = _f; };


public:
    //충돌시 호출되는 충돌관련 함수들

    void BeginOverlap(CCollider* _pOther);  //충돌 시작시 한 번 호출
    void OnOverlap(CCollider* _pOther);     //계속 충돌중일 때 지속적으로 호출
    void EndOverlap(CCollider* _pOther);    //충돌이 끝난 후 떨어질 때 한 번 호출

};

