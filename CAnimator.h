#pragma once
#include "CComponent.h"

//게임 내의 애니메이션들을 저장 및 관리, 재생시켜주는 관리자

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
    map<wstring, CAnimation*>   m_mapAnim;      //애니메이션들을 키값으로 관리하는 map 자료형
    CAnimation*                 m_pCurAnim;     //현재 애니메이션
    bool                        m_bRepeat;      //애니메이션 반복재생 (Loop) 여부
    float                       m_fRatio;       //애니메이션 투명도를 결정할 Ratio
    float                       m_fSizeRatio;   //애니메이션 크기를 결정할 Ratio
    Vec2                        m_vOffset;    //오프셋


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

