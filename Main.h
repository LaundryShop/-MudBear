#ifndef MAIN_H
#define MAIN_H
#include <stdio.h>
#include "Global.h"		//グローバルクラス.

#include "CDX9Mesh.h"	//Dx9を使用するｸﾗｽ.
#include "CSprite.h"	//板ﾎﾟﾘｺﾞﾝ(ｽﾌﾟﾗｲﾄ)ｸﾗｽ.
#include "CSpriteUI.h"	//ｽﾌﾟﾗｲﾄUIｸﾗｽ.
#include "CHideTex.h"	//隠すテクスチャクラス(吉田).
#include "CCharacter.h"	
#include "CArea.h"		//範囲.
//=============================================
//			小松が手を加えている
//=============================================
#include "CCamera.h"		//カメラクラス.
#include "CMouse.h"			//マウスクラス.
#include "CPlayer.h"		//プレイヤークラス.
#include "CGround.h"		//地面クラス.
#include "CGrassGround.h"	//草の地面クラス.
#include "CStone.h"			//石クラス.
#include "CGrass.h"			//草クラス.
#include "CBackground.h"	//背景クラス.
#include "CGunParts.h"		//水鉄砲のパーツクラス.
#include "CPump.h"			//水鉄砲のポンプクラス.
#include "CFade.h"			//フェイドクラス.
#include "CTime.h"			//時間クラス.
#include "CSound.h"			//サウンドクラス.

//=================================================
//	定数.
//=================================================
const char WND_TITLE[] = "初めての3Dシューティング";
const char APP_NAME[]  = "3DSTG";
//-------------------------------
// 吉田　変更

const float IKD_EPSIRON = 0.00001f;	// 誤差
const float SHOT_SPD = 0.3f;		//弾の速さ.
const int SHOT_MAX = 20;			//弾の最大数.

//泥調整用.
const float MUD_SPD = 0.1f;
const int MUD_MAX = 1;				//泥の最大数.
const float MUD_JUMP_POWER = 0.09f;	//泥ジャンプ力.0.06.
const float GREVITY = 0.002f;		//重力.

//水風船用
const int BOM_MAX = 3;				//水風船の最大数.
const int BOM_DAMAGE = 2;		    //水風船ダメージ.
const float BOM_SPD = 0.1f;			//水風船の速度.
const float BOM_JUMP_POWER = 0.09f;	//水風船ジャンプ力;
const float BOM_GREVITY = 0.002f;	//重力.

const float BOM_AREA = 9.0f;		//当たる範囲.

const int HIT_CNT_MAX = 3;				//(ボスくまさん)当たったときにひるむ.


const float SHOT_WAIT = 0.5f;		//発射間隔.
const int HEAD = 0;	//頭.
const int BODY = 1;	//体.
const int ARM_L = 2;//左腕.
const int ARM_R = 3;//右腕.

const float ATK_LENGTH = 2.0f;	//くまさんボスくまさんが攻撃してくる距離.
const int CLEAN = 0;			//きれいな状態.

//ボスくまさん行動用
const int APPROACH_TIME = 1800;//近づいてくる時間(仮).
const int THROW_TIME = 100;//泥投げる間隔(仮).
const float BKUMA_INIT = 56.0f;


//ぼすくまさん(B).
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
//		小松が手を加えている(追加した)
//=================================================
const float K_Left = -1.0f;	//くまさんが左側から登場
const float K_Right = 1.0f;	//くまさんが右側から登場
const float CAM_CENTER		= 0.0f;	//カメラの真ん中.
const int	CLEARMISSION	= 5;	//ミッション(くまさんを綺麗にした数).

const int	WATER_BALLOON	= 3;	//水風船の数(最大数).

const float	CLICK_START_Y = 500.0f;	//クリック座標のY座標

const float UI_MOSTLOWER = 580.0f;	//UIの表示で一番下の場所.
//=================================================
/**************************************************
*	ﾒｲﾝｸﾗｽ.
**/
class CMain
{
public:
	CMain();	//ｺﾝｽﾄﾗｸﾀ.
	~CMain();	//ﾃﾞｽﾄﾗｸﾀ.

