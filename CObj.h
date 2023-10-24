#pragma once
#include "CEntity.h"

//오브젝트 클래스는 Entity를 상속받으며, 게임 내 모든 객체들의 부모가 되는 추상 클래스이다.

//물체의 렌더링 포지션을 구해오기 위해 카메라는 전방선언이 아닌 include를 한다
//#include "CCamera.h"

//Component 클래스들을 전방선언해준다.
class CCollider;
class CAnimator;
class CRigidbody;
class CFSM;

class CObj :
    public CEntity
{
    friend class CEventManager;     //EventManager가 오브젝트의 생성/소멸 처리를 할 수 있게 friend처리

public:
    CObj();
    CObj(const CObj& _other);  
    virtual ~CObj();

private:
    //객체들은 기본적으로 화면에 배치되어 렌더링되어야하므로 좌표와 크기 정보를 가지고 있어야한다.
    
    Vec2            m_vPos;     //x, y 좌표값을 가진 POINT 구조체
    Vec2            m_vScale;   //가로, 세로 길이 값을 가지는 POINT 구조체

    //오브젝트들은 기능을 가진 Component 들을 가질수도, 가지지 않을 수도 있다.
    CCollider*      m_pCollider;
    CAnimator*      m_pAnimator;
    CRigidbody*     m_pRigidbody;
    CFSM*           m_pFSM;

    bool            m_bDead;   //객체가 죽었는지 여부로, true가 되면 아직 메모리에서 삭제되지 않았더라도 죽은 취급을 받는다.
    bool            m_bDoNotDestroy;


public:
    void SetPos(Vec2 _v) { m_vPos = _v; }
    Vec2 GetPos() { return m_vPos; }

    void SetScale(Vec2 _v) { m_vScale = _v; }
    Vec2 GetScale() { return m_vScale; }

    //Component 함수
    void CreateCollider();
    void CreateAnimator();
    void CreateRigidbody();
    void CreateFSM();

    CCollider* GetCollider() { return m_pCollider; }
    CAnimator* GetAnimator() { return m_pAnimator; }
    CRigidbody* GetRigidbody() { return m_pRigidbody; }
    CFSM*       GetFSM()       { return m_pFSM; }

public:
    //CObj 함수는 실제 객체 생성용이 아닌 추상 클래스이다.
    virtual void tick();
    virtual void final_tick() final;
    virtual void render(HDC _dc);
    
    virtual CObj* Clone() = 0; //순수가상함수, 자식 클래스들은 Clone을 반드시 구현해야한다.

public:
    //충돌 함수
    virtual void BeginOverlap(CCollider* _pOther);
    virtual void OnOverlap(CCollider* _pOther);
    virtual void EndOverlap(CCollider* _pOther);

public:
    bool IsDead() { return m_bDead; }
    void SetDead();
    void SetDoNotDestroy(bool _b) { m_bDoNotDestroy = _b; };
    bool IsNotDestroy() { return m_bDoNotDestroy; };
};

