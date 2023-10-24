#pragma once
#include "CObj.h"

class CTexture;

class CUI :
    public CObj
{
    friend class CUIManager;

public:
    CUI();
    CUI(const CUI& _other);
    virtual ~CUI();

public:
    CLONE(CUI);

private:
    CUI*            m_pParentUI;            //�θ� UI
    vector<CUI*>    m_vecChildUI;           //�ڽ� UI
    Vec2            m_vFinalPos;

    CTexture*       m_pIdleTex;             //UI �⺻ �ؽ���


    bool            m_bLbtnDown;            //���� ���콺 ��ư�� �������ִ°�
    bool            m_bMouseOn;             //���� ���콺�����Ͱ� UI ���� �÷��� �ִ°�
    bool            m_bCameraAffected;      //UI�� ī�޶��� ������ �޴��� ����


public:
    virtual void tick() override;
    virtual void render(HDC _dc) override;

    void render_childUI(HDC _dc);

public:
    //�������̵� �� ��� �θ��Լ��� ȣ���� ��
    virtual void MouseLbtnDown() { m_bLbtnDown = true; };
    virtual void MouseLbtnClicked() {};
    virtual void MouseOn() {};

    bool IsMouseOn() { return m_bMouseOn; }
    bool IsLbtnDown() { return m_bLbtnDown; }
    void SetLbtnDown(bool _b) { m_bLbtnDown = _b; };

public:
    Vec2 GetFinalPos() { return m_vFinalPos; }
    void SetFinalPos(Vec2 _vPos) { m_vFinalPos = _vPos; };
    void SetIdleTex(CTexture* _pTex);
    CTexture* GetIdleTex() { return m_pIdleTex; }
    void SetCameraAffected(bool _bSet) { m_bCameraAffected = _bSet; }

    CUI* GetParentUI() { return m_pParentUI; }

    void AddChildUI(CUI* _pChildUI)
    {
        _pChildUI->m_pParentUI = this;
        m_vecChildUI.push_back(_pChildUI);
    }
    vector<CUI*>& GetChildUI() { return m_vecChildUI; }
    void ClearChildUI();

private:
    void MouseOnCheck();

};