	//ｳｨﾝﾄﾞｳ初期化関数.
	HRESULT InitWindow(
		HINSTANCE hInstance,
		INT x, INT y, INT width, INT height,
		const char* WindowName);

	//ｳｨﾝﾄﾞｳ関数(ﾒｯｾｰｼﾞ毎の処理).
	LRESULT MsgProc(
		HWND hWnd, UINT uMsg,
		WPARAM wParam, LPARAM lParam);

	//ﾒｯｾｰｼﾞﾙｰﾌﾟ.
	void Loop();
	//ｱﾌﾟﾘｹｰｼｮﾝﾒｲﾝ処理.
	void AppMain();
	//ﾚﾝﾀﾞﾘﾝｸﾞ(描画).
	void Render();

//----------------------------
// ここから3D関係.
	//DirectX初期化.
	HRESULT InitD3D();
	//Direct3D終了処理.
	void DestroyD3D();
	
	//======Load関係==========//
	//ﾒｯｼｭの読み込み関数.
	HRESULT LoadMesh();
	//UIの読み込み関数.
	HRESULT	 LoadUI();
	//サウンドの読み込み関数.
	HRESULT	 LoadSound();
	//========================//

	//ｼｪｰﾀﾞ作成.
	HRESULT InitShader();
//	//ﾓﾃﾞﾙ作成.
//	HRESULT InitModel();

	//深度(Z)ﾃｽﾄON/OFF切り替え.
	void SetDepth(bool flag);

	//ｽﾌｨｱ作成.
	HRESULT InitSphere(CDX9Mesh* pMesh);

	//--------------------------------------------
	// 吉田.
	//衝突判定(ﾊﾞｳﾝﾃﾞｨﾝｸﾞｽﾌｨｱ).
	bool Collision(CDX9Mesh* pAttack, D3DXVECTOR3 AtkPos, CDX9Mesh* pTarget, D3DXVECTOR3 TgtPos);
	//---------------------------------------------

	//-----------------------------------------------------------


	// XZ平面とスクリーン座標の交点算出関数.
	D3DXVECTOR3* CalcScreenToXZ(D3DXVECTOR3* Pos, float Sx, float Sy, D3DXMATRIX* View, D3DXMATRIX* Prj);

	//初期座標と交点座標間の単位ベクトル取得関数.
	D3DXVECTOR3* UnitMove(D3DXVECTOR3* Pos, D3DXVECTOR3 HitPos, D3DXVECTOR3 InitPos, float SHOT_SPD);

	//------------------------------------------------------------------
	//	2019_7_26 更新　吉田.

	//レイとｽﾌｨｱの当たり判定.
	bool RaySphere(D3DXVECTOR3* Pos, D3DXVECTOR3* x, D3DXVECTOR3* v, D3DXVECTOR3* pTgtPos);
	//---------------------------------------------------------------------
	//吉田
	//アニメーション更新.
	void UpdeAnim();
	//初期化する.
	void  Init();

private:
	HWND	m_hWnd;	//ｳｨﾝﾄﾞｳﾊﾝﾄﾞﾙ.

//----------------------------
// ここから3D関係.
	//======================================
	//	構造体.
	//======================================
	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧのｱﾌﾟﾘ側の定義.
	//※ｼｪｰﾀﾞ内のｺﾝｽﾀﾝﾄﾊﾞｯﾌｧと一致している必要あり.
	struct SHADER_CONSTANT_BUFFER
	{
		D3DXMATRIX	mW;			//ﾜｰﾙﾄﾞ行列.
		D3DXMATRIX	mWVP;		//ﾜｰﾙﾄﾞ,ﾋﾞｭｰ,ﾌﾟﾛｼﾞｪｸｼｮﾝの合成変換行列.	
		D3DXVECTOR4	vLightDir;	//ﾗｲﾄ方向.
		D3DXVECTOR4	vDiffuse;	//ﾃﾞｨﾌｭｰｽﾞ色(拡散反射色).
		D3DXVECTOR4	vSpecular;	//ｽﾍﾟｷｭﾗ色(鏡面反射色).
		D3DXVECTOR4	vCamPos;	//ｶﾒﾗ位置(視点位置).
	};
	
