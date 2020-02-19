#ifndef MAIN_H
#define MAIN_H
#include <stdio.h>
#include "Global.h"		//�O���[�o���N���X.

#include "CDX9Mesh.h"	//Dx9���g�p����׽.
#include "CSprite.h"	//����غ��(���ײ�)�׽.
#include "CSpriteUI.h"	//���ײ�UI�׽.
#include "CHideTex.h"	//�B���e�N�X�`���N���X(�g�c).
#include "CCharacter.h"	
#include "CArea.h"		//�͈�.
//=============================================
//			��������������Ă���
//=============================================
#include "CCamera.h"		//�J�����N���X.
#include "CMouse.h"			//�}�E�X�N���X.
#include "CPlayer.h"		//�v���C���[�N���X.
#include "CGround.h"		//�n�ʃN���X.
#include "CGrassGround.h"	//���̒n�ʃN���X.
#include "CStone.h"			//�΃N���X.
#include "CGrass.h"			//���N���X.
#include "CBackground.h"	//�w�i�N���X.
#include "CGunParts.h"		//���S�C�̃p�[�c�N���X.
#include "CPump.h"			//���S�C�̃|���v�N���X.
#include "CFade.h"			//�t�F�C�h�N���X.
#include "CTime.h"			//���ԃN���X.
#include "CSound.h"			//�T�E���h�N���X.

//=================================================
//	�萔.
//=================================================
const char WND_TITLE[] = "���߂Ă�3D�V���[�e�B���O";
const char APP_NAME[]  = "3DSTG";
//-------------------------------
// �g�c�@�ύX

const float IKD_EPSIRON = 0.00001f;	// �덷
const float SHOT_SPD = 0.3f;		//�e�̑���.
const int SHOT_MAX = 20;			//�e�̍ő吔.

//�D�����p.
const float MUD_SPD = 0.1f;
const int MUD_MAX = 1;				//�D�̍ő吔.
const float MUD_JUMP_POWER = 0.09f;	//�D�W�����v��.0.06.
const float GREVITY = 0.002f;		//�d��.

//�����D�p
const int BOM_MAX = 3;				//�����D�̍ő吔.
const int BOM_DAMAGE = 2;		    //�����D�_���[�W.
const float BOM_SPD = 0.1f;			//�����D�̑��x.
const float BOM_JUMP_POWER = 0.09f;	//�����D�W�����v��;
const float BOM_GREVITY = 0.002f;	//�d��.

const float BOM_AREA = 9.0f;		//������͈�.

const int HIT_CNT_MAX = 3;				//(�{�X���܂���)���������Ƃ��ɂЂ��.


const float SHOT_WAIT = 0.5f;		//���ˊԊu.
const int HEAD = 0;	//��.
const int BODY = 1;	//��.
const int ARM_L = 2;//���r.
const int ARM_R = 3;//�E�r.

const float ATK_LENGTH = 2.0f;	//���܂���{�X���܂��񂪍U�����Ă��鋗��.
const int CLEAN = 0;			//���ꂢ�ȏ��.

//�{�X���܂���s���p
const int APPROACH_TIME = 1800;//�߂Â��Ă��鎞��(��).
const int THROW_TIME = 100;//�D������Ԋu(��).
const float BKUMA_INIT = 56.0f;


//�ڂ����܂���(B).
const int BWait = 0;
const int BApproach = 1;
const int BAttack = 2;
const int BThrow = 3;
const int BWinch = 4;
const int BRejoice = 5;
const int BKnead = 6;
const int BShakeUp = 7;
const int BShakeDown = 8;



//-------------------------------
//=================================================
//		��������������Ă���(�ǉ�����)
//=================================================
const float K_Left = -1.0f;	//���܂��񂪍�������o��
const float K_Right = 1.0f;	//���܂��񂪉E������o��
const float CAM_CENTER		= 0.0f;	//�J�����̐^��.
const int	CLEARMISSION	= 5;	//�~�b�V����(���܂�����Y��ɂ�����).

