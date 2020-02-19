#ifndef CPLAYER_H
#define CPLAYER_H
#include "CGameObject.h"

const int	SMALL_DAMAGE = 1;	//ダメージ量(小)
const int	MIDDLE_DAMAGE = 2;	//ダメージ量(中)
const int	BIG_DAMAGE = 3;		//ダメージ量(大)
const int	LAST_DAMAGE = 4;	//プレイヤーがコンテニューになるためのダメージ.
class CPlayer
	:public	CGameObject
{
public:
	//自身のダメージの列挙体.
	enum class enDamageParam
	{
		enNoDamage = 0,

		enDamage_1,
		enDamage_2,
		enDamage_3,
	};
	CPlayer();
	~CPlayer();

	void Init();		//初期化.
	void DamageInit();	//ダメージの初期化.

	void Move(bool m_flag);	//プレイヤーの動き.

	void UpDamage() { m_DamageCnt += 1;	 }		//ダメージを受けた時.
	int	 GetDamage() { return m_DamageCnt; }	//ダメージ情報を取得.
private:
	int				m_DamageCnt;	//ダメージのカウント.
};
#endif //#ifndef CPLAYER_H