	//===================================================
	//	小松が手を加えている.
	//===================================================
	//シーン遷移.
	enum class enScene
	{
		enTitle = 0,
		enGameMain,
		enGameOver,
		enGameClear,
	};

	//ステージの内容.
	enum class enStageScene
	{
		enMission = 0,
		enBonus,
		enBoss
	};

	//敵機のパラメータ.
	enum class enENEMY_PARAM_DATA
	{
		NONE = -1,

		enAPPEARANCECNT,	//出現カウント
		enPOSITION_X,		//x座標
		enPOSITION_Y,		//y座標
		enPOSITION_Z,		//z座標
		enSPEED,			//スピード.
		enDAMAGECNT,		//ダメージ(汚れ具合)カウント.		
		enMOVEPATTERN,		//移動のパターン.

		enMAX
	};
	//=======================================================

	//方向.
	enum class enDirection
	{
		Stop = 0,		//停止.
		Forward,		//前進.
		Backward,		//後退.
		Left,			//左.
		Right,			//右.
	};

	//--------------------------------
	//　吉田.


	//-------------------------------------
	// 吉田更新　2020_1_18.
	//くまさんの現在の動き.
	enum  class enKumaMove
	{
		Wait = 0,
		Move,
		Attack,
		Stop,
		Escape,
	};


	//ボスくまさんの現在の動き.
	enum  class enBossKumaMove
	{
		Wait = 0,
		Approach,//近づく.
		Attack,	//攻撃.
		Throw,	//投げる.
		Winch,	//よろけ.
	};


	////くまさん構造体.
	//struct BEAR_PARAM
	//{
	//	D3DXVECTOR3 m_Pos;		//ポジション(座標).
	//	D3DXVECTOR3 m_Rot;		//ローテーション(回転).
	//	D3DXVECTOR3 m_InitPos;	//初期位置取得用.
	//	D3DXVECTOR3 m_OldPos;	//当たり判定調節用.
	//	float m_Speed;			//くまのスピード.

	//	int m_anim_no;
	//	double m_anim_time;
	//	enSkinChange m_enSkin;	//現在のテクスチャ.
	//	//------------------------------------
	//	// 吉田.
	//	enKumaMove m_enKMove;//くまさんのみ.
	//	int m_InitDispCnt; //初期値取得用.
	//	int m_AppearCnt;		//出現カウント.
	//	bool m_TexChangeFlag;	//テクスチャ切り替えしてる？.
	//	int  m_SkinCnt;			//テクスチャの汚れ具合.
	//	int m_InitSkinCnt;		//初期値取得用.
	//	int	m_MovePattern;		//移動パターン.
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
	//		, m_enSkin(enSkinChange::Dirty)//初期で汚いが入っている.
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

	//ボスくまさん構造体.
	struct BOSSBEAR_PARAM
	{
		D3DXVECTOR3 m_Pos;		//ポジション(座標).
		int m_SkinChangeCnt[TEX_NO];	//ボスくまさん用のテクスチャ切り替えカウンタ.
		enBossKumaMove m_enBKMove;		//ボスくまさんの現在の動き.
		int m_anim_no;
		double m_anim_time;
		int m_HitCnt;					//弾に当たった回数.
		int m_CleanCnt;					//きれいになった部位の数.
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


	//弾構造体.
	struct BULLET_PARAM
	{
		D3DXVECTOR3 m_Pos;
		D3DXVECTOR3   m_VecUnit;		//単位ベクトル.
		int		m_DispCount;	//表示カウント.
		bool	m_ShotFlag;		//ｼｮｯﾄﾌﾗｸﾞ.
		bool  m_SoundFlag;		//サウンドフラグ.

		BULLET_PARAM()
			: m_Pos()
			, m_VecUnit()
			, m_DispCount(0)
			, m_ShotFlag(false)
			, m_SoundFlag(false)
		{};
	};