const int	WATER_BALLOON	= 3;	//�����D�̐�(�ő吔).

const float	CLICK_START_Y = 500.0f;	//�N���b�N���W��Y���W

const float UI_MOSTLOWER = 580.0f;	//UI�̕\���ň�ԉ��̏ꏊ.
//=================================================
/**************************************************
*	Ҳݸ׽.
**/
class CMain
{
public:
	CMain();	//�ݽ�׸�.
	~CMain();	//�޽�׸�.

	//����޳�������֐�.
	HRESULT InitWindow(
		HINSTANCE hInstance,
		INT x, INT y, INT width, INT height,
		const char* WindowName);

	//����޳�֐�(ү���ޖ��̏���).
	LRESULT MsgProc(
		HWND hWnd, UINT uMsg,
		WPARAM wParam, LPARAM lParam);

	//ү����ٰ��.
	void Loop();
	//���ع����Ҳݏ���.
	void AppMain();
	//�����ݸ�(�`��).
	void Render();

//----------------------------
// ��������3D�֌W.
	//DirectX������.
	HRESULT InitD3D();
	//Direct3D�I������.
	void DestroyD3D();
	
	//======Load�֌W==========//
	//ү���̓ǂݍ��݊֐�.
	HRESULT LoadMesh();
	//UI�̓ǂݍ��݊֐�.
	HRESULT	 LoadUI();
	//�T�E���h�̓ǂݍ��݊֐�.
	HRESULT	 LoadSound();
	//========================//

	//����ލ쐬.
	HRESULT InitShader();
//	//���ٍ쐬.
//	HRESULT InitModel();

	//�[�x(Z)ý�ON/OFF�؂�ւ�.
	void SetDepth(bool flag);

	//�̨��쐬.
	HRESULT InitSphere(CDX9Mesh* pMesh);

	//--------------------------------------------
	// �g�c.
	//�Փ˔���(�޳��ިݸ޽̨�).
	bool Collision(CDX9Mesh* pAttack, D3DXVECTOR3 AtkPos, CDX9Mesh* pTarget, D3DXVECTOR3 TgtPos);
	//---------------------------------------------

	//-----------------------------------------------------------


	// XZ���ʂƃX�N���[�����W�̌�_�Z�o�֐�.
	D3DXVECTOR3* CalcScreenToXZ(D3DXVECTOR3* Pos, float Sx, float Sy, D3DXMATRIX* View, D3DXMATRIX* Prj);

	//�������W�ƌ�_���W�Ԃ̒P�ʃx�N�g���擾�֐�.
	D3DXVECTOR3* UnitMove(D3DXVECTOR3* Pos, D3DXVECTOR3 HitPos, D3DXVECTOR3 InitPos, float SHOT_SPD);

	//------------------------------------------------------------------
	//	2019_7_26 �X�V�@�g�c.

	//���C�ƽ̨��̓����蔻��.
	bool RaySphere(D3DXVECTOR3* Pos, D3DXVECTOR3* x, D3DXVECTOR3* v, D3DXVECTOR3* pTgtPos);
	//---------------------------------------------------------------------
	//�g�c
	//�A�j���[�V�����X�V.
	void UpdeAnim();
	//����������.
	void  Init();

private:
	HWND	m_hWnd;	//����޳�����.

//----------------------------
// ��������3D�֌W.
	//======================================
	//	�\����.
	//======================================
	//�ݽ����ޯ̧�̱��ؑ��̒�`.
	//������ޓ��̺ݽ����ޯ̧�ƈ�v���Ă���K�v����.
	struct SHADER_CONSTANT_BUFFER
	{
		D3DXMATRIX	mW;			//ܰ��ލs��.
		D3DXMATRIX	mWVP;		//ܰ���,�ޭ�,��ۼު���݂̍����ϊ��s��.	
		D3DXVECTOR4	vLightDir;	//ײĕ���.
		D3DXVECTOR4	vDiffuse;	//�ި̭��ސF(�g�U���ːF).
		D3DXVECTOR4	vSpecular;	//��߷�אF(���ʔ��ːF).
		D3DXVECTOR4	vCamPos;	//��׈ʒu(���_�ʒu).
	};
	
