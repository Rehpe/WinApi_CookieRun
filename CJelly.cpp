#include "pch.h"
#include "CJelly.h"
#include "CTexture.h"
#include "CResourceManager.h"
#include "CAnimation.h"
#include "CAnimator.h"
#include "CCollider.h"
#include "CCamera.h"
#include "CPlayer.h"
#include "CLevelManager.h"
#include "CLevel.h"
#include "CObstacle.h"
#include "CPet.h"
#include "CTimeManager.h"
#include "CEffect.h"
#include "CSound.h"
#include "CPetFlowerPod.h"
#include "CSnowSugarCookie.h"

CJelly::CJelly()
	: m_eItem(ITEM_TYPE::NONE)
	, m_pEffectTex(nullptr)
	, m_pJellySFX(nullptr)
	, m_bDefaultMagnet(false)
{
	//�⺻ ������ ��Ʋ�� ����
	m_pTexture = CResourceManager::GetInst()->LoadTexture(L"items", L"texture\\jelly\\items.png");

}

CJelly::~CJelly()
{
}

void CJelly::init(ITEM_TYPE m_eItem)
{
	switch (m_eItem)
	{
	case ITEM_TYPE::JELLY:
	{
		SetItemType(ITEM_TYPE::JELLY);
		m_pTexture = CResourceManager::GetInst()->LoadTexture(L"Commonjelly", L"texture\\jelly\\jelly.png");
		CMapTile::init();

		m_pEffectTex = CResourceManager::GetInst()->LoadTexture(L"whiteVFX", L"texture\\VFX\\whiteVFX.png");
		m_pJellySFX = CResourceManager::GetInst()->LoadSound(L"SFX_GetJelly", L"sound\\Jelly\\SFX_GetJelly.wav");
	}
	break;
	case ITEM_TYPE::ITEM_BEARJELLYPARTY:
	{
		//�ִϸ����� ����
		CreateAnimator();
		SetScale(Vec2(90.f, 90.f));

		SetItemType(ITEM_TYPE::ITEM_BEARJELLYPARTY);
		GetAnimator()->LoadAnimation(L"animation\\jelly\\Item_BearJellyParty.anim");
		GetAnimator()->Play(L"BearJellyParty", true);

		m_pEffectTex = CResourceManager::GetInst()->LoadTexture(L"TxtEffect_BearJellyParty", L"texture\\UI\\TxtEffect_BearJellyParty.png");
		m_pJellySFX = CResourceManager::GetInst()->LoadSound(L"SFX_GetItemJelly", L"sound\\Jelly\\SFX_GetItemJelly.wav");
	}
		break;
	case ITEM_TYPE::ITEM_BIGGER:
	{
		//�ִϸ����� ����
		CreateAnimator();
		SetScale(Vec2(90.f, 90.f));

		SetItemType(ITEM_TYPE::ITEM_BIGGER);
		GetAnimator()->LoadAnimation(L"animation\\jelly\\Item_Bigger.anim");
		GetAnimator()->Play(L"Bigger", true);
		m_pEffectTex = CResourceManager::GetInst()->LoadTexture(L"TxtEffect_Bigger", L"texture\\UI\\TxtEffect_Bigger.png");
		m_pJellySFX = CResourceManager::GetInst()->LoadSound(L"SFX_GetItemJelly", L"sound\\Jelly\\SFX_GetItemJelly.wav");
	}
		break;
	case ITEM_TYPE::ITEM_BONUS:
	{
		//�ִϸ����� ����
		CreateAnimator();
		SetScale(Vec2(90.f, 90.f));
		SetItemType(ITEM_TYPE::ITEM_BONUS);
		GetAnimator()->LoadAnimation(L"animation\\jelly\\Item_Bonus.anim");
		GetAnimator()->Play(L"Bonus", true);

		m_pEffectTex = CResourceManager::GetInst()->LoadTexture(L"TxtEffect_Bonus", L"texture\\UI\\TxtEffect_Bonus.png");
		m_pJellySFX = CResourceManager::GetInst()->LoadSound(L"SFX_GetItemJelly", L"sound\\Jelly\\SFX_GetItemJelly.wav");
	}
		break;
	case ITEM_TYPE::ITEM_BOOSTER:
	{
		//�ִϸ����� ����
		CreateAnimator();
		SetScale(Vec2(90.f, 90.f));
		SetItemType(ITEM_TYPE::ITEM_BOOSTER);
		GetAnimator()->LoadAnimation(L"animation\\jelly\\Item_Booster.anim");
		GetAnimator()->Play(L"Booster", true);

		m_pEffectTex = CResourceManager::GetInst()->LoadTexture(L"TxtEffect_Boost", L"texture\\UI\\TxtEffect_Boost.png");
		m_pJellySFX = CResourceManager::GetInst()->LoadSound(L"SFX_GetItemJelly", L"sound\\Jelly\\SFX_GetItemJelly.wav");
	}
		break;
	case ITEM_TYPE::ITEM_COINMAGIC:
	{
		//�ִϸ����� ����
		CreateAnimator();
		SetScale(Vec2(90.f, 90.f));
		SetItemType(ITEM_TYPE::ITEM_COINMAGIC);
		GetAnimator()->LoadAnimation(L"animation\\jelly\\Item_CoinMagic.anim");
		GetAnimator()->Play(L"CoinMagic", true);

		m_pEffectTex = CResourceManager::GetInst()->LoadTexture(L"TxtEffect_CoinMagic", L"texture\\UI\\TxtEffect_CoinMagic.png");
		m_pJellySFX = CResourceManager::GetInst()->LoadSound(L"SFX_CoinMagic", L"sound\\Jelly\\SFX_CoinMagic.wav");
	}
		break;
	case ITEM_TYPE::ITEM_MAGNET:
	{
		//�ִϸ����� ����
		CreateAnimator();
		SetScale(Vec2(90.f, 90.f));
		SetItemType(ITEM_TYPE::ITEM_MAGNET);
		GetAnimator()->LoadAnimation(L"animation\\jelly\\Item_Magnet.anim");
		GetAnimator()->Play(L"Magnet", true);

		m_pEffectTex = CResourceManager::GetInst()->LoadTexture(L"TxtEffect_Magnet", L"texture\\UI\\TxtEffect_Magnet.png");
		m_pJellySFX = CResourceManager::GetInst()->LoadSound(L"SFX_GetItemJelly", L"sound\\Jelly\\SFX_GetItemJelly.wav");
	}
		break;
	case ITEM_TYPE::ITEM_ICEBEARJELLYPARTY:
	{
		//�ִϸ����� ����
		CreateAnimator();
		SetScale(Vec2(90.f, 90.f));
		SetItemType(ITEM_TYPE::ITEM_ICEBEARJELLYPARTY);
		GetAnimator()->LoadAnimation(L"animation\\jelly\\Item_IceBearJellyParty.anim");
		GetAnimator()->Play(L"IceBearJellyParty", true);

		m_pEffectTex = CResourceManager::GetInst()->LoadTexture(L"TxtEffect_BearJellyParty", L"texture\\UI\\TxtEffect_BearJellyParty.png");
		m_pJellySFX = CResourceManager::GetInst()->LoadSound(L"SFX_GetItemJelly", L"sound\\Jelly\\SFX_GetItemJelly.wav");
	}
		break;
	case ITEM_TYPE::ITEM_MINIPOTION:
	{
		//�ִϸ����� ����
		CreateAnimator();
		SetScale(Vec2(90.f, 90.f));
		SetItemType(ITEM_TYPE::ITEM_MINIPOTION);
		GetAnimator()->LoadAnimation(L"animation\\jelly\\Item_MiniPotion.anim");
		GetAnimator()->FindAnimation(L"MiniPotion")->Save(L"animation\\Item_MiniPotion.anim");
		GetAnimator()->Play(L"MiniPotion", true);

		m_pEffectTex = CResourceManager::GetInst()->LoadTexture(L"TxtEffect_HpPotion", L"texture\\UI\\TxtEffect_HpPotion.png");
		m_pJellySFX = CResourceManager::GetInst()->LoadSound(L"SFX_BigHp", L"sound\\Jelly\\SFX_BigHp.wav");
	}
		break;
	case ITEM_TYPE::ITEM_BIGPOTION:
	{
		//�ִϸ����� ����
		CreateAnimator();
		SetScale(Vec2(144.f, 144.f));
		SetItemType(ITEM_TYPE::ITEM_BIGPOTION);
		GetAnimator()->LoadAnimation(L"animation\\jelly\\Item_BigPotion.anim");
		GetAnimator()->Play(L"BigPotion", true);

		m_pEffectTex = CResourceManager::GetInst()->LoadTexture(L"TxtEffect_HpPotion", L"texture\\UI\\TxtEffect_HpPotion.png");
		m_pJellySFX = CResourceManager::GetInst()->LoadSound(L"SFX_BigHp", L"sound\\Jelly\\SFX_BigHp.wav");
	}
		break;
	case ITEM_TYPE::JELLY_YELLOWBEAR:
	{
		SetItemType(ITEM_TYPE::JELLY_YELLOWBEAR);
		SetScale(Vec2(55.f, 51.f));
		m_pTexture = CResourceManager::GetInst()->LoadTexture(L"YellowBearJelly", L"texture\\jelly\\YellowBear.png");
		CMapTile::init();

		m_pEffectTex = CResourceManager::GetInst()->LoadTexture(L"yellowVFX", L"texture\\VFX\\yellowVFX.png");
		m_pJellySFX = CResourceManager::GetInst()->LoadSound(L"SFX_GetBigBearJelly", L"sound\\Jelly\\SFX_GetBigBearJelly.wav");
	}
	break;
	case ITEM_TYPE::JELLY_PINKBEAR:
	{
		SetItemType(ITEM_TYPE::JELLY_PINKBEAR);
		SetScale(Vec2(55.f, 51.f));
		m_pTexture = CResourceManager::GetInst()->LoadTexture(L"PinkBearJelly", L"texture\\jelly\\PinkBear.png");
		CMapTile::init();

		m_pEffectTex = CResourceManager::GetInst()->LoadTexture(L"pinkVFX", L"texture\\VFX\\pinkVFX.png");
		m_pJellySFX = CResourceManager::GetInst()->LoadSound(L"SFX_GetBigBearJelly", L"sound\\Jelly\\SFX_GetBigBearJelly.wav");
	}
	break;
	case ITEM_TYPE::JELLY_ICEBEAR:
	{
		SetItemType(ITEM_TYPE::JELLY_ICEBEAR);
		SetScale(Vec2(55.f, 51.f));
		m_pTexture = CResourceManager::GetInst()->LoadTexture(L"IceBearJelly", L"texture\\jelly\\IceBear.png");
		CMapTile::init();

		m_pEffectTex = CResourceManager::GetInst()->LoadTexture(L"blueVFX", L"texture\\VFX\\blueVFX.png");
		m_pJellySFX = CResourceManager::GetInst()->LoadSound(L"SFX_GetBigBearJelly", L"sound\\Jelly\\SFX_GetBigBearJelly.wav");
	}
	break;
	case ITEM_TYPE::JELLY_BIGBEAR:
	{
		SetItemType(ITEM_TYPE::JELLY_BIGBEAR);
		SetScale(Vec2(144.f, 138.f));
		m_pTexture = CResourceManager::GetInst()->LoadTexture(L"BigBearJelly", L"texture\\jelly\\BigBear.png");
		CMapTile::init();

		m_pEffectTex = CResourceManager::GetInst()->LoadTexture(L"yellowVFX", L"texture\\VFX\\yellowVFX.png");
		m_pJellySFX = CResourceManager::GetInst()->LoadSound(L"SFX_GetBigBearJelly", L"sound\\Jelly\\SFX_GetBigBearJelly.wav");
	}
	break;
	case ITEM_TYPE::JELLY_RAINBOWBEAR:
	{
		//�ִϸ����� ����
		CreateAnimator();
		SetScale(Vec2(176.f, 126.f));
		SetItemType(ITEM_TYPE::JELLY_RAINBOWBEAR);
		GetAnimator()->LoadAnimation(L"animation\\jelly\\Item_RainbowBear.anim");
		GetAnimator()->Play(L"RainbowBear", true);

		m_pEffectTex = CResourceManager::GetInst()->LoadTexture(L"purpleVFX", L"texture\\VFX\\purpleVFX.png");
		m_pJellySFX = CResourceManager::GetInst()->LoadSound(L"SFX_GetBigBearJelly", L"sound\\Jelly\\SFX_GetBigBearJelly.wav");
	}
	break;
	case ITEM_TYPE::BONUS_RAINBOW:
	{
		//�ִϸ����� ����
		CreateAnimator();
		SetScale(Vec2(120.f, 140.f));
		SetItemType(ITEM_TYPE::BONUS_RAINBOW);
		GetAnimator()->LoadAnimation(L"animation\\jelly\\Item_BonusRainbowStar.anim");
		GetAnimator()->Play(L"BonusRainbowStar", true);

		m_pEffectTex = CResourceManager::GetInst()->LoadTexture(L"purpleVFX", L"texture\\VFX\\purpleVFX.png");
		m_pJellySFX = CResourceManager::GetInst()->LoadSound(L"SFX_GetBigBearJelly", L"sound\\Jelly\\SFX_GetBigBearJelly.wav");
	}
	break;
	case ITEM_TYPE::BONUS_GOLD:
	{
		//�ִϸ����� ����
		CreateAnimator();
		SetScale(Vec2(120.f, 140.f));
		SetItemType(ITEM_TYPE::BONUS_GOLD);
		GetAnimator()->LoadAnimation(L"animation\\jelly\\Item_BonusGoldStar.anim");
		GetAnimator()->Play(L"BonusGoldStar", true);

		m_pEffectTex = CResourceManager::GetInst()->LoadTexture(L"yellowVFX", L"texture\\VFX\\yellowVFX.png");
		m_pJellySFX = CResourceManager::GetInst()->LoadSound(L"SFX_GetBigBearJelly", L"sound\\Jelly\\SFX_GetBigBearJelly.wav");
	}
	break;
	case ITEM_TYPE::COIN_SILVER:
	{
		//�ִϸ����� ����
		CreateAnimator();
		SetScale(Vec2(48.f, 48.f));
		SetItemType(ITEM_TYPE::COIN_SILVER);
		GetAnimator()->LoadAnimation(L"animation\\jelly\\Item_SilverCoin.anim");
		GetAnimator()->Play(L"SilverCoin", true);

		m_pEffectTex = CResourceManager::GetInst()->LoadTexture(L"whiteVFX", L"texture\\VFX\\whiteVFX.png");
		m_pJellySFX = CResourceManager::GetInst()->LoadSound(L"SFX_GetCoinJelly", L"sound\\Jelly\\SFX_GetCoinJelly.wav");
	}
	break;
	case ITEM_TYPE::COIN_GOLD:
	{
		//�ִϸ����� ����
		CreateAnimator();
		SetScale(Vec2(58.f, 59.f));
		SetItemType(ITEM_TYPE::COIN_GOLD);
		GetAnimator()->LoadAnimation(L"animation\\jelly\\Item_GoldCoin.anim");
		GetAnimator()->Play(L"GoldCoin", true);

		m_pEffectTex = CResourceManager::GetInst()->LoadTexture(L"yellowVFX", L"texture\\VFX\\yellowVFX.png");
		m_pJellySFX = CResourceManager::GetInst()->LoadSound(L"SFX_GetGoldJelly", L"sound\\Jelly\\SFX_GetGoldJelly.wav");
	}
	break;
	case ITEM_TYPE::COIN_BIGSILVER:
	{
		//�ִϸ����� ����
		CreateAnimator();
		SetScale(Vec2(160.f, 161.f));
		SetItemType(ITEM_TYPE::COIN_BIGSILVER);
		GetAnimator()->LoadAnimation(L"animation\\jelly\\Item_BigSilverCoin.anim");
		GetAnimator()->Play(L"BigSilverCoin", true);

		m_pEffectTex = CResourceManager::GetInst()->LoadTexture(L"whiteVFX", L"texture\\VFX\\whiteVFX.png");
		m_pJellySFX = CResourceManager::GetInst()->LoadSound(L"SFX_GetCoinJelly", L"sound\\Jelly\\SFX_GetCoinJelly.wav");
	}
	break;
	case ITEM_TYPE::COIN_BIGGOLD:
	{
		//�ִϸ����� ����
		CreateAnimator();
		SetScale(Vec2(160.f, 161.f));
		SetItemType(ITEM_TYPE::COIN_BIGGOLD);
		GetAnimator()->LoadAnimation(L"animation\\jelly\\Item_BigGoldCoin.anim");
		GetAnimator()->Play(L"BigGoldCoin", true);

		m_pEffectTex = CResourceManager::GetInst()->LoadTexture(L"yellowVFX", L"texture\\VFX\\yellowVFX.png");
		m_pJellySFX = CResourceManager::GetInst()->LoadSound(L"SFX_GetGoldJelly", L"sound\\Jelly\\SFX_GetGoldJelly.wav");
	}
	break;
	case ITEM_TYPE::PETJELLY_MILK:
	{
		//�ִϸ����� ����
		CreateAnimator();
		SetScale(Vec2(50.f, 50.f));
		SetItemType(ITEM_TYPE::PETJELLY_MILK);
		GetAnimator()->LoadAnimation(L"animation\\jelly\\Item_PetJelly_Milk.anim");
		GetAnimator()->Play(L"PetJelly_Milk", true);
		m_bDefaultMagnet = true;

		m_pEffectTex = CResourceManager::GetInst()->LoadTexture(L"whiteVFX", L"texture\\VFX\\whiteVFX.png");
		m_pJellySFX = CResourceManager::GetInst()->LoadSound(L"SFX_GetJelly", L"sound\\Jelly\\SFX_GetJelly.wav");
	}
	break;
	case ITEM_TYPE::PETJELLY_FAIRY:
	{
		SetItemType(ITEM_TYPE::PETJELLY_FAIRY);
		SetScale(Vec2(70.f, 70.f));
		m_pTexture = CResourceManager::GetInst()->LoadTexture(L"PetJelly_Fairy", L"texture\\jelly\\PetJelly_Fairy.png");
		m_bDefaultMagnet = true;
		CMapTile::init();

		m_pEffectTex = CResourceManager::GetInst()->LoadTexture(L"whiteVFX", L"texture\\VFX\\whiteVFX.png");
		m_pJellySFX = CResourceManager::GetInst()->LoadSound(L"SFX_GetJelly", L"sound\\Jelly\\SFX_GetJelly.wav");
	}
	break;
	case ITEM_TYPE::SKILLJELLY_SNOWSUGAR:
	{
		SetItemType(ITEM_TYPE::SKILLJELLY_SNOWSUGAR);
		SetScale(Vec2(60.f, 60.f));
		m_pTexture = CResourceManager::GetInst()->LoadTexture(L"SkillJelly_SnowSugar", L"texture\\jelly\\SkillJelly_SnowSugar.png");
		m_bDefaultMagnet = true;
		CMapTile::init();

		m_pEffectTex = CResourceManager::GetInst()->LoadTexture(L"whiteVFX", L"texture\\VFX\\whiteVFX.png");
		m_pJellySFX = CResourceManager::GetInst()->LoadSound(L"SFX_GetJelly", L"sound\\Jelly\\SFX_GetJelly.wav");
	}
	break;
	case ITEM_TYPE::PETJELLY_SNOWSUGAR:
	{
		//�ִϸ����� ����
		CreateAnimator();
		SetScale(Vec2(50.f, 50.f));
		SetItemType(ITEM_TYPE::PETJELLY_SNOWSUGAR);
		GetAnimator()->LoadAnimation(L"animation\\jelly\\Item_PetJelly_SnowSugar.anim");
		GetAnimator()->Play(L"PetJelly_SnowSugar", true);
		m_bDefaultMagnet = true;

		m_pEffectTex = CResourceManager::GetInst()->LoadTexture(L"whiteVFX", L"texture\\VFX\\whiteVFX.png");
		m_pJellySFX = CResourceManager::GetInst()->LoadSound(L"SFX_GetJelly", L"sound\\Jelly\\SFX_GetJelly.wav");
	}
	break;

	default:
		break;
	}
}

