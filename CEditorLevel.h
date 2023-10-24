#pragma once
#include "CLevel.h"

//맵 제작 툴
class CPreview;
class CGrid;
class CPanelUI;
class CButtonUI;
class CPageBtn;

enum class EDITOR_MODE
{
    TILE,
    OBJECT,

    NONE,
};

class CEditorLevel :
    public CLevel
{
public:
    CEditorLevel();
    ~CEditorLevel();

    friend class  CLevel;

private:
    EDITOR_MODE         m_eMode;
    CPreview*           m_pPreview;
    vector<tBtnInfo>    m_vecBtnInfo;
    vector<CGrid*>      m_vecGrid;
    list<CObj*>        m_WorkList;

    //====UI PANEL====
    CPanelUI*   m_pPanelUI;
    CPanelUI*   m_pBlankPanelUI;
    CButtonUI*  m_pPage1Btn;
    CButtonUI*  m_pPage2Btn;
    CButtonUI*  m_pPage3Btn;
    CButtonUI*  m_pPage4Btn;
    CButtonUI*  m_pPage5Btn;

    CButtonUI* m_pBackBtn;

    //====Create모드에서 Instantiate할 정보 저장=====
    CLASS_TYPE  m_eCreateClassType;


public:
    virtual void init() override;
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    void SetClassType(CLASS_TYPE _eClass) { m_eCreateClassType = _eClass; };
    CLASS_TYPE GetClassType() { return m_eCreateClassType; };

    void SetPreview(CPreview* _pPreview) { m_pPreview = _pPreview; };
    CPreview* GetPreview() { return m_pPreview; };
    //void DeletePreview() { if (m_pPreview != nullptr)  delete m_pPreview;  m_pPreview = nullptr; };

private:
    void object_update();
    void LoadBtnInfo(const wstring& _strRelativePath);
    void CreateMapTileBtn(const wstring& _strRelativePath, int _iStartCount);
    void SaveMap();
    void LoadMap();
    void CreatePlayer();

};

