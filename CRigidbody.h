#pragma once
#include "CComponent.h"

//물리, 강체

class CRigidbody :
    public CComponent
{

public:
    CRigidbody(CObj* _pOwner);
    CRigidbody(const CRigidbody& _other);
    ~CRigidbody();

public:
    CLONE(CRigidbody);

private:
    float   m_fMass;            //질량
    Vec2    m_vVelocity;        //속도
    Vec2    m_vForce;           //가해지는 힘
    Vec2    m_vMaxVelocity;     //제한 속도

    //중력 관련 변수(플랫포머)
    bool    m_bGround;          //오브젝트가 땅 위에 있는가?
    float   m_fGravityAccel;    //중력 가속도
    float   m_fGravityVLimit;   // 중력에 의한 제한 속도
   
public:
    virtual void tick() override;
    virtual void final_tick() override;
    virtual void render(HDC _dc) override;

public:
    void AddForce(Vec2 _vForce) { m_vForce += _vForce; }
    void SetVelocityLimit(Vec2 _vMaxVelocity) { m_vMaxVelocity = _vMaxVelocity; }
    void SetGravityVelocityLimit(float _fLimit) { m_fGravityVLimit = _fLimit; }
    void SetVelocity(Vec2 _vVelocity) { m_vVelocity = _vVelocity; }
    void AddVelocity(Vec2 _vAddV) { m_vVelocity += _vAddV; }
    Vec2 GetVelocity() { return m_vVelocity; }
    void SetGravityAccel(float _fAccel) { m_fGravityAccel = _fAccel; }
    void SetGround(bool _b);
    bool IsGround() { return m_bGround; };
};

