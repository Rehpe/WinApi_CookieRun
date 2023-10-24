#pragma once
#include "CButtonUI.h"
class CMapTileBtn :
    public CButtonUI
{
public:
    CMapTileBtn();
    ~CMapTileBtn();

public:
    CLONE(CMapTileBtn);

private:
    //wstring   m_strKey;
    //wstring   m_strRelativePath;
    CLASS_TYPE m_eClass;

public:
    void render(HDC _dc);

public:
    //void SetKey(const wstring _strKey) { m_strKey = _strKey; };
    //void SetRelativePath(const wstring _strRelativePath) { m_strRelativePath = _strRelativePath; };

    //const wstring GetKey() { return m_strKey; };
    //const wstring GetRelativePath() { return m_strRelativePath; };

    void SetClassType(int iClass) { m_eClass = (CLASS_TYPE)iClass; };
    CLASS_TYPE GetClassType() { return m_eClass; };

public:
    void selectObject();
};

