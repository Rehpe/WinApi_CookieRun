#include "CState.h"

class CBonusIdleState :
    public CState
{
public:
    CBonusIdleState();
    ~CBonusIdleState();

public:
    CLONE(CBonusIdleState);

private:

public:
    virtual void final_tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

};
