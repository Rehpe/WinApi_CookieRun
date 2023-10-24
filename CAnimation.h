#pragma once
#include "CEntity.h"

//애니메이션의 주인 애니메이터, 아틀라스 이미지, 프레임 정보, 반복재생 여부 등을 관리하는 클래스

class CTexture;
class CAnimator;

class CAnimation :
    public CEntity
{
    friend class CAnimator;     //애니메이터 클래스가 자유롭게 사용할 수 있게 friend 처리

public:
    CAnimation(CAnimator* _pAnimator);      //애니메이션은 반드시 본인을 관리할 애니메이터를 가진다
    ~CAnimation();

public:
    CLONE(CAnimation);

private:
    CAnimator*          m_pAnimator;    //자신을 소유하고 있는 Animator
    
    CTexture*           m_pAtlas;       //애니메이션이 사용할 배경 이미지(아틀라스)
    vector<tAnimFrm>    m_vecFrm;       //프레임 정보 구조체를 여러개 담고 있는 벡터(애니메이션은 여러 프레임으로 이루어진 배열이다)
    int                 m_iCurFrm;      //현재 애니메이션의 몇 번째 프레임을 재생중인지
    float               m_fAccTime;     //누적 시간
    bool                m_bFinish;      //애니메이션 재생이 완료되었는지 여부

public:
    void tick();
    void render(HDC _dc);

private:
    void init(const wstring& _strName, CTexture* _pAtlas, Vec2 _vLeftTop,
              Vec2 _vSize, Vec2 _vOffset, int _iMaxFrmCount, float _fDuration);

public:
    bool IsFinish() { return m_bFinish; }
    void Reset();
    void Save(const wstring& _strRelativePath);
    void Load(const wstring& _strRelativePath);

};

