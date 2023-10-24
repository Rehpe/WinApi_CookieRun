#pragma once
#include "CUI.h"

class CEditorLevel;
class CTexture;

typedef void(*TestFuncType)(void);
typedef void(CEntity::* DELEGATE)(void);
typedef void(CEntity::* DOUBLEPARAMDELEGATE) (const wstring&, int);

class CButtonUI :
    public CUI
{
public:
    CButtonUI();
    virtual ~CButtonUI();

public:
    CLONE(CButtonUI);

private:
    CTexture*       m_pPressedTex;
    CTexture*       m_pHighlightTex;

    // Call Back
    TestFuncType    m_pFunc;

    // Delegate
    CEntity*        m_pInst;
    DELEGATE        m_pDelegateFunc;

    wstring         m_strRelativePath;
    int             m_iStartCount;

    DOUBLEPARAMDELEGATE    m_pDoubleParamDelegateFunc;

public:
    virtual void render(HDC _dc);

public:
    void SetCallBack(TestFuncType _pCallBack) { m_pFunc = _pCallBack; }
    void SetDelegate(CEntity* _pInst, DELEGATE _Func)
    {
        m_pInst = _pInst;
        m_pDelegateFunc = _Func;
    }
    void SetDoubleParamDelegate(CEntity* _pInst, DOUBLEPARAMDELEGATE _Func)
    {
        m_pInst = _pInst;
        m_pDoubleParamDelegateFunc = _Func;
    }
    void SetRelativePath(const wstring& _strRelativePath) { m_strRelativePath = _strRelativePath; }
    void SetStartCount(int _iStartCount) { m_iStartCount = _iStartCount; }
   

public:
    void SetHighlightTex(CTexture* _pTex) { m_pHighlightTex = _pTex; }
    void SetPressedTex(CTexture* _pTex) { m_pPressedTex = _pTex; }

public:
    virtual void MouseLbtnClicked();
};



