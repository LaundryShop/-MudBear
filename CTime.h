#ifndef CTIME_H
#define CTIME_H

const int	GAME_TIME = 34;	//��������.
const int	MISSION_TIME = 1860;		//��̏I��鎞��.
class CTime
{
public:
	CTime();
	~CTime();
	void	TimeControl();	//�~�b�V�������̎��Ԃ��Ǘ�����.
	void    TimeCnt();	//�J�E���g���Ă����֐�.

	void    Init();

	//�\���p���Ԃ��擾.
	char GetDispTime() {
		return m_DisplayTime;
	}

	//���ԏ���ݒ�.
	void SetMissionTime(const int& Time) { 
		m_MissionTime = Time;
	}
	//���ԏ����擾.
	int GetMissionTime() {
		return m_MissionTime;
	}
	//�{�X�̍s�����ԏ���ݒ�.
	void SetBossTime(const int& BossTime) {
		m_BossTime = BossTime;
	}
	//�{�X�̍s�����ԏ����擾.
	int GetBossTime() {
		return m_BossTime;
	}

	//���Ԃ̌��̗]��o��(UI�\���Ɏg�p���Ă���).
	int SetDigits() {
		return m_DisplayTime % 10;
	}
	//���Ԃ̌��̏o��(UI�\���Ɏg�p���Ă���).
	void PutDigits() {
		m_DisplayTime /= 10;
	}
private:
	float	ForChangeTime;	//�\���p�ɕύX�p����.
	char	m_DisplayTime;	//�\���p����.
	int		m_MissionTime;		//���܂���̕\�����Ԃ�}�違
								//�~�b�V�����X�e�[�W���N���A�o���Ȃ��������i�߂�p.
	int     m_BossTime;		//�{�X�s�����ԗp.
};
#endif //#ifndef CTIME_H
