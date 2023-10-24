#pragma once
#include "CObj.h"

class CTexture;

class CMapTile :
    public CObj
{
public:
    CMapTile();
    virtual ~CMapTile();

public:
    CLONE(CMapTile);

protected:
    CTexture* m_pTexture;

    bool      m_bMouseOn;

public:
    void init();
    void tick();
    void render(HDC _dc);

public:
    void SetTexture(CTexture* _pTexture) { m_pTexture = _pTexture; };
    CTexture* GetTexture() { return m_pTexture; };

private:
    virtual void BeginOverlap(CCollider* _pOther);
    virtual void OnOverlap(CCollider* _pOther);
    virtual void EndOverlap(CCollider* _pOther);

public:
    void MouseOnCheck();
};

