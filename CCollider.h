#pragma once
#include "CComponent.h"

//������Ʈ���� �浹 ����

class CCollider :
    public CComponent
{
public:
    //�浹ü�� Ư���� ������ ���� ��ü�� ������ �ϹǷ�, �����ڿ��� ������ ��ü�� �޾ƾ��Ѵ�.
    CCollider(CObj* _pOwner);
    CCollider(const CCollider& _other);     //���������
    ~CCollider();

public:
    CLONE(CCollider);

private:
    //�⺻������ �浹ü�� �ڽ��� ������ ��ü�� ��ġ�� ����ٴϴµ�,
    //������ ��ġ ������ ���� ��ü�� ��ŭ �������ִ��� ���� ��ġ�� ������ �� �ִ�.

    Vec2    m_vOffsetPos;       //���� ��ü�� �߽����� ��ŭ �������ִ��� (������)
    Vec2    m_vFinalPos;        //���� ��ü�� �߽��� ��ġ�� �����°��� ���� �浹ü�� ���� ��ġ (������) 

    Vec2    m_vScale;           //�浹ü�� ũ��
    Vec2    m_vOffsetScale;     //�浹ü ũ�� ������
    Vec2    m_vFinalScale;      //�浹ü ���� ũ��

    int     m_iOverlapCount;    //�ٸ� �浹ü�� �浹������, �� ���� �浹������ �� �� �ִ� ��

    float   m_fSizeRatio;       //�浹ü ũ�� ����

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
    //�浹�� ȣ��Ǵ� �浹���� �Լ���

    void BeginOverlap(CCollider* _pOther);  //�浹 ���۽� �� �� ȣ��
    void OnOverlap(CCollider* _pOther);     //��� �浹���� �� ���������� ȣ��
    void EndOverlap(CCollider* _pOther);    //�浹�� ���� �� ������ �� �� �� ȣ��

};

