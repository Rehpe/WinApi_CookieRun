#pragma once
#include "CEntity.h"

//�ִϸ��̼��� ���� �ִϸ�����, ��Ʋ�� �̹���, ������ ����, �ݺ���� ���� ���� �����ϴ� Ŭ����

class CTexture;
class CAnimator;

class CAnimation :
    public CEntity
{
    friend class CAnimator;     //�ִϸ����� Ŭ������ �����Ӱ� ����� �� �ְ� friend ó��

public:
    CAnimation(CAnimator* _pAnimator);      //�ִϸ��̼��� �ݵ�� ������ ������ �ִϸ����͸� ������
    ~CAnimation();

public:
    CLONE(CAnimation);

private:
    CAnimator*          m_pAnimator;    //�ڽ��� �����ϰ� �ִ� Animator
    
    CTexture*           m_pAtlas;       //�ִϸ��̼��� ����� ��� �̹���(��Ʋ��)
    vector<tAnimFrm>    m_vecFrm;       //������ ���� ����ü�� ������ ��� �ִ� ����(�ִϸ��̼��� ���� ���������� �̷���� �迭�̴�)
    int                 m_iCurFrm;      //���� �ִϸ��̼��� �� ��° �������� ���������
    float               m_fAccTime;     //���� �ð�
    bool                m_bFinish;      //�ִϸ��̼� ����� �Ϸ�Ǿ����� ����

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

