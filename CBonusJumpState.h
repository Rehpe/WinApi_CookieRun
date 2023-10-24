
#include "CState.h"
class CBonusJumpState :
    public CState
{
public:
    CBonusJumpState();
    ~CBonusJumpState();

public:
    CLONE(CBonusJumpState);

private:

public:
    virtual void final_tick() override;
    virtual void Enter() override;
    virtual void Exit() override;
};
