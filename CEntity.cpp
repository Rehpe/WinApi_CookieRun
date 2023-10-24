#include "pch.h"
#include "CEntity.h"

UINT CEntity::g_iNextID = 0;

CEntity::CEntity()
	: m_id(g_iNextID++)
{
}

//CEntity 클래스는 복사될 때 원본의 id가 아닌, 새로운 id를 부여받는다. 
CEntity::CEntity(const CEntity& _Origin)
	:m_strName(_Origin.m_strName)
	,m_id(g_iNextID++)
{
}

CEntity::~CEntity()
{
}
