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

//���Ԃ��Ǘ�����.
void CTime::TimeControl()
{
	m_MissionTime++;	//���܂���̏o�����Ԃ��J�E���g.

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
	/*m_MissionTime++;*/	//���܂���̏o�����Ԃ��J�E���g.
	m_BossTime++;		//�{�X���܂���̍s�����Ǘ�.
}

void CTime::Init()
{
	ForChangeTime = static_cast<float>(GAME_TIME);
	m_BossTime = 0;
	//m_MissionTime = 0;
}
