#pragma once
#include "CEntity.h"

//������Ʈ Ŭ������ Entity�� ��ӹ�����, ���� �� ��� ��ü���� �θ� �Ǵ� �߻� Ŭ�����̴�.

//��ü�� ������ �������� ���ؿ��� ���� ī�޶�� ���漱���� �ƴ� include�� �Ѵ�
//#include "CCamera.h"

//Component Ŭ�������� ���漱�����ش�.
class CCollider;
class CAnimator;
class CRigidbody;
class CFSM;

class CObj :
    public CEntity
{
    friend class CEventManager;     //EventManager�� ������Ʈ�� ����/�Ҹ� ó���� �� �� �ְ� friendó��

public:
    CObj();
    CObj(const CObj& _other);  
    virtual ~CObj();

private:
    //��ü���� �⺻������ ȭ�鿡 ��ġ�Ǿ� �������Ǿ���ϹǷ� ��ǥ�� ũ�� ������ ������ �־���Ѵ�.
    
    Vec2            m_vPos;     //x, y ��ǥ���� ���� POINT ����ü
    Vec2            m_vScale;   //����, ���� ���� ���� ������ POINT ����ü

    //������Ʈ���� ����� ���� Component ���� ��������, ������ ���� ���� �ִ�.
    CCollider*      m_pCollider;
    CAnimator*      m_pAnimator;
    CRigidbody*     m_pRigidbody;
    CFSM*           m_pFSM;

    bool            m_bDead;   //��ü�� �׾����� ���η�, true�� �Ǹ� ���� �޸𸮿��� �������� �ʾҴ��� ���� ����� �޴´�.
    bool            m_bDoNotDestroy;


public:
    void SetPos(Vec2 _v) { m_vPos = _v; }
    Vec2 GetPos() { return m_vPos; }

    void SetScale(Vec2 _v) { m_vScale = _v; }
    Vec2 GetScale() { return m_vScale; }

    //Component �Լ�
    void CreateCollider();
    void CreateAnimator();
    void CreateRigidbody();
    void CreateFSM();

    CCollider* GetCollider() { return m_pCollider; }
    CAnimator* GetAnimator() { return m_pAnimator; }
    CRigidbody* GetRigidbody() { return m_pRigidbody; }
    CFSM*       GetFSM()       { return m_pFSM; }

public:
    //CObj �Լ��� ���� ��ü �������� �ƴ� �߻� Ŭ�����̴�.
    virtual void tick();
    virtual void final_tick() final;
    virtual void render(HDC _dc);
    
    virtual CObj* Clone() = 0; //���������Լ�, �ڽ� Ŭ�������� Clone�� �ݵ�� �����ؾ��Ѵ�.

public:
    //�浹 �Լ�
    virtual void BeginOverlap(CCollider* _pOther);
    virtual void OnOverlap(CCollider* _pOther);
    virtual void EndOverlap(CCollider* _pOther);

public:
    bool IsDead() { return m_bDead; }
    void SetDead();
    void SetDoNotDestroy(bool _b) { m_bDoNotDestroy = _b; };
    bool IsNotDestroy() { return m_bDoNotDestroy; };
};

