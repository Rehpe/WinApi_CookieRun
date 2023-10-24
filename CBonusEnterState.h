#include "CState.h"

class CBonusEnterState :
    public CState
{
public:
    CBonusEnterState();
    ~CBonusEnterState();

public:
    CLONE(CBonusEnterState);

private:

public:
    virtual void final_tick() override;
    virtual void Enter() override;
    virtual void Exit() override;
};
