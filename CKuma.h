#ifndef CKUMA_H
#define CKUMA_H

#include "CCharacter.h"
//くまさんの最大数.
const int BEAR_MAX = 12;

class CKuma 
	: public CCharacter
{
public:



	CKuma();
	~CKuma();


	void Init();

private:


	//アニメーション制御用.
	//くまさん(K).
	const int KWait = 0;
	const int KWalk = 1;
	const int KAttack = 2;
	const int KRejoice = 3;
	const int KEscape = 4;
	const int KHit = 5;
	const int KHyokori = 6;



	//くまさんの現在の動き.
	enum  class enKumaMove
	{
		Wait = 0,
		Move,
		Attack,
		Stop,
		Escape,
	};

	//テクスチャ差し替え.
	enum class enSkinChange
	{
		Clean = 0,
		Middle,
		Dirty,
	};


	//くまさん構造体.
	struct BEAR_PARAM
	{
		D3DXVECTOR3 m_Pos;		//ポジション(座標).
		D3DXVECTOR3 m_Rot;		//ローテーション(回転).
		D3DXVECTOR3 m_InitPos;	//初期位置取得用.
		D3DXVECTOR3 m_OldPos;	//当たり判定調節用.
		float m_Speed;			//くまのスピード.

		int m_anim_no;
		double m_anim_time;
		enSkinChange m_enSkin;	//現在のテクスチャ.
		//------------------------------------
		// 吉田.
		enKumaMove m_enKMove;//くまさんのみ.
		int m_InitDispCnt; //初期値取得用.
		int m_AppearCnt;		//出現カウント.
		bool m_TexChangeFlag;	//テクスチャ切り替えしてる？.
		int  m_SkinCnt;			//テクスチャの汚れ具合.
		int m_InitSkinCnt;		//初期値取得用.
		int	m_MovePattern;		//移動パターン.
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
			, m_enSkin(enSkinChange::Dirty)//初期で汚いが入っている.
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





	//情報を更新.
	//※拡縮や回転、座標などの値をﾓﾃﾞﾙﾃﾞｰﾀに反映.
	void Update() final override;
	//モデルデータの情報を管理.
	void MeshCreate() final override;


	BEAR_PARAM  m_KumaParam[BEAR_MAX];


};
#endif //#ifndef CKUMA_H