void CJelly::render(HDC _dc)
{
	if (!GetAnimator())
	{
		Vec2 vPos = CCamera::GetInst()->GetRenderPos(GetPos());
		Vec2 vScale = GetScale();

		//AlphaBlend ����
		BLENDFUNCTION tBlend = {};

		tBlend.AlphaFormat = AC_SRC_ALPHA;
		tBlend.BlendFlags = 0;
		tBlend.BlendOp = AC_SRC_OVER;
		tBlend.SourceConstantAlpha = int(255.f);

		AlphaBlend(_dc
			, (int)(vPos.x - m_pTexture->GetWidth() / 2)
			, (int)(vPos.y - m_pTexture->GetHeight() / 2)
			, m_pTexture->GetWidth()
			, m_pTexture->GetHeight()
			, m_pTexture->GetDC()
			, 0, 0
			, m_pTexture->GetWidth()
			, m_pTexture->GetHeight()
			, tBlend);
	}

	if (m_bDefaultMagnet)
	{
		//���� ���� Ư�� ������ �ڷ��� �ִ�
		CPlayer* pPlayer = CLevelManager::GetInst()->GetCurLevel()->GetPlayer();
		if ((fabsf(GetPos().x - pPlayer->GetPos().x) <= 350))
		{
			Vec2 vJellyPos = GetPos();
			Vec2 vJellyDir = pPlayer->GetPos() - vJellyPos;
			vJellyDir.Normalize();

			vJellyPos.x += vJellyDir.x * 500.f * DT;
			vJellyPos.y += vJellyDir.y * 500.f * DT;

			SetPos(vJellyPos);
		}
	}

	CObj::render(_dc);
}

