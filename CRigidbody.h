#pragma once
#include "CComponent.h"

//����, ��ü

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
    float   m_fMass;            //����
    Vec2    m_vVelocity;        //�ӵ�
    Vec2    m_vForce;           //�������� ��
    Vec2    m_vMaxVelocity;     //���� �ӵ�

    //�߷� ���� ����(�÷�����)
    bool    m_bGround;          //������Ʈ�� �� ���� �ִ°�?
    float   m_fGravityAccel;    //�߷� ���ӵ�
    float   m_fGravityVLimit;   // �߷¿� ���� ���� �ӵ�
   
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

