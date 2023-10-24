#pragma once
#include "CObj.h"
class CTexture;

class CBackground :
    public CObj
{
public:
    CBackground();
    ~CBackground();

public:
    CLONE(CBackground);

public:
    CTexture*   m_pBG1Tex;
    CTexture*   m_pBG2Tex;
    CTexture*   m_pBG3Tex;
    CTexture*   m_pBG4Tex;

private:
    Vec2 m_vBG1Pos;
    Vec2 m_vBG2Pos;
    Vec2 m_vBG3Pos;
    Vec2 m_vBG4Pos;



public:
    void init();
    void tick();
    void render(HDC _dc);

private:
    void BG1Loop(HDC _dc);
    void BG2Loop(HDC _dc);
    void BG3Loop(HDC _dc);
    void BG4Loop(HDC _dc);
};