	//===================================================
	//	��������������Ă���.
	//===================================================
	//�V�[���J��.
	enum class enScene
	{
		enTitle = 0,
		enGameMain,
		enGameOver,
		enGameClear,
	};

	//�X�e�[�W�̓��e.
	enum class enStageScene
	{
		enMission = 0,
		enBonus,
		enBoss
	};

	//�G�@�̃p�����[�^.
	enum class enENEMY_PARAM_DATA
	{
		NONE = -1,

		enAPPEARANCECNT,	//�o���J�E���g
		enPOSITION_X,		//x���W
		enPOSITION_Y,		//y���W
		enPOSITION_Z,		//z���W
		enSPEED,			//�X�s�[�h.
		enDAMAGECNT,		//�_���[�W(����)�J�E���g.		
		enMOVEPATTERN,		//�ړ��̃p�^�[��.

		enMAX
	};
	//=======================================================

	//����.
	enum class enDirection
	{
		Stop = 0,		//��~.
		Forward,		//�O�i.
		Backward,		//���.
		Left,			//��.
		Right,			//�E.
	};

	//--------------------------------
	//�@�g�c.


	//-------------------------------------
	// �g�c�X�V�@2020_1_18.
	//���܂���̌��݂̓���.
	enum  class enKumaMove
	{
		Wait = 0,
		Move,
		Attack,
		Stop,
		Escape,
	};


	//�{�X���܂���̌��݂̓���.
	enum  class enBossKumaMove
	{
		Wait = 0,
		Approach,//�߂Â�.
		Attack,	//�U��.
		Throw,	//������.
		Winch,	//��낯.
	};


	////���܂���\����.
	//struct BEAR_PARAM
	//{
	//	D3DXVECTOR3 m_Pos;		//�|�W�V����(���W).
	//	D3DXVECTOR3 m_Rot;		//���[�e�[�V����(��]).
	//	D3DXVECTOR3 m_InitPos;	//�����ʒu�擾�p.
	//	D3DXVECTOR3 m_OldPos;	//�����蔻�蒲�ߗp.
	//	float m_Speed;			//���܂̃X�s�[�h.

	//	int m_anim_no;
	//	double m_anim_time;
	//	enSkinChange m_enSkin;	//���݂̃e�N�X�`��.
	//	//------------------------------------
	//	// �g�c.
	//	enKumaMove m_enKMove;//���܂���̂�.
	//	int m_InitDispCnt; //�����l�擾�p.
	//	int m_AppearCnt;		//�o���J�E���g.
	//	bool m_TexChangeFlag;	//�e�N�X�`���؂�ւ����Ă�H.
	//	int  m_SkinCnt;			//�e�N�X�`���̉���.
	//	int m_InitSkinCnt;		//�����l�擾�p.
	//	int	m_MovePattern;		//�ړ��p�^�[��.
	//	bool m_DispFlag;
	//	bool m_HitBomFlag;
	//	bool m_BearDamageSEflag;

	//	LPD3DXANIMATIONCONTROLLER pAC;

	//	BEAR_PARAM()
	//		: m_Pos()
	//		, m_Rot()
	//		, m_OldPos()
	//		, m_Speed(0.0f)
	//		, m_anim_no(0)
	//		, m_anim_time(0.0)
	//		, m_enSkin(enSkinChange::Dirty)//�����ŉ����������Ă���.
	//		, m_enKMove(enKumaMove::Wait)
	//		, m_InitDispCnt(0)
	//		, m_AppearCnt(0)
	//		, m_TexChangeFlag(false)
	//		, m_SkinCnt(0)
	//		, m_InitSkinCnt(0)
	//		, m_MovePattern(0)
	//		, m_DispFlag(false)
	//		, m_HitBomFlag(false)
	//		, m_BearDamageSEflag(false)
	//	{
	//	};
	//};

