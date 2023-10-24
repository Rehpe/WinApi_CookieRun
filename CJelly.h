#pragma once
#include "CMapTile.h"

class CTexture;
class CSound;

enum class ITEM_TYPE
{
    NONE,
    ITEM_BEARJELLYPARTY,
    ITEM_BIGGER,
    ITEM_BONUS,
    ITEM_BOOSTER,
    ITEM_COINMAGIC,
    ITEM_MAGNET,
    ITEM_ICEBEARJELLYPARTY,
    ITEM_MINIPOTION,
    ITEM_BIGPOTION,
    JELLY,
    JELLY_YELLOWBEAR,
    JELLY_PINKBEAR,
    JELLY_ICEBEAR,
    JELLY_BIGBEAR,
    JELLY_RAINBOWBEAR,
    BONUS_RAINBOW,
    BONUS_GOLD,
    COIN_SILVER,
    COIN_GOLD,
    COIN_BIGSILVER,
    COIN_BIGGOLD,
    PETJELLY_MILK,
    PETJELLY_FAIRY,
    SKILLJELLY_SNOWSUGAR,
    PETJELLY_SNOWSUGAR,
};

class CJelly :
    public CMapTile
{
public:
    CJelly();
    ~CJelly();

public:
    CLONE(CJelly);

private:
    ITEM_TYPE   m_eItem;
    CTexture*   m_pEffectTex;
    CSound*     m_pJellySFX;        //���� ���� ���� ȿ����

    bool        m_bDefaultMagnet;   //�⺻ �ڷ��� �ִ� ����(�� ����, Ư�� ���� ��)�� true

public:
    void init(ITEM_TYPE eItem);
    void render(HDC _dc) override;

    void SetItemType(ITEM_TYPE _eItem) { m_eItem = _eItem; };
    ITEM_TYPE GetItemType() { return m_eItem; };

public:
    virtual void BeginOverlap(CCollider* _pOther) override;
    virtual void OnOverlap(CCollider* _pOther) override;
    virtual void EndOverlap(CCollider* _pOther) override;

private:
    void ItemEffect();
    void ItemVFX(float _fTime, bool _bReduceAlpha);
};

