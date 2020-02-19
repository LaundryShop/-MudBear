#include "CTime.h"

CTime::CTime()
	: ForChangeTime(static_cast<float>(GAME_TIME))
	, m_MissionTime(0)
	, m_BossTime(0)
	
{
}

CTime::~CTime()
{
}

//時間を管理する.
void CTime::TimeControl()
{
	m_MissionTime++;	//くまさんの出現時間をカウント.

	ForChangeTime = ForChangeTime / 1.000f;
	if (ForChangeTime > 0.0f) {
		ForChangeTime -= 0.018f;
	}
	else if (ForChangeTime <= 0.0f) {
		ForChangeTime = 0.0f;
	}
	m_DisplayTime = static_cast<char>(ForChangeTime);
}

void CTime::TimeCnt()
{
	/*m_MissionTime++;*/	//くまさんの出現時間をカウント.
	m_BossTime++;		//ボスくまさんの行動を管理.
}

void CTime::Init()
{
	ForChangeTime = static_cast<float>(GAME_TIME);
	m_BossTime = 0;
	//m_MissionTime = 0;
}
