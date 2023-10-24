#pragma once
#include "CComponent.h"

//���� ���� �ִϸ��̼ǵ��� ���� �� ����, ��������ִ� ������

class CAnimation;
class CTexture;

class CAnimator :
    public CComponent
{
public:
    CAnimator(CObj* _pOwner);
    CAnimator(const CAnimator& _other);
    ~CAnimator();

public:
    CLONE(CAnimator);

private:
    map<wstring, CAnimation*>   m_mapAnim;      //�ִϸ��̼ǵ��� Ű������ �����ϴ� map �ڷ���
    CAnimation*                 m_pCurAnim;     //���� �ִϸ��̼�
    bool                        m_bRepeat;      //�ִϸ��̼� �ݺ���� (Loop) ����
    float                       m_fRatio;       //�ִϸ��̼� ������ ������ Ratio
    float                       m_fSizeRatio;   //�ִϸ��̼� ũ�⸦ ������ Ratio
    Vec2                        m_vOffset;    //������


public:
    virtual void tick() override;
    virtual void final_tick() override;
    virtual void render(HDC _dc) override;

public:
    CAnimation* FindAnimation(const wstring& _strName);
    CAnimation* LoadAnimation(const wstring& _strRelativePath);

    void CreateAnimation(const wstring& _strName, CTexture* _pAtlas, Vec2 _vLeftTop, Vec2 _vSize, Vec2 _vOffset, int _iMaxFrmCount, float _fDuration);

    void Play(const wstring& _strName, bool _bRepeat);

    CAnimation* GetCurAnimation() { return m_pCurAnim; };

    void SetRatio(float _f) { m_fRatio = _f; };
    float GetRatio() { return m_fRatio; }

    void SetSizeRatio(float _f) { m_fSizeRatio = _f; };
    float GetSizeRatio() { return m_fSizeRatio; }

    void SetOffset(Vec2 _vOffset) { m_vOffset = _vOffset; };
    Vec2 GetOffset() { return m_vOffset; };



};

