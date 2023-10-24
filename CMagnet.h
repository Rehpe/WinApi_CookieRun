#pragma once
#include "CItem.h"
class CMagnet :
    public CItem
{
public:
    CMagnet();
    ~CMagnet();

public:
    CLONE(CMagnet);

public:
    void render(HDC _dc);

private:
    virtual void BeginOverlap(CCollider* _pOther);
    virtual void OnOverlap(CCollider* _pOther);
    virtual void EndOverlap(CCollider* _pOther);


};