	//�{�X���܂���\����.
	struct BOSSBEAR_PARAM
	{
		D3DXVECTOR3 m_Pos;		//�|�W�V����(���W).
		int m_SkinChangeCnt[TEX_NO];	//�{�X���܂���p�̃e�N�X�`���؂�ւ��J�E���^.
		enBossKumaMove m_enBKMove;		//�{�X���܂���̌��݂̓���.
		int m_anim_no;
		double m_anim_time;
		int m_HitCnt;					//�e�ɓ���������.
		int m_CleanCnt;					//���ꂢ�ɂȂ������ʂ̐�.
		bool m_BomHitFlag;
		bool m_HitBomFlag;

		BOSSBEAR_PARAM()
			: m_Pos()
			, m_SkinChangeCnt()
			, m_enBKMove(enBossKumaMove::Wait)
			, m_anim_no(0)
			, m_anim_time(0.0)
			, m_HitCnt(0)
			, m_CleanCnt(0)
			, m_BomHitFlag(false)
			, m_HitBomFlag(false)

		{
		};
	};


	//�e�\����.
	struct BULLET_PARAM
	{
		D3DXVECTOR3 m_Pos;
		D3DXVECTOR3   m_VecUnit;		//�P�ʃx�N�g��.
		int		m_DispCount;	//�\���J�E���g.
		bool	m_ShotFlag;		//�����׸�.
		bool  m_SoundFlag;		//�T�E���h�t���O.

		BULLET_PARAM()
			: m_Pos()
			, m_VecUnit()
			, m_DispCount(0)
			, m_ShotFlag(false)
			, m_SoundFlag(false)
		{};
	};

	//�����D�\����.
	struct BOM_PARAM
	{
		D3DXVECTOR3 m_Pos;
		D3DXVECTOR3 m_HitPos;
		D3DXVECTOR3   m_VecUnit;		//�P�ʃx�N�g��.
		int		m_DispCount;	//�\���J�E���g.
		float	m_fAcc;
		bool	m_ShotFlag;			//�����׸�.
		bool    m_ExpFlag;	        //�����t���O.
		bool    m_BExpFlag;			//�ڂ����ܗp.
		bool  m_SoundFlag;		//�T�E���h�t���O.

		BOM_PARAM()
			: m_Pos()
			, m_VecUnit()
			, m_DispCount(0)
			, m_fAcc(0.0f)
			, m_ShotFlag(false)
			, m_ExpFlag(false)
			, m_BExpFlag(false)
			, m_SoundFlag(false)
		{};
	};
	
	//�D�\����.
	struct MUD_PARAM
	{
		D3DXVECTOR3 m_Pos;
		D3DXVECTOR3   m_VecUnit;		//�P�ʃx�N�g��.
		int		m_DispCount;	//�\���J�E���g.
		float	m_fAcc;			//����.
		bool	m_ShotFlag;		//�����׸�.
		bool	m_SwishFlag;

		MUD_PARAM()
			: m_Pos()
			, m_VecUnit()
			, m_DispCount(0)
			, m_fAcc(0.0f)
			, m_ShotFlag(false)
			, m_SwishFlag(false)
		{};
	};

	//�B���e�N�X�`���\����.
	struct HIDE_TEX_PARAM
	{
		D3DXVECTOR3 m_Pos;

		HIDE_TEX_PARAM()
			: m_Pos()
		{};
	};


	//-------------------------------------

	//�����؂ɂЂƂ�.
	ID3D11Device*			m_pDevice11;	//���޲���޼ު��.
	ID3D11DeviceContext*	m_pContext11;	//���޲���÷��.
	IDXGISwapChain*			m_pSwapChain;	//�ܯ������.
	ID3D11RenderTargetView*	m_pBackBuffer_TexRTV;//���ް���ޯ��ޭ�.
	ID3D11Texture2D*		m_pBackBuffer_DSTex;//���߽��ݼٗpø���.
	ID3D11DepthStencilView*	m_pBackBuffer_DSTexDSV;//���߽��ݼ��ޭ�.