	//水風船構造体.
	struct BOM_PARAM
	{
		D3DXVECTOR3 m_Pos;
		D3DXVECTOR3 m_HitPos;
		D3DXVECTOR3   m_VecUnit;		//単位ベクトル.
		int		m_DispCount;	//表示カウント.
		float	m_fAcc;
		bool	m_ShotFlag;			//ｼｮｯﾄﾌﾗｸﾞ.
		bool    m_ExpFlag;	        //爆発フラグ.
		bool    m_BExpFlag;			//ぼすくま用.
		bool  m_SoundFlag;		//サウンドフラグ.

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
	
	//泥構造体.
	struct MUD_PARAM
	{
		D3DXVECTOR3 m_Pos;
		D3DXVECTOR3   m_VecUnit;		//単位ベクトル.
		int		m_DispCount;	//表示カウント.
		float	m_fAcc;			//加速.
		bool	m_ShotFlag;		//ｼｮｯﾄﾌﾗｸﾞ.
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

	//隠すテクスチャ構造体.
	struct HIDE_TEX_PARAM
	{
		D3DXVECTOR3 m_Pos;

		HIDE_TEX_PARAM()
			: m_Pos()
		{};
	};


	//-------------------------------------

	//↓ｱﾌﾟﾘにひとつ.
	ID3D11Device*			m_pDevice11;	//ﾃﾞﾊﾞｲｽｵﾌﾞｼﾞｪｸﾄ.
	ID3D11DeviceContext*	m_pContext11;	//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
	IDXGISwapChain*			m_pSwapChain;	//ｽﾜｯﾌﾟﾁｪｰﾝ.
	ID3D11RenderTargetView*	m_pBackBuffer_TexRTV;//ﾚﾝﾀﾞｰﾀｰｹﾞｯﾄﾋﾞｭｰ.
	ID3D11Texture2D*		m_pBackBuffer_DSTex;//ﾃﾞﾌﾟｽｽﾃﾝｼﾙ用ﾃｸｽﾁｬ.
	ID3D11DepthStencilView*	m_pBackBuffer_DSTexDSV;//ﾃﾞﾌﾟｽｽﾃﾝｼﾙﾋﾞｭｰ.

	//深度(Z)ﾃｽﾄ設定.
	ID3D11DepthStencilState*	m_pDepthStencilState;
	ID3D11DepthStencilState*	m_pDepthStencilStateOff;

	D3DXVECTOR3				m_vLight;	//ﾗｲﾄの方向.

	D3DXMATRIX	mWorld;	//ﾜｰﾙﾄﾞ行列.
	D3DXMATRIX	mView;	//ﾋﾞｭｰ(ｶﾒﾗ)行列.
	D3DXMATRIX	mProj;	//ﾌﾟﾛｼﾞｪｸｼｮﾝ(射影)行列.

	//↓ﾓﾃﾞﾙの種類ごとに用意.
	ID3D11VertexShader*		m_pVertexShader;	//頂点ｼｪｰﾀﾞ.
	ID3D11InputLayout*		m_pVertexLayout;	//頂点ﾚｲｱｳﾄ.
	ID3D11PixelShader*		m_pPixelShader;		//ﾋﾟｸｾﾙｼｪｰﾀﾞ.
	ID3D11Buffer*			m_pConstantBuffer;	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.

	//↓ﾓﾃﾞﾙごとに用意.
	ID3D11Buffer*			m_pVertexBuffer;	//頂点ﾊﾞｯﾌｧ.


	
	ID3D11SamplerState*		m_pSampleLinear;	//ｻﾝﾌﾟﾗ:ﾃｸｽﾁｬに各種ﾌｨﾙﾀをかける.
	ID3D11ShaderResourceView*	m_pTexture;		//ﾃｸｽﾁｬ.
	//----------------------------
	// 吉田　更新 2019_10_17.
	CDX9SkinMesh*	m_pCDxKumaMesh;	//スキンメッシュクラス　くまさん用.
	CCharacter*		m_pKumaSkin;