void CJelly::BeginOverlap(CCollider* _pOther)
{
	//���̳���ĳ��Ʈ�� �̿��Ͽ� �浹�� ������Ʈ�� player Ŭ�������� Ȯ���� �� �ִ�
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(_pOther->GetOwner());
	CPet* pPet = dynamic_cast<CPet*>(_pOther->GetOwner());
	if (pPlayer != nullptr || pPet != nullptr)
	{
		m_pJellySFX->Play();
		ItemEffect();
		ItemVFX(0.5f, true);
		this->SetDead();
	}
}

void CJelly::OnOverlap(CCollider* _pOther)
{
}

void CJelly::EndOverlap(CCollider* _pOther)
{
}

void CJelly::ItemEffect()
{
	switch (m_eItem)
	{
	case ITEM_TYPE::ITEM_BEARJELLYPARTY:
	{
		//���� ���� ���� �Ϲ� ����(ITEM_TYPE::JELLY)���� ����������� �ٲ۴�
		vector<CObj*> vecJelly = CLevelManager::GetInst()->GetCurLevel()->GetVisibleJellyLayer();

		if (vecJelly.empty())
			return;

		for (size_t i = 0; i < vecJelly.size(); i++)
		{
			CJelly* Jelly = static_cast<CJelly*>(vecJelly[i]);
			
			//�Ϲ������� �ٲ۴�
			if (Jelly->GetItemType() == ITEM_TYPE::JELLY)
			{
				Vec2 vJellyPos = Jelly->GetPos();
				Jelly->SetDead();
				CJelly* pYellowBear = new CJelly;
				pYellowBear->init(ITEM_TYPE::JELLY_YELLOWBEAR);
				Instantiate(pYellowBear, vJellyPos, LAYER::JELLY);
			}
		}
	}
		break;
	case ITEM_TYPE::ITEM_BIGGER:
	{
		//�÷��̾��� �ִϸ��̼� ũ��� �浹ü ũ�⸦ 2��� �����, �Ŀ��� ���·� �����
		CPlayer* pPlayer = CLevelManager::GetInst()->GetCurLevel()->GetPlayer();
		pPlayer->SetBigger(true);
	}
		break;
	case ITEM_TYPE::ITEM_BONUS:
	{
		//�÷��̾��� ���ʽ�Ÿ�� ���� ���ɿ��θ� true�� �����.
		CPlayer* pPlayer = CLevelManager::GetInst()->GetCurLevel()->GetPlayer();
		pPlayer->SetBonus(true);
	}
		break;
	case ITEM_TYPE::ITEM_BOOSTER:
	{
		//�÷��̾ �Ŀ��� + ���ֻ��·� �����
		CPlayer* pPlayer = CLevelManager::GetInst()->GetCurLevel()->GetPlayer();
		pPlayer->SetBooster(true);
	}
		break;
	case ITEM_TYPE::ITEM_COINMAGIC:
	{
		//���� ���� ���� ��ֹ���(LAYER::OBSTACLE)���� ��ȭ�������� �ٲ۴�.
		vector<CObj*> vecObstacle = CLevelManager::GetInst()->GetCurLevel()->GetVisibleObstacleLayer();

		if (vecObstacle.empty())
			return;

		for (size_t i = 0; i < vecObstacle.size(); i++)
		{
			CObstacle* Obs = static_cast<CObstacle*>(vecObstacle[i]);
			
			Vec2 vObsScale = Obs->GetCollider()->GetFinalScale();
			float fObsWeight = vObsScale.x;
			float fObsHeight = vObsScale.y;
			Vec2 vObsLeftTop = Vec2(Obs->GetPos().x - fObsWeight /2.f, Obs->GetPos().y - fObsWeight / 2.f);
			float fCoinSize = 48.f;

			//���ΰ���(iRow)
			int iRow = fObsWeight / fCoinSize;
			//���� ����(iCol)
			int iCol = fObsHeight / fCoinSize;

			float fRowInnerPadding = (fObsWeight - (fCoinSize * iRow)) / 2.f;
			float fColInnerPadding = (fObsHeight - (fCoinSize * iCol)) / 2.f;

			for (int i = 0; i < iCol; i++)
			{
				for (int j = 0; j < iRow; j++)
				{
					CJelly* pSilverCoin = new CJelly;
					pSilverCoin->init(ITEM_TYPE::COIN_SILVER);

					Vec2 vStartPos = Vec2(vObsLeftTop.x - fRowInnerPadding - fCoinSize / 2, vObsLeftTop.y + fColInnerPadding + fCoinSize / 2);
					Vec2 vPos = Vec2(vStartPos.x + fCoinSize * j, vStartPos.y + fCoinSize * i);
					Instantiate(pSilverCoin, vPos, LAYER::JELLY);

					CEffect* pVFX = new CEffect;
					pVFX->CreateAnimator();
					pVFX->GetAnimator()->LoadAnimation(L"animation\\jelly\\VFX_CoinMagicPuff.anim");
					Instantiate(pVFX, vPos, LAYER::EFFECT);
					pVFX->GetAnimator()->Play(L"CoinMagic_Puff", false);
				}
			}
			Obs->SetDead();
		}
	}
		break;
	case ITEM_TYPE::ITEM_MAGNET:
	{
		//�� ������ �����ͼ� magnet�� on��Ų��
		CPet* pPet = CLevelManager::GetInst()->GetCurLevel()->GetPet();
		pPet->SetMagnet(true);

	}
		break;
	case ITEM_TYPE::ITEM_ICEBEARJELLYPARTY:
	{
		//���� ���� ���� �Ϲ� ����(ITEM_TYPE::JELLY)���� ������������ �ٲ۴�
		vector<CObj*> vecJelly = CLevelManager::GetInst()->GetCurLevel()->GetVisibleJellyLayer();

		if (vecJelly.empty())
			return;

		for (size_t i = 0; i < vecJelly.size(); i++)
		{
			CJelly* Jelly = static_cast<CJelly*>(vecJelly[i]);

			//�Ϲ������� �ٲ۴�
			if (Jelly->GetItemType() == ITEM_TYPE::JELLY)
			{
				Vec2 vJellyPos = Jelly->GetPos();
				Jelly->SetDead();
				CJelly* pIceBear = new CJelly;
				pIceBear->init(ITEM_TYPE::JELLY_ICEBEAR);
				Instantiate(pIceBear, vJellyPos, LAYER::JELLY);
			}
		}
	}
		break;
	case ITEM_TYPE::ITEM_MINIPOTION:
	{
		//�÷��̾��� ü���� 10 ȸ���Ѵ�.
		CPlayer* pPlayer = CLevelManager::GetInst()->GetCurLevel()->GetPlayer();
		int iHP = pPlayer->GetHP();
		iHP += 10;
		pPlayer->SetHP(iHP);
	}
		break;
	case ITEM_TYPE::ITEM_BIGPOTION:
	{
		//�÷��̾��� ü���� 40 ȸ���Ѵ�.
		CPlayer* pPlayer = CLevelManager::GetInst()->GetCurLevel()->GetPlayer();
		int iHP = pPlayer->GetHP();
		iHP += 40;
		pPlayer->SetHP(iHP);
	}
		break;
	case ITEM_TYPE::JELLY:
	{
		//�Ϲ� ����, ������ 300�� ȹ���Ѵ�.
		CPlayer* pPlayer = CLevelManager::GetInst()->GetCurLevel()->GetPlayer();
		pPlayer->AddScore(300);
	}
		break;
	case ITEM_TYPE::JELLY_YELLOWBEAR:
	{
		//����� ����, ������ 1000�� ȹ���Ѵ�.
		CPlayer* pPlayer = CLevelManager::GetInst()->GetCurLevel()->GetPlayer();
		pPlayer->AddScore(1000);
	}
		break;
	case ITEM_TYPE::JELLY_PINKBEAR:
	{
		//��ȫ�� ����, ������ 3000�� ȹ���Ѵ�.
		CPlayer* pPlayer = CLevelManager::GetInst()->GetCurLevel()->GetPlayer();
		pPlayer->AddScore(3000);
	}
		break;
	case ITEM_TYPE::JELLY_ICEBEAR:
	{
		//������ ����, ������ 5000�� ȹ���Ѵ�.
		CPlayer* pPlayer = CLevelManager::GetInst()->GetCurLevel()->GetPlayer();
		pPlayer->AddScore(5000);
	}
		break;
	case ITEM_TYPE::JELLY_BIGBEAR:
	{
		//�հ� ����, ������ 10000�� ȹ���Ѵ�.
		CPlayer* pPlayer = CLevelManager::GetInst()->GetCurLevel()->GetPlayer();
		pPlayer->AddScore(10000);
	}
		break;
	case ITEM_TYPE::JELLY_RAINBOWBEAR:
	{
		//�������� ����, ������ 15000�� ȹ���Ѵ�.
		CPlayer* pPlayer = CLevelManager::GetInst()->GetCurLevel()->GetPlayer();
		pPlayer->AddScore(15000);
	}
		break;
	case ITEM_TYPE::BONUS_RAINBOW:
	{
		//���ʽ� �������������� ������ ����, ������ 10000�� ȹ���Ѵ�.
		CPlayer* pPlayer = CLevelManager::GetInst()->GetCurLevel()->GetPlayer();
		pPlayer->AddScore(10000);
	}
		break;
	case ITEM_TYPE::BONUS_GOLD:
	{
		//���ʽ� �������������� ������ ����, ������ 8000�� ȹ���Ѵ�.
		CPlayer* pPlayer = CLevelManager::GetInst()->GetCurLevel()->GetPlayer();
		pPlayer->AddScore(8000);
	}
		break;
	case ITEM_TYPE::COIN_SILVER:
	{
		//���� ��ȭ ����, ���� 50���� ���� 1���� ȹ���Ѵ�.
		CPlayer* pPlayer = CLevelManager::GetInst()->GetCurLevel()->GetPlayer();
		pPlayer->AddScore(50);
		pPlayer->AddCoin(1);
	}
		break;
	case ITEM_TYPE::COIN_GOLD:
	{
		//���� ��ȭ ����, ���� 50���� ���� 5���� ȹ���Ѵ�.
		CPlayer* pPlayer = CLevelManager::GetInst()->GetCurLevel()->GetPlayer();
		pPlayer->AddScore(50);
		pPlayer->AddCoin(5);
	}
		break;
	case ITEM_TYPE::COIN_BIGSILVER:
	{
		//ū ��ȭ ����, ���� 500���� ���� 10���� ȹ���Ѵ�.
		CPlayer* pPlayer = CLevelManager::GetInst()->GetCurLevel()->GetPlayer();
		pPlayer->AddScore(500);
		pPlayer->AddCoin(10);
	}
		break;
	case ITEM_TYPE::COIN_BIGGOLD:
	{
		//ū ��ȭ ����, ���� 500���� ���� 50���� ȹ���Ѵ�.
		CPlayer* pPlayer = CLevelManager::GetInst()->GetCurLevel()->GetPlayer();
		pPlayer->AddScore(500);
		pPlayer->AddCoin(50);
	}
		break;
	case ITEM_TYPE::PETJELLY_MILK:
	{
		//������ ��Ű�� ���� ������ ������� ����, ���� 5õ��
		CPlayer* pPlayer = CLevelManager::GetInst()->GetCurLevel()->GetPlayer();
		pPlayer->AddScore(5000);
	}
	break;
	case ITEM_TYPE::PETJELLY_FAIRY:
	{
		//������ ��Ű�� ���� ������ �� ����, ���� 1���� // ���� ���ʽ��� 2����
		CPlayer* pPlayer = CLevelManager::GetInst()->GetCurLevel()->GetPlayer();
		CPetFlowerPod* pPet = dynamic_cast<CPetFlowerPod*>(CLevelManager::GetInst()->GetCurLevel()->GetPet());
		pPlayer->AddScore(pPet->GetFlowerJellyScore());
	}
	break;
	case ITEM_TYPE::SKILLJELLY_SNOWSUGAR:
	{
		//�������� ��Ű�� ������ ���� ����, ���� 1õ��	// ���� ���ʽ� �� 2õ��
		CSnowSugarCookie* pPlayer = dynamic_cast<CSnowSugarCookie*>(CLevelManager::GetInst()->GetCurLevel()->GetPlayer());
		pPlayer->AddScore(pPlayer->GetSnowJellyScore());
	}
	break;
	case ITEM_TYPE::PETJELLY_SNOWSUGAR:
	{
		//�������� ��Ű�� ���� ������ ��������Ƽ ������, ���� 3õ��
		CPlayer* pPlayer = CLevelManager::GetInst()->GetCurLevel()->GetPlayer();
		pPlayer->AddScore(3000);
		float hp = pPlayer->GetHP();
		pPlayer->SetHP(hp + 5);

		//���� ���� ���� �Ϲ� ����(ITEM_TYPE::JELLY)���� ������������ �ٲ۴�
		vector<CObj*> vecJelly = CLevelManager::GetInst()->GetCurLevel()->GetVisibleJellyLayer();

		if (vecJelly.empty())
			return;

		for (size_t i = 0; i < vecJelly.size(); i++)
		{
			CJelly* Jelly = static_cast<CJelly*>(vecJelly[i]);

			//�Ϲ������� �ٲ۴�
			if (Jelly->GetItemType() == ITEM_TYPE::JELLY)
			{
				Vec2 vJellyPos = Jelly->GetPos();
				Jelly->SetDead();
				CJelly* pIceBear = new CJelly;
				pIceBear->init(ITEM_TYPE::JELLY_ICEBEAR);
				Instantiate(pIceBear, vJellyPos, LAYER::JELLY);
			}
		}
	}
	break;
	default:
		break;
	}
}

void CJelly::ItemVFX(float _fTime, bool _bReduceAlpha)
{
	if (!m_pEffectTex)
		return;

	CEffect* pEffect = new CEffect;
	pEffect->SetEffectTex(m_pEffectTex);
	pEffect->SetMaxTime(_fTime);
	pEffect->SetAlphaReduce(_bReduceAlpha);

	Instantiate(pEffect, GetPos(), LAYER::EFFECT);
}