	//�[�x(Z)ýĐݒ�.
	ID3D11DepthStencilState*	m_pDepthStencilState;
	ID3D11DepthStencilState*	m_pDepthStencilStateOff;

	D3DXVECTOR3				m_vLight;	//ײĂ̕���.

	D3DXMATRIX	mWorld;	//ܰ��ލs��.
	D3DXMATRIX	mView;	//�ޭ�(���)�s��.
	D3DXMATRIX	mProj;	//��ۼު����(�ˉe)�s��.

	//�����ق̎�ނ��Ƃɗp��.
	ID3D11VertexShader*		m_pVertexShader;	//���_�����.
	ID3D11InputLayout*		m_pVertexLayout;	//���_ڲ���.
	ID3D11PixelShader*		m_pPixelShader;		//�߸�ټ����.
	ID3D11Buffer*			m_pConstantBuffer;	//�ݽ����ޯ̧.

	//�����ق��Ƃɗp��.
	ID3D11Buffer*			m_pVertexBuffer;	//���_�ޯ̧.


	
	ID3D11SamplerState*		m_pSampleLinear;	//�����:ø����Ɋe��̨����������.
	ID3D11ShaderResourceView*	m_pTexture;		//ø���.
	//----------------------------
	// �g�c�@�X�V 2019_10_17.
	CDX9SkinMesh*	m_pCDxKumaMesh;	//�X�L�����b�V���N���X�@���܂���p.
	CCharacter*		m_pKumaSkin;

	CDX9SkinMesh*	m_pCDxB_KumaMesh;	//�X�L�����b�V���N���X�@�{�X���܂���p.
	CCharacter*		m_pB_KumaSkin;
	//----------------------------
	//�޳��ިݸ޽̨��p.
	CDX9Mesh*       m_pSphere;//���܂��񓖂��蔻��p.
	//---------------------------------------------------
	// �{�X���܂���̓����蔻��p�X�t�B�A.
	//---------------------------------------------------
	CDX9Mesh*       m_pBHeadSphere;	//�{�X���܂��񓪓����蔻��p.
	CDX9Mesh*       m_pBBodySphere;	//�{�X���܂���̓����蔻��p.
	CDX9Mesh*		m_pBNeckSphere;
	CDX9Mesh*       m_pBArmSphere;	//�{�X���܂���r�����蔻��p.

	//---------------------------------------------
	// �g�c.
	CDX9Mesh*		m_pEnemy;		//�G�@.
	CDX9Mesh*		m_pShot;		//�e.
	CDX9Mesh*		m_pBom;			//�{��.
	CDX9Mesh*		m_pMud;			//�D.


	//����ؗp.
	CSprite*		m_pHide;	//�B���p.

	CHideTex*		m_pHideTex;
	CHideTex*		m_pBHideTex;
	CHideTex*		m_pBomHideTex;

	CFade*			m_pFade;		//�t�F�C�h�̏���.
	CSpriteUI*		m_pUIFade;		//�t�F�C�h�̉摜.


	CArea*          m_pArea;		//�����D�̃G���A����.

	//�^�C�g����ʂŎg���悤��.
	CSpriteUI*		m_pTitle;		//�^�C�g��UI(���S).
	CSpriteUI*		m_pStartUI;		//�n�߂�(�Q�[����i�߂�).
	CSpriteUI*		m_pEndUI;		//�Q�[������߂�.
	//�Q�[���I�[�o�[��ʂŎg���悤��.
	CSpriteUI*		m_pOverUI;		//�Q�[���I�[�o�[UI(���S).
	CSpriteUI*		m_pOverWord;	//�Q�[���I�[�o�[�̎��̌��t.
	CSpriteUI*		m_pRetryButton;	//�Q�[���ɖ߂�{�^��.
	CSpriteUI*		m_pBackButton;	//�^�C�g���ɖ߂�{�^��.
	//�Q�[���N���A��ʂŎg���悤��.
	CSpriteUI*		m_pClearUI;		//�N���AUI(���S).
	CSpriteUI*		m_pReturnButton;//�߂�{�^��.

