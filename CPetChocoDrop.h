#pragma once
#include "CPet.h"
class CPetChocoDrop :
    public CPet
{
public:
    CPetChocoDrop();
    ~CPetChocoDrop();

public:
    CLONE(CPetChocoDrop);

public:
    virtual void tick();
    virtual void render(HDC _dc);


};

