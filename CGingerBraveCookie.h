#pragma once
#include "CPlayer.h"


class CGingerBraveCookie :
    public CPlayer
{

public:
    CGingerBraveCookie();
    ~CGingerBraveCookie();

public:
    CLONE(CGingerBraveCookie);


public:
    void init();
    virtual void tick() override;
    virtual void render(HDC _dc) override;
    virtual void BeginOverlap(CCollider* _pOther) override;
    virtual void OnOverlap(CCollider* _pOther) override;
    virtual void EndOverlap(CCollider* _pOther) override;
};