	enScene			m_enScene;		//�V�[���J�ڗp.
	enStageScene	m_enStageScene;	//�Q�[���i�s�p.

	BEAR_PARAM	m_KumaParam[BEAR_MAX];		//���܂���̃p�����[�^.
	BOSSBEAR_PARAM  m_BossKumaParam;		//�{�X���܂���̃p�����[�^.
	BULLET_PARAM 	m_ShotParam[SHOT_MAX];	//���S�C�̐��̃p�����[�^.
	BOM_PARAM	m_BomParam;					//�����D�̃p�����[�^.
	MUD_PARAM   m_MudParam[MUD_MAX];		//�{�X�̓����D�̃p�����[�^.

	HIDE_TEX_PARAM	m_E_HideParam[BEAR_MAX];//���܂���p.
	HIDE_TEX_PARAM   m_M_HideParam[MUD_MAX];//�D�p.
	HIDE_TEX_PARAM  m_B_HideParam[TEX_NO];	//�{�X���܂���p.
	HIDE_TEX_PARAM	m_Bom_HideParam;		//�����D�p

	//----------------------------------------
	//	�e�N�X�`���؂�ւ�.
	//���܂���p.
	std::shared_ptr <CDX9SkinMesh::CHANGE_TEXTURES>	m_pChangeTextures;
	//�{�X���܂���p.
	CDX9SkinMesh::CHANGE_TEXTURES*		m_pBChangeTextures;

	//�e��޼ު�����Ұ�.
	D3DXVECTOR3 m_WMousePos;//�}�E�X�̃��[���h���W���p.
	D3DXVECTOR3 m_HitPos;	//���C�œ��������ꏊ.

	/*D3DXVECTOR3 m_VecUnitOld;*/
	//---------------------------------------------
	// �X�L�����b�V���̃{�[�����W���擾����.
	//---------------------------------------------
		D3DXVECTOR3 vHeadBonePos;		//���̃{�[���ʒu.
		D3DXVECTOR3 vHeadBoneOldPos;	//���̃{�[���ʒu�����p.
		D3DXVECTOR3 vBodyBonePos[2];	//�̂̃{�[���ʒu.
		D3DXVECTOR3 vBodyBoneOldPos;	//�̂̃{�[���ʒu�����p.
		D3DXVECTOR3 vNeckBoneOldPos;	//��̃{�[���ʒu�����p.
		D3DXVECTOR3 vArmRBonePos[3];	//�E�r�̃{�[���ʒu.
		D3DXVECTOR3 vArmLBonePos[3];	//���r�̃{�[���ʒu.

		

	float ShotWait; //���ˊԊu�p.

	bool			m_alphaFlag;
	CPlayer*		m_pPlayer;	//�v���C���[�N���X.
	CCamera*		m_pCamera;	//�J�������.
	CMouse*			m_pMouse;	//�}�E�X���.
	CSpriteUI*		m_pCursor;	//�J�[�\��UI.

	CGround*		m_pGround;		//�n�ʃN���X.
	CGrassGround*	m_pGrassGround;	//���̒n�ʃN���X.
	CStone*			m_pStone;		//�΃N���X.
	CGrass*			m_pGrass;		//���N���X.
	CBackground*	m_pBackground;	//�w�i�N���X.
	CGunParts*		m_pGunParts;	//���S�C�̃p�[�c�N���X.
	CPump*			m_pPump;		//���S�C�̃|���v�N���X.
	bool			m_GunDispFlag;	//���S�C�\���t���O.

	CTime*			m_pTime;		//���ԃN���X.

	int				BearDataCnt;	//�f�[�^�J�E���g(Excel�f�[�^�ɕK�v).

	bool			m_ContinueFlag;	//�R���e�j���[����(true)���Ȃ�(false).

