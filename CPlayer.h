#ifndef CPLAYER_H
#define CPLAYER_H
#include "CGameObject.h"

const int	SMALL_DAMAGE = 1;	//�_���[�W��(��)
const int	MIDDLE_DAMAGE = 2;	//�_���[�W��(��)
const int	BIG_DAMAGE = 3;		//�_���[�W��(��)
const int	LAST_DAMAGE = 4;	//�v���C���[���R���e�j���[�ɂȂ邽�߂̃_���[�W.
class CPlayer
	:public	CGameObject
{
public:
	//���g�̃_���[�W�̗񋓑�.
	enum class enDamageParam
	{
		enNoDamage = 0,

		enDamage_1,
		enDamage_2,
		enDamage_3,
	};
	CPlayer();
	~CPlayer();

	void Init();		//������.
	void DamageInit();	//�_���[�W�̏�����.

	void Move(bool m_flag);	//�v���C���[�̓���.

	void UpDamage() { m_DamageCnt += 1;	 }		//�_���[�W���󂯂���.
	int	 GetDamage() { return m_DamageCnt; }	//�_���[�W�����擾.
private:
	int				m_DamageCnt;	//�_���[�W�̃J�E���g.
};
#endif //#ifndef CPLAYER_H

