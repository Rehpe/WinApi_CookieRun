#include "pch.h"
#include "CBonusStage.h"
#include "CPlayer.h"
#include "CBackground.h"
#include "CEngine.h"
#include "CCamera.h"
#include "CGingerBraveCookie.h"
#include "CPetChocoDrop.h"
#include "CSound.h"
#include "CResourceManager.h"
#include "CTextUI.h"
#include "CLevelManager.h"
#include "CKeyManager.h"

CBonusStage::CBonusStage()
{
	SetColliderRender(false);
}

CBonusStage::~CBonusStage()
{
}

void CBonusStage::init()
{
	//보너스타임 맵 Load
 	LoadMap(L"map\\BonusStage.map");

	//Background 생성
	CObj* pBackground = new CBackground;
	static_cast<CBackground*>(pBackground)->init();
	pBackground->SetPos(Vec2(0.f, 0.f));
	AddObject(pBackground, LAYER::BACKGROUND);

	//플레이어 생성 및 초기셋팅
	vector<CObj*> vecPlayer = GetLayer(LAYER::PLAYER);
	if (!vecPlayer.empty())
	{
		SetPlayer(static_cast<CPlayer*>(vecPlayer.front()));
	}

	vector<CObj*> vecPet = GetLayer(LAYER::PET);
	if (!vecPet.empty())
	{
		SetPet(static_cast<CPet*>(vecPet.front()));
	}

	CCamera::GetInst()->SetLook(Vec2(200.f, 360.f));
	GetPlayer()->SetPos(Vec2(200.f, 360.f));

	//BGM 재생
	m_pBGM = CResourceManager::GetInst()->LoadSound(L"BGM_BonusStage", L"sound\\BGM\\BGM_BonusStage.wav");
	m_pBGM->PlayToBGM(true);

}

void CBonusStage::tick()
{
	//카메라가 보고 있는 위치를 해상도의 중심으로 설정(좌상단의 좌표가 0,0이 될 수 있게)
	Vec2 vResolution = CEngine::GetInst()->GetResolution();
	Vec2 vLook = CCamera::GetInst()->GetLook();
	Vec2 vOffset = Vec2(450.f, -280.f);
	CCamera::GetInst()->SetLook(Vec2((GetPlayer()->GetPos()).x + vOffset.x, 300.f));

	//jump 누르면 캐릭터 올라가고, 떼면 캐릭터 내려옴
	//쿠키가 움직일수 있는 한계 고점, 저점이 있음

 	m_pCoinText->SetNumber(GetPlayer()->GetCoin());
	m_pScoreText->SetNumber(GetPlayer()->GetScore());

	if (IsTap(KEY::LCTRL))
	{
		if (GetColliderRender())
		{
			SetColliderRender(false);
		}
		else
		{
			SetColliderRender(true);
		}
	}

	if (IsTap(KEY::ESC))
	{
		if (CLevelManager::GetInst()->GetCurLevel()->GetPause())
			return;

		Pause();
	}


	CLevel::tick();
}

void CBonusStage::Enter()
{
	//보너스 타임 진입시
	init();
}

void CBonusStage::Exit()
{
	m_pBGM->Stop();

	GetLayer(LAYER::PLAYER).clear();
	GetLayer(LAYER::PET).clear();
	GetLayer(LAYER::UI).clear();
	GetLayer(LAYER::PLAYER_SKILL).clear();

	//보너스 타임 나올때
	LevelDestroy();
}