	CPlayer::enDamageParam	m_Damage;	//�v���C���[�̉���.
	CSpriteUI*		m_pMudDamage_1;	//�}�b�h�_���[�W��UI_1(�_���[�W��).
	CSpriteUI*		m_pMudDamage_2;	//�}�b�h�_���[�W��UI_2(�_���[�W��).
	CSpriteUI*		m_pMudDamage_3;	//�}�b�h�_���[�W��UI_3(�_���[�W��).

	//==============�Q�[�W�Ɏg���Ă�����̂���============.
	CSpriteUI*		m_pDecremetGauge;//�����Q�[�W��UI.
	CSpriteUI*		m_pGaugeBar;	 //�Q�[�W�o�[.
	CSpriteUI*		m_pBossBearIcon; //�{�X���܂���̃A�C�R��.
	CSpriteUI*		m_pSpaceBar;	 //������������ƃX�y�[�X.
	CSpriteUI*		m_pSpaceLogo;	 //�X�y�[�X�̃��S.
	bool			m_SpaceDispflag; //�\���t���O.

	CSpriteUI*		m_pWaterBalloon[WATER_BALLOON];	//�����DUI.
	int				WaterBallonCnt;					//�����D(���������p).
	int				WaterBallonDispCnt;				//�����D(�O�������p).

	CSpriteUI*		m_pBearMission;	//���܂���̃~�b�V����.
	CSpriteUI*		m_pBossMission;	//�{�X�~�b�V����.


	//============���܂���̐������Ǘ�====================================.
	CSpriteUI*		m_pInGameCleanUI;		//���܂�����Y��ɂ�����(�Q�[����).
	CSpriteUI*		m_pMissionUI;			//�~�b�V�����N���A���邽�߂̐�.
	CSpriteUI*		m_pCleanUI;				//���܂�����Y��ɂ�����UI.
	int				m_BearCleanCnt;			//���܂�����Y��ɂ��������J�E���g����.
	int				m_BearCleanDigit;		//m_BearCleanCnt�̌�������o�����߂Ɋi�[����p.
	int				m_InsideBearClean[2];	///UI��\������ۂ�m_BearCleanCnt�̌�������o��(��������).
	//=====================================================================.
	
	//============�{�X���܂���̐������Ǘ�=================================.
	CSpriteUI*		m_pBossCleanUI;	//�{�X���܂�����Y��ɂ�����UI.
	int				m_BossCleanCnt;	//�{�X���܂�����Y��ɂ��������J�E���g����.
	int				m_BossCleanDigit;	//m_BearCleanCnt�̌�������o�����߂Ɋi�[����p.
	int				m_InsideBossClean[2];	///UI��\������ۂ�m_BearCleanCnt�̌�������o��(��������).
	//=====================================================================.

	//============�{�X���܂���̐������Ǘ�=================================.
	CSpriteUI*		m_pTotalBearUI;	//���܂���̍��v��UI.
	int				m_TotalBearCnt;	//���܂���̍��v��.
	int				m_TotalBearDigit;	//m_BearCleanCnt�̌�������o�����߂Ɋi�[����p.
	int				m_InsideTotalBear[2];	///UI��\������ۂ�m_BearCleanCnt�̌�������o��(��������).
	//=====================================================================.

	CSpriteUI*		m_pTimeUI;		//���Ԃ��J�E���g����UI.
	int				m_TimeCnt[2];	//���Ԃ�����I�ɓ����.

	bool m_P_Moveflag;		//�v���C���[�Ȃǂ̈ړ��t���O.
	//���̃p�����[�^�Ɏg���Ă܂�.
	struct Parts
	{
		D3DXVECTOR3 m_Pos;
		D3DXVECTOR3 m_Rot;
		D3DXVECTOR3 m_Scale;

		Parts()
			: m_Pos(0.0f,0.0f,0.0f)
			, m_Rot(0.0f, 0.0f, 0.0f)
			, m_Scale(0.0f, 0.0f, 0.0f)
		{};
	};
	//===============�����D�͈�===================
	CDX9Mesh*		m_pBomArea;		//�{���͈�.
	Parts           m_HaniParam;

