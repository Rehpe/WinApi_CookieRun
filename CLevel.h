#pragma once
#include "CEntity.h"

//유니티의 Scene, 언리얼의 Level 과 같은 의미로 게임 무대의 한 막, 스테이지 등의 개념이다.
//모든 Level들의 부모 클래스이다.
class CPlayer;
class CPet;
class CPanelUI;

class CLevel :
    public CEntity
{
public:
    CLevel();
    virtual ~CLevel();      //자식 클래스의 소멸자가 호출될 수 있게 가상함수 처리한다.

public:
    //레벨은 CLONE이 필요 없고, 복사되어서도 안되므로 CLONE을 비활성화시킨다.
    CLONE_DEACTIVATE(CLevel);

private:
    //레벨에 배치되는 모든 오브젝트들은 레이어로 나뉘며, 각각의 레이어 벡터에 오브젝트들이 담겨있다.
    vector<CObj*>   m_arrLayer[(UINT)LAYER::END];

    //화면 내에 들어오는 오브젝트들을 담은 벡터
    vector<CObj*>   m_vecVisibleGroundObj;
    vector<CObj*>   m_vecVisibleObstacleObj;
    vector<CObj*>   m_vecVisibleJellyObj;


    //맵 타일의 가로 개수(X)와 세로 갯수(Y)
    UINT    m_iGridXCount;
    UINT    m_iGridYCount;

    CPlayer*    m_pPlayer;              //플레이어 쿠키
    CPet*       m_pPet;

    bool        m_bColliderRender;      //충돌체 사각형을 렌더할것인지
    bool        m_bPause;               //일시정지 상태인가?

    CPanelUI*   m_pPausePanel;


public:
    //CLevel은 추상클래스이므로 기본 함수들을 virtual 선언한다.
    virtual void init() = 0;
    virtual void tick();
    virtual void final_tick();
    virtual void render(HDC _dc);
    
    virtual void Enter() = 0;           //레벨 진입시 호출
    virtual void Exit() = 0;            //레벨 벗어날 시 호출

    void DeleteAllObject();             //레벨 내의 모든 오브젝트 삭제
    void DeleteObject(LAYER _eLayer);   //레벨 내 특정 레이어 오브젝트만 삭제
    void LevelDestroy();                //DoNotDestroy 객체 제외하고 레이어 전부 삭제

public:
    void AddObject(CObj* _pObj, LAYER _eLayer);     //생성한 오브젝트를 레벨 내의 레이어 벡터에 추가하는 함수

    //해당 레이어 벡터에 들어있는 모든 오브젝트 목록을 가져오는 함수
    //vector의 복사 비용을 줄이기 위해, 반환을 const &(레퍼런스) 타입으로 원본을 받아온다.
    vector<CObj*>& GetLayer(LAYER _eLayer) { return m_arrLayer[(UINT)_eLayer]; }
    vector<CObj*>& GetVisibleObstacleLayer() { return m_vecVisibleObstacleObj; }
    vector<CObj*>& GetVisibleJellyLayer() { return m_vecVisibleJellyObj; }

    void CreateGrid(UINT _X, UINT _Y);

    void SetFocusedUI(CObj* _pUI);

public:
    UINT GetGridXCount() { return m_iGridXCount; }
    UINT GetGridYCount() { return m_iGridYCount; }

    CPlayer* GetPlayer() { return m_pPlayer; };
    void SetPlayer(CPlayer* _pPlayer) { m_pPlayer = _pPlayer; };
    
    CPet* GetPet() { return m_pPet; }
    void SetPet(CPet* _pPet) { m_pPet = _pPet; };

    void LoadMap(const wstring& _strRelativePath);

    void SetColliderRender(bool _b) {      m_bColliderRender = _b;    };
    bool GetColliderRender() { return m_bColliderRender; };

    void SetPause(bool _b) { m_bPause = _b; };
    bool GetPause() { return m_bPause; };

    void Pause();
    void BreakPause();
    void StopGame();
};

