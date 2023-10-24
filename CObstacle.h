#pragma once
#include "CMapTile.h"

enum class OBS_TYPE
{
    FIXED,
    ANIM_1,
    ANIM_2,
    ANIM_3,
    ANIM_4,
    ANIM_5,
    ANIM_6,
    ANIM_7,
    ANIM_8,

};


class CTexture;

class CObstacle :
    public CMapTile
{
public:
    CObstacle();
    ~CObstacle();

public:
    CLONE(CObstacle);

private:
    OBS_TYPE m_eObsType;
    bool     m_bIsPassing;
    bool     m_bParabola;
    int      m_iLifeTime;

public:
    void init(OBS_TYPE _eObsType, bool _bIsPassing);
    void tick();
    void render(HDC _dc);

public:
    void SetObsType(OBS_TYPE _obsType) { m_eObsType = _obsType; };
    OBS_TYPE GetObsType() { return m_eObsType; }

    void SetIsPassing(bool _b) { m_bIsPassing = _b; };
    bool GetIsPassing() { return m_bIsPassing; };

    void SetParabola(bool _b) { m_bParabola = _b; }
    bool IsParabola() { return m_bParabola; };

    void Parabola();

private:
    virtual void BeginOverlap(CCollider* _pOther);
    virtual void OnOverlap(CCollider* _pOther);
    virtual void EndOverlap(CCollider* _pOther);

};

