#pragma once
#include "CLevel.h"

class CPlayer;
class CSound;
class CTextUI;

class CBonusStage :
	public CLevel
{
public:
	CBonusStage();
	~CBonusStage();

public:
	CLONE_DEACTIVATE(CBonusStage);

private:
	CPlayer*	m_pPlayer;
	CSound*		m_pBGM;

	CTextUI* m_pCoinText;
	CTextUI* m_pScoreText;

public:
	virtual void init() override;
	virtual void tick() override;
	virtual void Enter() override;
	virtual void Exit() override;

public:
	void SetScoreText(CTextUI* _UI) { m_pScoreText = _UI; };
	void SetCoinText(CTextUI* _UI) { m_pCoinText = _UI; };

	CTextUI* GetScoreText() { return m_pScoreText; }
	CTextUI* GetCoinText() { return m_pCoinText; }
};

