#ifndef CKUMA_H
#define CKUMA_H

#include "CCharacter.h"
//���܂���̍ő吔.
const int BEAR_MAX = 12;

class CKuma 
	: public CCharacter
{
public:



	CKuma();
	~CKuma();


	void Init();

private:


	//�A�j���[�V��������p.
	//���܂���(K).
	const int KWait = 0;
	const int KWalk = 1;
	const int KAttack = 2;
	const int KRejoice = 3;
	const int KEscape = 4;
	const int KHit = 5;
	const int KHyokori = 6;



	//���܂���̌��݂̓���.
	enum  class enKumaMove
	{
		Wait = 0,
		Move,
		Attack,
		Stop,
		Escape,
	};

	//�e�N�X�`�������ւ�.
	enum class enSkinChange
	{
		Clean = 0,
		Middle,
		Dirty,
	};


	//���܂���\����.
	struct BEAR_PARAM
	{
		D3DXVECTOR3 m_Pos;		//�|�W�V����(���W).
		D3DXVECTOR3 m_Rot;		//���[�e�[�V����(��]).
		D3DXVECTOR3 m_InitPos;	//�����ʒu�擾�p.
		D3DXVECTOR3 m_OldPos;	//�����蔻�蒲�ߗp.
		float m_Speed;			//���܂̃X�s�[�h.

		int m_anim_no;
		double m_anim_time;
		enSkinChange m_enSkin;	//���݂̃e�N�X�`��.
		//------------------------------------
		// �g�c.
		enKumaMove m_enKMove;//���܂���̂�.
		int m_InitDispCnt; //�����l�擾�p.
		int m_AppearCnt;		//�o���J�E���g.
		bool m_TexChangeFlag;	//�e�N�X�`���؂�ւ����Ă�H.
		int  m_SkinCnt;			//�e�N�X�`���̉���.
		int m_InitSkinCnt;		//�����l�擾�p.
		int	m_MovePattern;		//�ړ��p�^�[��.
		bool m_DispFlag;
		bool m_HitBomFlag;
		bool m_BearDamageSEflag;

		LPD3DXANIMATIONCONTROLLER pAC;

		BEAR_PARAM()
			: m_Pos()
			, m_Rot()
			, m_OldPos()
			, m_Speed(0.0f)
			, m_anim_no(0)
			, m_anim_time(0.0)
			, m_enSkin(enSkinChange::Dirty)//�����ŉ����������Ă���.
			, m_enKMove(enKumaMove::Wait)
			, m_InitDispCnt(0)
			, m_AppearCnt(0)
			, m_TexChangeFlag(false)
			, m_SkinCnt(0)
			, m_InitSkinCnt(0)
			, m_MovePattern(0)
			, m_DispFlag(false)
			, m_HitBomFlag(false)
			, m_BearDamageSEflag(false)
		{
		};
	};





	//�����X�V.
	//���g�k���]�A���W�Ȃǂ̒l�������ް��ɔ��f.
	void Update() final override;
	//���f���f�[�^�̏����Ǘ�.
	void MeshCreate() final override;


	BEAR_PARAM  m_KumaParam[BEAR_MAX];


};
#endif //#ifndef CKUMA_H