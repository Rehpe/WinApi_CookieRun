#pragma once
#include "CButtonUI.h"

typedef void(CEntity::* DOUBLEPARAMDELEGATE) (const wstring&, int);

class CPageBtn :
    public CButtonUI
{
public:
    CPageBtn();
    ~CPageBtn();

public:
    CLONE(CPageBtn);

private:
    wstring m_strRelativePath;
    int m_iStartCount;

    CEntity* m_pInst;
    DOUBLEPARAMDELEGATE    m_pDoubleParamDelegateFunc;

public:
    void render(HDC _dc);
    void SetPageDelegate(CEntity* _pInst, DOUBLEPARAMDELEGATE _Func)
    {
        m_pInst = _pInst;
        m_pDoubleParamDelegateFunc = _Func;
    }

    void SetRelativePath(const wstring& _strRelativePath) { m_strRelativePath = _strRelativePath; }
    void SetStartCount(int _iStartCount) { m_iStartCount = _iStartCount; }

public:
    virtual void MouseLbtnClicked();


};

