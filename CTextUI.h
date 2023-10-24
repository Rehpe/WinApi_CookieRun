#pragma once
#include "CUI.h"

enum class FONT_ALIGN
{
    LEFT,
    CENTER,
    RIGHT,
};

class CTextUI :
    public CUI
{
public:
    CTextUI();
    ~CTextUI();

public:
    CLONE(CTextUI);

private:
    int         m_iNumber;      //텍스트가 표시할 숫자
    int         m_iFontSize;    //폰트 크기
    FONT_ALIGN  m_eFontAlign;   //폰트 정렬


public:
    virtual void tick() override;
    virtual void render(HDC _dc) override;

public:
    void SetNumber(int _iNum) { m_iNumber = _iNum; };
    void SetFontSize(int _iSize) { m_iFontSize = _iSize; };
    void SetFontAlign(FONT_ALIGN _eAlign) { m_eFontAlign = _eAlign; };

private:
    wstring NumberComma(int _num);

};

