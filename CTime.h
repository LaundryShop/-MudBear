#ifndef CTIME_H
#define CTIME_H

const int	GAME_TIME = 34;	//制限時間.
const int	MISSION_TIME = 1860;		//大体終わる時間.
class CTime
{
public:
	CTime();
	~CTime();
	void	TimeControl();	//ミッション内の時間を管理する.
	void    TimeCnt();	//カウントしていく関数.

	void    Init();

	//表示用時間を取得.
	char GetDispTime() {
		return m_DisplayTime;
	}

	//時間情報を設定.
	void SetMissionTime(const int& Time) { 
		m_MissionTime = Time;
	}
	//時間情報を取得.
	int GetMissionTime() {
		return m_MissionTime;
	}
	//ボスの行動時間情報を設定.
	void SetBossTime(const int& BossTime) {
		m_BossTime = BossTime;
	}
	//ボスの行動時間情報を取得.
	int GetBossTime() {
		return m_BossTime;
	}

	//時間の桁の余り出し(UI表示に使用している).
	int SetDigits() {
		return m_DisplayTime % 10;
	}
	//時間の桁の出し(UI表示に使用している).
	void PutDigits() {
		m_DisplayTime /= 10;
	}
private:
	float	ForChangeTime;	//表示用に変更用時間.
	char	m_DisplayTime;	//表示用時間.
	int		m_MissionTime;		//くまさんの表示時間を図る＆
								//ミッションステージをクリア出来なかった時進める用.
	int     m_BossTime;		//ボス行動時間用.
};
#endif //#ifndef CTIME_H
