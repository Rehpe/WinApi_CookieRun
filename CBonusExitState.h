#include "CState.h"

class CBonusExitState :
    public CState
{
public:
    CBonusExitState();
    ~CBonusExitState();

public:
    CLONE(CBonusExitState);

private:

public:
    virtual void final_tick() override;
    virtual void Enter() override;
    virtual void Exit() override;
};