	//�L�[���������Ƃ���x�����ʂ�Ȃ����邽�߂̂��.
	int m_SpeceKey;

	//=========�T�E���h�֌W==============.
	clsSound*	m_TitleBGM;		//�^�C�g��BGM.
	clsSound*	m_pMainBGM;		//�Q�[�����C��BGM.
	clsSound*	m_pBossBGM;		//�{�X�X�e�[�WBGM.
	clsSound*	m_pGameOverBGM;	//�Q�[���I�[�o�[BGM.
	clsSound*	m_pGameClearBGM;//�Q�[���N���ABGM.

	clsSound*	m_pClickSE;			//�N���b�N��SE.
	clsSound*	m_pCircleSE;		//�~��SE.
	clsSound*	m_WaterShotSE;		//���̔���SE.
	clsSound*	m_WaterBalloonSE;	//�����D�̔���SE.
	clsSound*	m_pPlayerMoveSE;	//�v���C���[�̈ړ�SE.
	clsSound*	m_pPlayerDamageSE;	//�v���C���[�̃_���[�WSE.
	clsSound*	m_pBearDamageSE;	//���܂���̃_���[�W.
	clsSound*	m_pPumpSE;
	clsSound*	m_pSwishSE;
	char	m_Sound[256];
	bool	m_BGMflag;				//BGM�p�t���O.
	bool	m_DamageSEflag;			//�_���[�W�̎���SE�t���O.
	bool	m_PumpSEflag;

	//===============�����炢���߂Ɋ֐��ɂ�������================================.
		//�X�N���v�gUI�̍��W.
	void UI_Display(
		float Base_W, float Base_H,		//���摜�̑傫��
		float Stride_W, float Stride_H,	//1�R�}������̕�,����
		float Disp_W, float Disp_H,		//�\������傫��.
		const char* fileName,			//�t�@�C���̖��O(�摜��)
		CSpriteUI*	UIName);			//SpriteUI�̖��O.

	void Sprite(
		float Base_W, float Base_H,		//���摜�̑傫��
		float Stride_W, float Stride_H,	//1�R�}������̕�,����
		float Disp_W, float Disp_H,		//�\������傫��.
		const char* fileName,			//�t�@�C���̖��O(�摜��)				
		CSprite*	UIName);			//SpriteUI�̖��O.

	void GetKyeAppMain();	//�^�C�g�����N���b�N�����Ƃ��̏���(�^�C�g���̏���).
	void GetKyeOver();	//�Q�[���I�[�o�[���N���b�N�����Ƃ��̏���(�Q�[���I�[�o�[�̏���).
	void GetKyeClear();	//�Q�[���N���A���N���b�N�����Ƃ��̏���(�Q�[���N���A�̏���).	
	void WaterBalloonProcessAppMain();//�A�v�����C���̐����D�̏���.
	void WaterGunProcessAppMain();//�A�v�����C���̐��S�C�̏���.
	void WaterGunGaugeChargeAppMain();		//���S�C��\�����ăQ�[�W���񕜂��鏈��.

	void GaugeRender();		//�Q�[�W�Ń����_�����O�ɒu���Ă���.
	void P_DamageRender();	//�v���C���[�_���[�W�Ń����_�����O�ɒu���Ă���.
	void FileDataBear();	//�f�[�^�̓ǂݍ���(���܂���̃f�[�^)&���[�h���b�V���ł��Ă���.
	void BGMSoundDrow(clsSound* m_pSound, bool m_SoundFlag);	//BGM�T�E���h�̗���(�����_�����O����).
	void SESoundDrowBlaze(clsSound* m_pSound, bool  m_SoundFlag);	//SE�T�E���h�̗���(�����_�����O����).
	void SESoundDrow(clsSound* m_pSound, bool  m_SoundFlag);	//SE�T�E���h�̗���(�����_�����O����).

};
#endif//#ifndef MAIN_H
