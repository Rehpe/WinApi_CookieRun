#pragma once
#include "CEntity.h"

//����Ƽ�� Scene, �𸮾��� Level �� ���� �ǹ̷� ���� ������ �� ��, �������� ���� �����̴�.
//��� Level���� �θ� Ŭ�����̴�.
class CPlayer;
class CPet;
class CPanelUI;

class CLevel :
    public CEntity
{
public:
    CLevel();
    virtual ~CLevel();      //�ڽ� Ŭ������ �Ҹ��ڰ� ȣ��� �� �ְ� �����Լ� ó���Ѵ�.

public:
    //������ CLONE�� �ʿ� ����, ����Ǿ�� �ȵǹǷ� CLONE�� ��Ȱ��ȭ��Ų��.
    CLONE_DEACTIVATE(CLevel);

private:
    //������ ��ġ�Ǵ� ��� ������Ʈ���� ���̾�� ������, ������ ���̾� ���Ϳ� ������Ʈ���� ����ִ�.
    vector<CObj*>   m_arrLayer[(UINT)LAYER::END];

    //ȭ�� ���� ������ ������Ʈ���� ���� ����
    vector<CObj*>   m_vecVisibleGroundObj;
    vector<CObj*>   m_vecVisibleObstacleObj;
    vector<CObj*>   m_vecVisibleJellyObj;


    //�� Ÿ���� ���� ����(X)�� ���� ����(Y)
    UINT    m_iGridXCount;
    UINT    m_iGridYCount;

    CPlayer*    m_pPlayer;              //�÷��̾� ��Ű
    CPet*       m_pPet;

    bool        m_bColliderRender;      //�浹ü �簢���� �����Ұ�����
    bool        m_bPause;               //�Ͻ����� �����ΰ�?

    CPanelUI*   m_pPausePanel;


public:
    //CLevel�� �߻�Ŭ�����̹Ƿ� �⺻ �Լ����� virtual �����Ѵ�.
    virtual void init() = 0;
    virtual void tick();
    virtual void final_tick();
    virtual void render(HDC _dc);
    
    virtual void Enter() = 0;           //���� ���Խ� ȣ��
    virtual void Exit() = 0;            //���� ��� �� ȣ��

    void DeleteAllObject();             //���� ���� ��� ������Ʈ ����
    void DeleteObject(LAYER _eLayer);   //���� �� Ư�� ���̾� ������Ʈ�� ����
    void LevelDestroy();                //DoNotDestroy ��ü �����ϰ� ���̾� ���� ����

public:
    void AddObject(CObj* _pObj, LAYER _eLayer);     //������ ������Ʈ�� ���� ���� ���̾� ���Ϳ� �߰��ϴ� �Լ�

    //�ش� ���̾� ���Ϳ� ����ִ� ��� ������Ʈ ����� �������� �Լ�
    //vector�� ���� ����� ���̱� ����, ��ȯ�� const &(���۷���) Ÿ������ ������ �޾ƿ´�.
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