	CDX9SkinMesh*	m_pCDxB_KumaMesh;	//スキンメッシュクラス　ボスくまさん用.
	CCharacter*		m_pB_KumaSkin;
	//----------------------------
	//ﾊﾞｳﾝﾃﾞｨﾝｸﾞｽﾌｨｱ用.
	CDX9Mesh*       m_pSphere;//くまさん当たり判定用.
	//---------------------------------------------------
	// ボスくまさんの当たり判定用スフィア.
	//---------------------------------------------------
	CDX9Mesh*       m_pBHeadSphere;	//ボスくまさん頭当たり判定用.
	CDX9Mesh*       m_pBBodySphere;	//ボスくまさん体当たり判定用.
	CDX9Mesh*		m_pBNeckSphere;
	CDX9Mesh*       m_pBArmSphere;	//ボスくまさん腕当たり判定用.

	//---------------------------------------------
	// 吉田.
	CDX9Mesh*		m_pEnemy;		//敵機.
	CDX9Mesh*		m_pShot;		//弾.
	CDX9Mesh*		m_pBom;			//ボム.
	CDX9Mesh*		m_pMud;			//泥.


	//板ﾎﾟﾘ用.
	CSprite*		m_pHide;	//隠す用.

	CHideTex*		m_pHideTex;
	CHideTex*		m_pBHideTex;
	CHideTex*		m_pBomHideTex;

	CFade*			m_pFade;		//フェイドの処理.
	CSpriteUI*		m_pUIFade;		//フェイドの画像.


	CArea*          m_pArea;		//水風船のエリア処理.

	//タイトル画面で使うよう↓.
	CSpriteUI*		m_pTitle;		//タイトルUI(ロゴ).
	CSpriteUI*		m_pStartUI;		//始める(ゲームを進める).
	CSpriteUI*		m_pEndUI;		//ゲームをやめる.
	//ゲームオーバー画面で使うよう↓.
	CSpriteUI*		m_pOverUI;		//ゲームオーバーUI(ロゴ).
	CSpriteUI*		m_pOverWord;	//ゲームオーバーの時の言葉.
	CSpriteUI*		m_pRetryButton;	//ゲームに戻るボタン.
	CSpriteUI*		m_pBackButton;	//タイトルに戻るボタン.
	//ゲームクリア画面で使うよう↓.
	CSpriteUI*		m_pClearUI;		//クリアUI(ロゴ).
	CSpriteUI*		m_pReturnButton;//戻るボタン.

	enScene			m_enScene;		//シーン遷移用.
	enStageScene	m_enStageScene;	//ゲーム進行用.

	BEAR_PARAM	m_KumaParam[BEAR_MAX];		//くまさんのパラメータ.
	BOSSBEAR_PARAM  m_BossKumaParam;		//ボスくまさんのパラメータ.
	BULLET_PARAM 	m_ShotParam[SHOT_MAX];	//水鉄砲の水のパラメータ.
	BOM_PARAM	m_BomParam;					//水風船のパラメータ.
	MUD_PARAM   m_MudParam[MUD_MAX];		//ボスの投擲泥のパラメータ.

	HIDE_TEX_PARAM	m_E_HideParam[BEAR_MAX];//くまさん用.
	HIDE_TEX_PARAM   m_M_HideParam[MUD_MAX];//泥用.
	HIDE_TEX_PARAM  m_B_HideParam[TEX_NO];	//ボスくまさん用.
	HIDE_TEX_PARAM	m_Bom_HideParam;		//水風船用

	//----------------------------------------
	//	テクスチャ切り替え.
	//くまさん用.
	std::shared_ptr <CDX9SkinMesh::CHANGE_TEXTURES>	m_pChangeTextures;
	//ボスくまさん用.
	CDX9SkinMesh::CHANGE_TEXTURES*		m_pBChangeTextures;

	//各ｵﾌﾞｼﾞｪｸﾄﾊﾟﾗﾒｰﾀ.
	D3DXVECTOR3 m_WMousePos;//マウスのワールド座標化用.
	D3DXVECTOR3 m_HitPos;	//レイで当たった場所.

	/*D3DXVECTOR3 m_VecUnitOld;*/
	//---------------------------------------------
	// スキンメッシュのボーン座標を取得する.
	//---------------------------------------------
		D3DXVECTOR3 vHeadBonePos;		//頭のボーン位置.
		D3DXVECTOR3 vHeadBoneOldPos;	//頭のボーン位置調整用.
		D3DXVECTOR3 vBodyBonePos[2];	//体のボーン位置.
		D3DXVECTOR3 vBodyBoneOldPos;	//体のボーン位置調整用.
		D3DXVECTOR3 vNeckBoneOldPos;	//首のボーン位置調整用.
		D3DXVECTOR3 vArmRBonePos[3];	//右腕のボーン位置.
		D3DXVECTOR3 vArmLBonePos[3];	//左腕のボーン位置.

		

	float ShotWait; //発射間隔用.

	bool			m_alphaFlag;
	CPlayer*		m_pPlayer;	//プレイヤークラス.
	CCamera*		m_pCamera;	//カメラ情報.
	CMouse*			m_pMouse;	//マウス情報.
	CSpriteUI*		m_pCursor;	//カーソルUI.

	CGround*		m_pGround;		//地面クラス.
	CGrassGround*	m_pGrassGround;	//草の地面クラス.
	CStone*			m_pStone;		//石クラス.
	CGrass*			m_pGrass;		//草クラス.
	CBackground*	m_pBackground;	//背景クラス.
	CGunParts*		m_pGunParts;	//水鉄砲のパーツクラス.
	CPump*			m_pPump;		//水鉄砲のポンプクラス.
	bool			m_GunDispFlag;	//水鉄砲表示フラグ.

	CTime*			m_pTime;		//時間クラス.

	int				BearDataCnt;	//データカウント(Excelデータに必要).

	bool			m_ContinueFlag;	//コンテニューする(true)しない(false).

	CPlayer::enDamageParam	m_Damage;	//プレイヤーの汚れ具合.
	CSpriteUI*		m_pMudDamage_1;	//マッドダメージのUI_1(ダメージ小).
	CSpriteUI*		m_pMudDamage_2;	//マッドダメージのUI_2(ダメージ中).
	CSpriteUI*		m_pMudDamage_3;	//マッドダメージのUI_3(ダメージ大).

	//==============ゲージに使っているものたち============.
	CSpriteUI*		m_pDecremetGauge;//減少ゲージのUI.
	CSpriteUI*		m_pGaugeBar;	 //ゲージバー.
	CSpriteUI*		m_pBossBearIcon; //ボスくまさんのアイコン.
	CSpriteUI*		m_pSpaceBar;	 //減少しすぎるとスペース.
	CSpriteUI*		m_pSpaceLogo;	 //スペースのロゴ.
	bool			m_SpaceDispflag; //表示フラグ.

	CSpriteUI*		m_pWaterBalloon[WATER_BALLOON];	//水風船UI.
	int				WaterBallonCnt;					//水風船(内部処理用).
	int				WaterBallonDispCnt;				//水風船(外部処理用).

	CSpriteUI*		m_pBearMission;	//くまさんのミッション.
	CSpriteUI*		m_pBossMission;	//ボスミッション.


	//============くまさんの数字を管理====================================.
	CSpriteUI*		m_pInGameCleanUI;		//くまさんを綺麗にした数(ゲーム中).
	CSpriteUI*		m_pMissionUI;			//ミッションクリアするための数.
	CSpriteUI*		m_pCleanUI;				//くまさんを綺麗にした数UI.
	int				m_BearCleanCnt;			//くまさんを綺麗にした数をカウントする.
	int				m_BearCleanDigit;		//m_BearCleanCntの桁を割り出すために格納する用.
	int				m_InsideBearClean[2];	///UIを表示する際にm_BearCleanCntの桁を割り出す(内部処理).
	//=====================================================================.
	
	//============ボスくまさんの数字を管理=================================.
	CSpriteUI*		m_pBossCleanUI;	//ボスくまさんを綺麗にした数UI.
	int				m_BossCleanCnt;	//ボスくまさんを綺麗にした数をカウントする.
	int				m_BossCleanDigit;	//m_BearCleanCntの桁を割り出すために格納する用.
	int				m_InsideBossClean[2];	///UIを表示する際にm_BearCleanCntの桁を割り出す(内部処理).
	//=====================================================================.

	//============ボスくまさんの数字を管理=================================.
	CSpriteUI*		m_pTotalBearUI;	//くまさんの合計数UI.
	int				m_TotalBearCnt;	//くまさんの合計数.
	int				m_TotalBearDigit;	//m_BearCleanCntの桁を割り出すために格納する用.
	int				m_InsideTotalBear[2];	///UIを表示する際にm_BearCleanCntの桁を割り出す(内部処理).
	//=====================================================================.

	CSpriteUI*		m_pTimeUI;		//時間をカウントするUI.
	int				m_TimeCnt[2];	//時間を内部的に入れる.

	bool m_P_Moveflag;		//プレイヤーなどの移動フラグ.
	//↓のパラメータに使ってます.
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
	//===============水風船範囲===================
	CDX9Mesh*		m_pBomArea;		//ボム範囲.
	Parts           m_HaniParam;

	//キーを押したとき一度しか通らなくするためのやつ.
	int m_SpeceKey;

	//=========サウンド関係==============.
	clsSound*	m_TitleBGM;		//タイトルBGM.
	clsSound*	m_pMainBGM;		//ゲームメインBGM.
	clsSound*	m_pBossBGM;		//ボスステージBGM.
	clsSound*	m_pGameOverBGM;	//ゲームオーバーBGM.
	clsSound*	m_pGameClearBGM;//ゲームクリアBGM.

	clsSound*	m_pClickSE;			//クリックのSE.
	clsSound*	m_pCircleSE;		//円のSE.
	clsSound*	m_WaterShotSE;		//水の発射SE.
	clsSound*	m_WaterBalloonSE;	//水風船の発射SE.
	clsSound*	m_pPlayerMoveSE;	//プレイヤーの移動SE.
	clsSound*	m_pPlayerDamageSE;	//プレイヤーのダメージSE.
	clsSound*	m_pBearDamageSE;	//くまさんのダメージ.
	clsSound*	m_pPumpSE;
	clsSound*	m_pSwishSE;
	char	m_Sound[256];
	bool	m_BGMflag;				//BGM用フラグ.
	bool	m_DamageSEflag;			//ダメージの時のSEフラグ.
	bool	m_PumpSEflag;

	//===============見ずらいために関数にしたもの================================.
		//スクリプトUIの座標.
	void UI_Display(
		float Base_W, float Base_H,		//元画像の大きさ
		float Stride_W, float Stride_H,	//1コマ当たりの幅,高さ
		float Disp_W, float Disp_H,		//表示する大きさ.
		const char* fileName,			//ファイルの名前(画像名)
		CSpriteUI*	UIName);			//SpriteUIの名前.

	void Sprite(
		float Base_W, float Base_H,		//元画像の大きさ
		float Stride_W, float Stride_H,	//1コマ当たりの幅,高さ
		float Disp_W, float Disp_H,		//表示する大きさ.
		const char* fileName,			//ファイルの名前(画像名)				
		CSprite*	UIName);			//SpriteUIの名前.

	void GetKyeAppMain();	//タイトルをクリックしたときの処理(タイトルの処理).
	void GetKyeOver();	//ゲームオーバーをクリックしたときの処理(ゲームオーバーの処理).
	void GetKyeClear();	//ゲームクリアをクリックしたときの処理(ゲームクリアの処理).	
	void WaterBalloonProcessAppMain();//アプリメインの水風船の処理.
	void WaterGunProcessAppMain();//アプリメインの水鉄砲の処理.
	void WaterGunGaugeChargeAppMain();		//水鉄砲を表示してゲージを回復する処理.

	void GaugeRender();		//ゲージでレンダリングに置いてある.
	void P_DamageRender();	//プレイヤーダメージでレンダリングに置いてある.
	void FileDataBear();	//データの読み込み(くまさんのデータ)&ロードメッシュでしている.
	void BGMSoundDrow(clsSound* m_pSound, bool m_SoundFlag);	//BGMサウンドの流す(レンダリング処理).
	void SESoundDrowBlaze(clsSound* m_pSound, bool  m_SoundFlag);	//SEサウンドの流す(レンダリング処理).
	void SESoundDrow(clsSound* m_pSound, bool  m_SoundFlag);	//SEサウンドの流す(レンダリング処理).

};
#endif//#ifndef MAIN_H
